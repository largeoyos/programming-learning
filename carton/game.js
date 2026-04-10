// ── 常量 ──────────────────────────────────────────────
const RES_NAME  = ['木材','砖块','粮食','羊毛','矿石'];
const RES_EMOJI = ['🌲','🧱','🌾','🐑','⛏️'];
const DESERT    = 5;
const PORT_TYPE = ['木材','砖块','粮食','羊毛','矿石','3:1','3:1','3:1','3:1'];  // 5个2:1资源港 + 4个3:1港
const TILE_RES  = shuffle([0,0,0,0,1,1,1,2,2,2,2,3,3,3,3,4,4,4,DESERT]);
const TILE_NUMS = shuffle([2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12]);
const P_COLORS  = ['#e63946','#f4a261','#2ec4b6','#a8dadc'];
const P_NAMES   = ['你','AI-赤','AI-橙','AI-青'];

const TILE_COLOR = ['#2d6a4f','#c1440e','#e9c46a','#74c69d','#8ecae6','#c4a35a'];
const TILE_LABEL = ['木材','砖块','粮食','羊毛','矿石','沙漠'];

// ── 棋盘几何 ─────────────────────────────────────────
const CW=520, CH=560, OX=CW/2, OY=CH/2, HR=48;
const ROW_COUNTS=[3,4,5,4,3];

const LAYOUT=[];
let tidx=0;
for(let r=0;r<5;r++){
  const n=ROW_COUNTS[r];
  for(let c=0;c<n;c++){
    const x=OX-(n-1)*HR*Math.sqrt(3)/2+c*HR*Math.sqrt(3);
    const y=OY+(r-2)*HR*1.5;
    LAYOUT.push({x,y,idx:tidx++});
  }
}

// ── 港口位置 ────────────────────────────────────────
function shuffle(a){const b=[...a];for(let i=b.length-1;i>0;i--){const j=Math.floor(Math.random()*(i+1));[b[i],b[j]]=[b[j],b[i]]}return b}

const PORTS = shuffle(PORT_TYPE).map((type, i) => {
  const angle = (i * 40) * Math.PI / 180;
  const dist = 6 * HR;  // 增加距离避免重叠
  return {
    x: OX + dist * Math.cos(angle),
    y: OY + dist * Math.sin(angle),
    angle: (i * 40),
    type: type
  };
});

function hexCorners(cx,cy,r=HR){
  return Array.from({length:6},(_,i)=>{
    const a=Math.PI/180*(60*i-30);
    return [cx+r*Math.cos(a), cy+r*Math.sin(a)];
  });
}

// ── 顶点 & 边 ────────────────────────────────────────
function buildVertices(){
  const verts=[];
  const EPS=5;
  LAYOUT.forEach(tile=>{
    hexCorners(tile.x,tile.y).forEach(([vx,vy])=>{
      if(!verts.some(v=>Math.hypot(v.x-vx,v.y-vy)<EPS))
        verts.push({x:vx,y:vy,adj:[],owner:null,type:null});
    });
  });
  verts.forEach((v,i)=>{
    verts.forEach((u,j)=>{
      if(i!==j&&Math.hypot(v.x-u.x,v.y-u.y)<HR*1.05&&!v.adj.includes(j))
        v.adj.push(j);
    });
  });
  return verts;
}

// ── 游戏状态 ─────────────────────────────────────────
const G={
  tiles: TILE_RES,
  nums:  (()=>{let ni=0;return TILE_RES.map(r=>r===DESERT?0:TILE_NUMS[ni++])})(),
  robber: TILE_RES.indexOf(DESERT),
  verts: buildVertices(),
  ports: PORTS,
  roads: [],          // [vi, vj, playerIdx]
  players: P_NAMES.map((n,i)=>({
    name:n, color:P_COLORS[i],
    res:[0,0,0,0,0],
    vp:0, roads:0, settlements:0, cities:0
  })),
  turn:0, phase:'setup', setupStep:0,
  diceVal:0, log:[],
  selected:null, mode:'settle',
  longestRoadOwner:-1
};

