import random
from engine import PokerEngine


class AIAgent:
    def __init__(self, name, aggression=0.5, bluff_frequency=0.2):
        self.name = name
        # aggression: 越大越倾向于加注/梭哈
        self.aggression = max(0.0, float(aggression))
        # bluff_frequency: 基线诈唬概率
        self.bluff_frequency = max(0.0, float(bluff_frequency))
        self.wins = 0  # 记录胜场

    def set_params(self, agg, bluff):
        self.aggression = max(0.0, float(agg))
        self.bluff_frequency = max(0.0, float(bluff))

    def _calc_draw_potential(self, hand, score, num_cards):
        """
        评估摸牌潜力，仅在前3张牌时考虑。
        - hand: 当前手牌
        - score: 当前牌型分数
        - num_cards: 当前手牌张数
        """
        if num_cards > 3 or score >= 4:  # 只在早期且非强牌时考虑潜力
            return 0.0

        ranks = sorted([PokerEngine.RANK_MAP[card[0]] for card in hand])
        suits = [card[1] for card in hand]
        
        potential_bonus = 0.0

        # 同花听牌
        suit_counts = Counter(suits)
        for suit, count in suit_counts.items():
            if count == 4: # 差一张同花
                potential_bonus += 0.25
                break

        # 顺子听牌 (开放式两头顺子)
        # 检查是否有4张牌是连续的，且两端都可构成顺子 (如 3,4,5,6)
        if num_cards >= 4:
            for i in range(len(ranks) - 3):
                sub_ranks = sorted(list(set(ranks[i:i+4])))
                if len(sub_ranks) == 4 and (sub_ranks[-1] - sub_ranks[0] == 3): # 4张连续牌
                    # 检查是否是开放式顺子听牌
                    # 例如：2,3,4,5 可以形成A2345或23456
                    # 或者 3,4,5,6 可以形成23456或34567
                    # 排除已经构成顺子的情况，因为score已经处理了
                    if not (sub_ranks[-1] - sub_ranks[0] == 4 and len(sub_ranks) == 5): # 避免重复计算已成顺子
                        if (sub_ranks[0] > 0 and sub_ranks[-1] < 12) or (sub_ranks[0] == 0 and sub_ranks[-1] == 3 and 12 in ranks) or (sub_ranks[0] == 9 and sub_ranks[-1] == 12 and 0 in ranks): # 开放式两头顺子
                            potential_bonus += 0.20 # 开放式顺子
                        elif (sub_ranks[0] > 0 and sub_ranks[-1] < 13) or (sub_ranks[0] == 0 and 12 in ranks) or (sub_ranks[-1] == 12 and 0 in ranks): # 仅一头顺子
                             potential_bonus += 0.10 # 单头顺子

        # 处理特殊顺子听牌 (A,2,3,4 或 J,Q,K,A) -- 5张牌时
        if num_cards == 4:
            # A,2,3,4 听5
            if set(ranks) == {0, 1, 2, 3}: potential_bonus += 0.15
            # 10,J,Q,K 听A
            if set(ranks) == {9, 10, 11, 12}: potential_bonus += 0.15

        return min(0.3, potential_bonus) # 限制潜力加成的上限


    def decide(self, hand, pot, wallet, current_max_bet, total_players):
        """
        返回 (action, amt)
        - action: one of 'fold','call','raise'
        - amt: 当 action=='raise' 时，表示希望在当前最高注上额外增加的金额（主逻辑会把它作为 delta 加到 current_max_bet 上）

        优化后：根据牌力、轮次（由手牌张数）、底池大小、对手余额、玩家人数和摸牌潜力来决定更激进的下注尺度。
        """
        score, ranks = PokerEngine.evaluate_hand(hand)
        num_cards = len(hand)

        # 1. 细粒度强度（融合牌型+高牌信息）
        base_strength = score / 8.0
        # ranks[0] 是最高牌的点数，除以12.0（A是12）归一化，乘以0.15作为加权，并限制在1.0以内
        # 引入 kicker_bonus, 考虑最高单牌点数, 增加细粒度
        kicker_bonus = (ranks[0] / 12.0) * 0.15 
        strength = min(1.0, base_strength + kicker_bonus)

        # 2. 摸牌潜力加成（仅早中期）
        draw_bonus = self._calc_draw_potential(hand, score, num_cards)
        effective_strength = min(1.0, strength + draw_bonus)

        # 3. 多人压力因子
        # 玩家越多，对牌力的要求越高，因此有效牌力会被稀释
        player_pressure = 1.0 + (total_players - 2) * 0.12
        adjusted_strength = effective_strength / player_pressure

        # 4. Pot Odds 检查
        # 计算需要跟注的筹码与底池的比例，如果这个比例低于自身牌力，则认为跟注划算
        pot_odds_threshold = current_max_bet / (pot + current_max_bet + 1) # 加1避免除以零
        pot_odds_ok = adjusted_strength > pot_odds_threshold
        
        # 当前需要跟的筹码比率（相对于 AI 自己的资金）
        risk_ratio = current_max_bet / (wallet + 1) # 加1避免除以零

        # === 决策逻辑 ===

        # 若无人下注 (current_max_bet == 0)，AI 可选择做开局下注
        if current_max_bet == 0:
            if adjusted_strength >= 0.6:
                # 强牌直接大幅加注或梭哈
                amt = int(wallet * (0.6 + 0.3 * self.aggression))
                return "raise", max(10, min(amt, wallet))
            elif adjusted_strength >= 0.3:
                # 中等牌，按底池比例下注，并考虑侵略性
                base = int(max(20, pot * (0.25 + 0.5 * self.aggression)))
                return "raise", min(base, wallet)
            else:
                # 低牌，考虑诈唬
                # 诈唬概率随轮次增加，前期更少诈唬
                bluff_chance_adjusted = self.bluff_frequency * (0.5 + 0.5 * self.aggression) * ((num_cards / 5.0) ** 1.5)
                if random.random() < bluff_chance_adjusted:
                    return "raise", min(int(max(10, pot * 0.4)), wallet)
                return "call", 0 # 没有理由诈唬就check

        # 如果当前已经有下注，基于牌力和风险来处理

        # 5. 分层弃牌逻辑
        # 风险高且牌力低时弃牌
        if (risk_ratio > 0.3 and effective_strength < 0.2) or \
           (risk_ratio > 0.5 and effective_strength < 0.35 and num_cards >= 4) or \
           (not pot_odds_ok and effective_strength < 0.4): # pot_odds 不划算且牌力一般
            return "fold", 0

        # 非常强的牌：在任何轮次都倾向于很大额下注或梭哈
        if adjusted_strength >= 0.6:
            if num_cards >= 4:
                # 后轮强牌更倾向于梭哈
                amt = int(wallet * (0.7 + 0.25 * self.aggression))
            else:
                # 前期强牌，更注重构建底池
                amt = int(pot * (0.6 + 0.6 * self.aggression))
            return "raise", max(10, min(amt, wallet))

        # 中等牌（如小对或两对潜力）
        if adjusted_strength >= 0.35:
            # 如果面对的风险不高，或者 pot_odds 划算，可能小加注以扩大底池；若风险高且 pot_odds 不划算则跟注
            if (risk_ratio < 0.4 or random.random() < self.aggression) and pot_odds_ok:
                amt = int(pot * (0.25 + 0.5 * self.aggression))
                return "raise", max(10, min(amt, wallet))
            elif pot_odds_ok:
                return "call", 0
            else: # pot_odds 不划算，弃牌
                return "fold", 0

        # 弱牌区域：根据底池大小和诈唬倾向决定是否诈唬
        bluff_chance_adjusted = self.bluff_frequency * (0.6 + 0.8 * self.aggression) * (1.0 + pot / (wallet + 1)) * ((num_cards / 5.0) ** 1.5)
        if random.random() < bluff_chance_adjusted and pot > 0 and num_cards >=3: # 早期诈唬成功率低
            # 诈唬量与余额相关，可能是一次性较大压制
            amt = int(wallet * (0.35 + 0.5 * self.aggression))
            return "raise", max(10, min(amt, wallet))

        # 默认跟注（或 check，如果没有需要跟的量），但要考虑 pot_odds
        if pot_odds_ok: 
            return "call", 0
        
        return "fold", 0 # 最终决策，如果不值得跟注，就弃牌