// ── 工具函数 ─────────────────────────────────────────
function addLog(msg, mine=false){
  G.log.unshift({msg,mine});
  if(G.log.length>40)G.log.pop();
}

function canBuildSettlement(vi){
  const v=G.verts[vi];
  if(v.owner!==null)return false;
  if(v.adj.some(j=>G.verts[j].owner!==null))return false;
  if(G.phase==='setup')return true;
  const p=G.players[G.turn];
  if(p.res[0]<1||p.res[1]<1||p.res[2]<1||p.res[3]<1)return false;
  return hasAdjacentRoad(vi, G.turn);
}

function hasAdjacentRoad(vi, pi){
  return G.roads.some(([a,b,p])=>p===pi&&(a===vi||b===vi));
}

function canBuildRoad(vi, vj){
  // 检查两个顶点是否相邻
  if(!G.verts[vi].adj.includes(vj))return false;
  // 检查该道路是否已经存在
  if(G.roads.some(([a,b])=>(a===vi&&b===vj)||(a===vj&&b===vi)))return false;
  // 准备阶段：必须连接到已建设的村庄
  if(G.phase==='setup'){
    const hasSettlement = G.verts[vi].owner===G.turn || G.verts[vj].owner===G.turn;
    return hasSettlement;
  }
  // 正常阶段需要检查资源
  const p=G.players[G.turn];
  if(p.res[0]<1||p.res[1]<1)return false;
  // 必须挨着城镇或现有道路
  const hasSettlement = G.verts[vi].owner===G.turn || G.verts[vj].owner===G.turn;
  const hasAdjacentRd = hasAdjacentRoad(vi,G.turn) || hasAdjacentRoad(vj,G.turn);
  return hasSettlement || hasAdjacentRd;
}

function buildSettlement(vi){
  const v=G.verts[vi];
  const pi=G.turn;
  v.owner=pi; v.type='settlement';
  G.players[pi].vp++;
  G.players[pi].settlements++;
  addLog(`${G.players[pi].name} 建造了聚落`, pi===0);
  if(G.phase==='setup'){
    if(G.setupStep>=G.players.length)
      distributeInitialResources(vi, pi);
    G.mode='road';
  } else {
    G.players[pi].res[0]--;G.players[pi].res[1]--;
    G.players[pi].res[2]--;G.players[pi].res[3]--;
  }
  render();
}

function distributeInitialResources(vi, pi){
  LAYOUT.forEach((tile,i)=>{
    if(G.tiles[i]===DESERT)return;
    const corn=hexCorners(tile.x,tile.y);
    if(corn.some(([cx,cy])=>Math.hypot(cx-G.verts[vi].x,cy-G.verts[vi].y)<6)){
      G.players[pi].res[G.tiles[i]]++;
      addLog(`${G.players[pi].name} 获得初始 ${RES_NAME[G.tiles[i]]}`, pi===0);
    }
  });
}

function buildRoad(vi, vj){
  const pi=G.turn;
  G.roads.push([vi,vj,pi]);
  G.players[pi].roads++;
  addLog(`${G.players[pi].name} 建造了道路`, pi===0);
  if(G.phase!=='setup'){
    G.players[pi].res[0]--;G.players[pi].res[1]--;
  }
  checkLongestRoad();
  if(G.phase==='setup'){
    G.setupStep++;
    G.selected=null;
    nextSetupTurn();
  } else {
    G.mode='none';G.selected=null;
  }
  render();
}

function buildCity(vi){
  const pi=G.turn;
  if(G.verts[vi].owner!==pi||G.verts[vi].type!=='settlement')return;
  if(G.players[pi].res[2]<2||G.players[pi].res[4]<3)return;
  G.verts[vi].type='city';
  G.players[pi].res[2]-=2;G.players[pi].res[4]-=3;
  G.players[pi].vp++;G.players[pi].cities++;G.players[pi].settlements--;
  addLog(`${G.players[pi].name} 升级为城市！`, pi===0);
  checkVictory();render();
}

// ── 道路长度计算 ─────────────────────────────────────
function calcRoadLen(pi){
  const edges=G.roads.filter(r=>r[2]===pi).map(r=>[r[0],r[1]]);
  if(!edges.length)return 0;
  let best=0;
  const nodes=new Set(edges.flat());
  nodes.forEach(start=>{
    const dfs=(cur,used,len)=>{
      best=Math.max(best,len);
      edges.forEach(([a,b],ei)=>{
        if(used[ei])return;
        const nxt=a===cur?b:b===cur?a:-1;
        if(nxt===-1)return;
        // 不能穿越对方聚落
        const midV=G.verts[cur];
        if(midV.owner!==null&&midV.owner!==pi&&cur!==start)return;
        used[ei]=true; dfs(nxt,used,len+1); used[ei]=false;
      });
    };
    dfs(start,{},0);
  });
  return best;
}

function checkLongestRoad(){
  let bestPi=-1, bestLen=4;
  G.players.forEach((_,i)=>{
    const l=calcRoadLen(i);
    if(l>bestLen){bestLen=l;bestPi=i;}
  });
  if(bestPi!==-1&&bestPi!==G.longestRoadOwner){
    if(G.longestRoadOwner!==-1)G.players[G.longestRoadOwner].vp-=2;
    G.longestRoadOwner=bestPi;
    G.players[bestPi].vp+=2;
    addLog(`${G.players[bestPi].name} 获得最长道路！+2VP`, bestPi===0);
    checkVictory();
  }
}

// ── 资源分配 ────────────────────────────────────────
function distributeResources(num){
  G.tiles.forEach((res,i)=>{
    if(G.nums[i]!==num||i===G.robber||res===DESERT)return;
    G.verts.forEach(v=>{
      if(v.owner===null)return;
      const corn=hexCorners(LAYOUT[i].x,LAYOUT[i].y);
      if(corn.some(([cx,cy])=>Math.hypot(cx-v.x,cy-v.y)<6)){
        const qty=v.type==='city'?2:1;
        G.players[v.owner].res[res]+=qty;
        addLog(`${G.players[v.owner].name} 获得 ${qty}${RES_NAME[res]}`, v.owner===0);
      }
    });
  });
}

// ── 胜利检测 ─────────────────────────────────────────
function checkVictory(){
  const winner=G.players.find(p=>p.vp>=10);
  if(!winner)return;
  document.getElementById('victory-title').textContent=
    winner.name==='你'?'🏆 你赢了！':`${winner.name} 赢得胜利！`;
  document.getElementById('victory-sub').textContent=
    `${winner.name} 达到了 ${winner.vp} 胜利点数`;
  document.getElementById('victory-overlay').classList.add('show');
}

// ── 准备阶段流程 ──────────────────────────────────────
function nextSetupTurn(){
  const n=G.players.length;
  const step=G.setupStep;
  if(step<n){
    G.turn=step; G.mode='settle';
  } else if(step<n*2){
    G.turn=n*2-1-step; G.mode='settle';
  } else {
    G.turn=0; G.phase='main'; G.mode='none';
    addLog('── 准备完成，游戏开始！──');
  }
  render();
  if(G.phase==='setup'&&G.turn!==0) setTimeout(aiSetupAction,700);
  if(G.phase==='main'&&G.turn!==0) setTimeout(aiFullTurn,800);
}

function aiSetupAction(){
  if(G.phase!=='setup'||G.turn===0)return;
  // 选得分最高的顶点
  const avail=G.verts.map((_,i)=>i).filter(i=>canBuildSettlement(i));
  if(!avail.length){nextSetupTurn();return;}
  const scored=avail.map(i=>{
    let score=0;
    LAYOUT.forEach((tile,ti)=>{
      if(G.tiles[ti]===DESERT)return;
      const corn=hexCorners(tile.x,tile.y);
      if(corn.some(([cx,cy])=>Math.hypot(cx-G.verts[i].x,cy-G.verts[i].y)<6)){
        const pip=[0,0,1,2,3,4,5,4,3,0,5,4,3,2,1,2,3,4,5][G.nums[ti]]||1;
        score+=pip+(G.tiles[ti]===2||G.tiles[ti]===3?0.5:0);
      }
    });
    return {i,score};
  }).sort((a,b)=>b.score-a.score);
  const best=scored[0].i;
  buildSettlement(best);
  if(G.mode==='road'){
    const adj=G.verts[best].adj.filter(j=>canBuildRoad(best,j));
    if(adj.length>0) setTimeout(()=>buildRoad(best,adj[0]),400);
  }
}

// ── AI 主回合 ─────────────────────────────────────────
function aiFullTurn(){
  if(G.phase!=='main'||G.turn===0)return;
  // 投骰
  const d1=Math.ceil(Math.random()*6),d2=Math.ceil(Math.random()*6);
  G.diceVal=d1+d2;
  const faces=['','⚀','⚁','⚂','⚃','⚄','⚅'];
  document.getElementById('dice-display').textContent=faces[d1]+' '+faces[d2];
  addLog(`${G.players[G.turn].name} 投出 ${G.diceVal}`);
  if(G.diceVal===7){
    // 移动强盗到玩家聚落最少的地块
    const notDesert=LAYOUT.map((_,i)=>i).filter(i=>i!==G.robber&&G.tiles[i]!==DESERT);
    G.robber=notDesert[Math.floor(Math.random()*notDesert.length)];
    addLog(`${G.players[G.turn].name} 移动强盗`);
  } else {
    distributeResources(G.diceVal);
  }
  render();
  setTimeout(()=>{
    aiSpend();
    endTurn();
  },600);
}

function aiSpend(){
  const pi=G.turn;
  const p=G.players[pi];
  let acted=true;
  while(acted){
    acted=false;
    // 升级城市
    if(p.res[2]>=2&&p.res[4]>=3){
      const cv=G.verts.findIndex(v=>v.owner===pi&&v.type==='settlement');
      if(cv!==-1){buildCity(cv);acted=true;continue;}
    }
    // 建聚落
    if(p.res[0]>=1&&p.res[1]>=1&&p.res[2]>=1&&p.res[3]>=1){
      const avail=G.verts.map((_,i)=>i).filter(i=>{
        const v=G.verts[i];
        if(v.owner!==null)return false;
        if(v.adj.some(j=>G.verts[j].owner!==null))return false;
        return hasAdjacentRoad(i,pi);
      });
      if(avail.length>0){
        G.turn=pi; buildSettlement(avail[0]);
        G.players[pi].res[0]++;G.players[pi].res[1]++;
        G.players[pi].res[2]++;G.players[pi].res[3]++;
        // buildSettlement already deducts; re-apply
        G.players[pi].res[0]--;G.players[pi].res[1]--;
        G.players[pi].res[2]--;G.players[pi].res[3]--;
        // 重新调用避免双扣
        acted=true;continue;
      }
    }
    // 建道路
    if(p.res[0]>=1&&p.res[1]>=1){
      const myVerts=G.verts.map((_,i)=>i).filter(i=>
        G.verts[i].owner===pi||hasAdjacentRoad(i,pi)
      );
      let built=false;
      for(const vi of myVerts){
        for(const vj of G.verts[vi].adj){
          if(canBuildRoad(vi,vj)){
            G.roads.push([vi,vj,pi]);
            p.res[0]--;p.res[1]--;
            p.roads++;
            addLog(`${p.name} 建造了道路`);
            checkLongestRoad();
            built=true;break;
          }
        }
        if(built)break;
      }
      if(built){acted=true;continue;}
    }
  }
}

function endTurn(){
  checkVictory();
  if(G.phase==='end')return;
  G.turn=(G.turn+1)%G.players.length;
  G.phase='main';G.mode='none';G.selected=null;
  addLog(`── ${G.players[G.turn].name} 的回合 ──`, G.turn===0);
  render();
  if(G.turn!==0)setTimeout(aiFullTurn,800);
}

// ── 掷骰（玩家） ─────────────────────────────────────
function playerRoll(){
  const d1=Math.ceil(Math.random()*6),d2=Math.ceil(Math.random()*6);
  G.diceVal=d1+d2;
  const faces=['','⚀','⚁','⚂','⚃','⚄','⚅'];
  document.getElementById('dice-display').textContent=faces[d1]+' '+faces[d2];
  addLog(`你投出了 ${G.diceVal}`, true);
  if(G.diceVal===7){
    G.phase='robber';
    addLog('骰子7！点击地块放置强盗', true);
  } else {
    distributeResources(G.diceVal);
    G.phase='action';
  }
  render();
}

// ── 画布点击 ─────────────────────────────────────────
document.getElementById('board').addEventListener('click',function(e){
  const rect=this.getBoundingClientRect();
  const sx=(e.clientX-rect.left)*(CW/rect.width);
  const sy=(e.clientY-rect.top)*(CH/rect.height);

  // 强盗模式：点地块
  if(G.phase==='robber'&&G.turn===0){
    const ti=LAYOUT.findIndex(t=>Math.hypot(t.x-sx,t.y-sy)<HR);
    if(ti!==-1&&G.tiles[ti]!==DESERT){
      G.robber=ti;
      addLog('你移动了强盗',true);
      G.phase='action';render();
    }
    return;
  }

  if(G.turn!==0)return;

  if(G.mode==='settle'){
    const vi=G.verts.findIndex(v=>Math.hypot(v.x-sx,v.y-sy)<14);
    if(vi!==-1&&canBuildSettlement(vi)) buildSettlement(vi);
  } else if(G.mode==='road'){
    const vi=G.verts.findIndex(v=>Math.hypot(v.x-sx,v.y-sy)<14);
    if(vi===-1)return;
    if(G.selected===null){G.selected=vi;render();}
    else if(G.selected===vi){G.selected=null;render();}
    else if(canBuildRoad(G.selected,vi)){buildRoad(G.selected,vi);}
    else{G.selected=vi;render();}
  } else if(G.mode==='city'){
    const vi=G.verts.findIndex(v=>Math.hypot(v.x-sx,v.y-sy)<14);
    if(vi!==-1) buildCity(vi);
  }
});

// ── 绘制棋盘 ─────────────────────────────────────────
function drawBoard(){
  const cv=document.getElementById('board');
  const ctx=cv.getContext('2d');
  ctx.clearRect(0,0,CW,CH);

  // 海洋背景
  ctx.fillStyle='#1a3a5c';
  ctx.fillRect(0,0,CW,CH);

  // 地块
  LAYOUT.forEach((tile,i)=>{
    const corn=hexCorners(tile.x,tile.y,HR-1);
    ctx.beginPath();
    corn.forEach(([x,y],j)=>j?ctx.lineTo(x,y):ctx.moveTo(x,y));
    ctx.closePath();
    ctx.fillStyle=TILE_COLOR[G.tiles[i]];
    ctx.fill();
    ctx.strokeStyle='rgba(0,0,0,0.3)';ctx.lineWidth=1.5;ctx.stroke();

    // 强盗标记
    if(i===G.robber){
      ctx.font='22px sans-serif';ctx.textAlign='center';ctx.textBaseline='middle';
      ctx.fillText('🦹',tile.x,tile.y+(G.nums[i]>0?-18:0));
    }

    // 数字令牌
    if(G.nums[i]>0){
      const isHot=G.nums[i]===6||G.nums[i]===8;
      ctx.beginPath();ctx.arc(tile.x,tile.y,15,0,Math.PI*2);
      ctx.fillStyle='rgba(255,248,220,0.92)';ctx.fill();
      ctx.strokeStyle='rgba(0,0,0,0.15)';ctx.lineWidth=1;ctx.stroke();
      ctx.fillStyle=isHot?'#c1440e':'#1a1a1a';
      ctx.font=`bold 13px system-ui`;
      ctx.textAlign='center';ctx.textBaseline='middle';
      ctx.fillText(G.nums[i],tile.x,tile.y);
      // 点数
      const pip=[0,0,1,2,3,4,5,4,3,0,5,4,3,2,1,2,3,4,5][G.nums[i]]||0;
      const dots='·'.repeat(pip);
      ctx.font='10px system-ui';ctx.fillStyle=isHot?'#c1440e':'#555';
      ctx.fillText(dots,tile.x,tile.y+10);
    }

    // 地块标签
    ctx.font='9px system-ui';ctx.fillStyle='rgba(0,0,0,0.4)';
    ctx.textAlign='center';ctx.textBaseline='middle';
    ctx.fillText(TILE_LABEL[G.tiles[i]],tile.x,tile.y-(G.nums[i]>0?20:4));
  });

  // 道路
  G.roads.forEach(([vi,vj,pi])=>{
    const a=G.verts[vi],b=G.verts[vj];
    ctx.beginPath();ctx.moveTo(a.x,a.y);ctx.lineTo(b.x,b.y);
    ctx.strokeStyle=P_COLORS[pi];ctx.lineWidth=5;ctx.stroke();
    ctx.strokeStyle='rgba(0,0,0,0.2)';ctx.lineWidth=1;ctx.stroke();
  });

  // 港口
  G.ports.forEach(port=>{
    ctx.beginPath();ctx.arc(port.x,port.y,14,0,Math.PI*2);
    ctx.fillStyle='rgba(200,180,100,0.8)';ctx.fill();
    ctx.strokeStyle='#d4a574';ctx.lineWidth=2;ctx.stroke();
    ctx.font='9px system-ui';ctx.textAlign='center';ctx.textBaseline='middle';
    ctx.fillStyle='#1a1a2e';
    if(port.type==='3:1'){
      ctx.fillText('3:1',port.x,port.y);
    } else {
      ctx.fillText(port.type.slice(0,1),port.x,port.y);
    }
  });

  // 可建提示（玩家回合）
  if(G.turn===0){
    if(G.mode==='settle'){
      G.verts.forEach((v,i)=>{
        if(canBuildSettlement(i)){
          ctx.beginPath();ctx.arc(v.x,v.y,8,0,Math.PI*2);
          ctx.strokeStyle='rgba(244,211,94,0.8)';ctx.lineWidth=2.5;ctx.stroke();
          ctx.fillStyle='rgba(244,211,94,0.2)';ctx.fill();
        }
      });
    }
    if(G.mode==='road'&&G.selected!==null){
      const sv=G.verts[G.selected];
      sv.adj.forEach(j=>{
        if(canBuildRoad(G.selected,j)){
          ctx.beginPath();ctx.arc(G.verts[j].x,G.verts[j].y,8,0,Math.PI*2);
          ctx.strokeStyle='rgba(244,211,94,0.8)';ctx.lineWidth=2.5;ctx.stroke();
        }
      });
    }
    if(G.mode==='city'){
      G.verts.forEach((v,i)=>{
        if(v.owner===0&&v.type==='settlement'){
          ctx.beginPath();ctx.arc(v.x,v.y,10,0,Math.PI*2);
          ctx.strokeStyle='rgba(244,211,94,0.8)';ctx.lineWidth=2.5;ctx.stroke();
        }
      });
    }
  }

  // 选中顶点
  if(G.selected!==null){
    const sv=G.verts[G.selected];
    ctx.beginPath();ctx.arc(sv.x,sv.y,11,0,Math.PI*2);
    ctx.strokeStyle='#f4d35e';ctx.lineWidth=3;ctx.stroke();
  }

  // 聚落 & 城市
  G.verts.forEach((v,i)=>{
    if(v.owner===null)return;
    const col=P_COLORS[v.owner];
    if(v.type==='city'){
      ctx.beginPath();
      ctx.rect(v.x-9,v.y-9,18,18);
      ctx.fillStyle=col;ctx.fill();
      ctx.strokeStyle='rgba(255,255,255,0.7)';ctx.lineWidth=2;ctx.stroke();
      ctx.font='10px system-ui';ctx.textAlign='center';ctx.textBaseline='middle';
      ctx.fillStyle='white';ctx.fillText('🏙',v.x,v.y);
    } else {
      ctx.beginPath();ctx.arc(v.x,v.y,8,0,Math.PI*2);
      ctx.fillStyle=col;ctx.fill();
      ctx.strokeStyle='rgba(255,255,255,0.7)';ctx.lineWidth=2;ctx.stroke();
    }
  });
}

// ── 渲染UI ────────────────────────────────────────────
function renderUI(){
  // 玩家列表
  document.getElementById('players-list').innerHTML=G.players.map((p,i)=>`
    <div class="player-row${i===G.turn?' active-row':''}">
      <div class="pcolor" style="background:${p.color}${i===G.longestRoadOwner?';outline:2px solid gold':''}"></div>
      <span class="pname">${p.name}${i===G.turn?' ◀':''}</span>
      <span class="pvp" style="color:${p.vp>=8?'#f4d35e':'rgba(255,255,255,0.5)'}">${p.vp}VP</span>
      <span class="pinfo">🏘${p.settlements+p.cities} 🛣${p.roads}</span>
    </div>`).join('');

  // 我的资源
  const p0=G.players[0];
  document.getElementById('my-resources').innerHTML=RES_NAME.map((r,i)=>`
    <div class="res-item">
      <span class="res-icon">${RES_EMOJI[i]}</span>
      <div class="res-label">${r}</div>
      <div class="res-count" style="color:${p0.res[i]>0?'#eee':'rgba(255,255,255,0.2)'}">${p0.res[i]}</div>
    </div>`).join('');

  // 阶段
  const phaseMap={setup:'准备阶段',main:'掷骰阶段',action:'行动阶段',robber:'强盗！',end:'游戏结束'};
  document.getElementById('phase-label').textContent=phaseMap[G.phase]||G.phase;

  // 行动按钮
  const ab=document.getElementById('action-btns');
  ab.innerHTML='';
  if(G.turn===0&&G.phase!=='end'){
    const mk=(txt,cls,fn,cost='')=>{
      const b=document.createElement('button');
      b.className='btn '+(cls||'');
      b.innerHTML=txt+(cost?`<div class="build-costs">${cost}</div>`:'');
      b.onclick=fn; ab.appendChild(b);
    };
    if(G.phase==='main'){
      mk('🎲 掷骰子','btn-primary',playerRoll);
    }
    if(G.phase==='action'){
      mk('🏘 建造聚落','',()=>{G.mode='settle';G.selected=null;render();},'🌲🧱🌾🐑 各1');
      mk('🛣 建造道路','',()=>{G.mode='road';G.selected=null;render();},'🌲🧱 各1');
      if(p0.res[2]>=2&&p0.res[4]>=3)
        mk('🏙 升级城市','',()=>{G.mode='city';G.selected=null;render();},'🌾×2 ⛏️×3');
      mk('⏩ 结束回合','',endTurn);
    }
    if(G.phase==='robber'){
      mk('点击地块放置强盗（避开沙漠）','',()=>{});
    }
    if(G.phase==='setup'&&G.mode==='settle') mk('点击棋盘放置聚落','',()=>{});
    if(G.phase==='setup'&&G.mode==='road')   mk('选择起点 → 终点放置道路','',()=>{});
  }

  // 日志
  document.getElementById('log').innerHTML=G.log.map(({msg,mine})=>
    `<p${mine?' class="mine"':''}>${msg}</p>`).join('');
}

function render(){drawBoard();renderUI();}

// ── 启动 ─────────────────────────────────────────────
document.getElementById('restart-btn').addEventListener('click',()=>location.reload());
document.getElementById('home-btn').addEventListener('click',()=>{if(confirm('确定要返回主页吗？'))location.href='/';});
document.getElementById('quit-btn').addEventListener('click',()=>{if(confirm('确定要结束游戏吗？'))location.reload();});

addLog('── 游戏开始，准备阶段 ──');
addLog('点击棋盘黄色提示点放置聚落', true);
render();
if(G.turn!==0&&G.phase==='setup') setTimeout(aiSetupAction,600);