import re

with open('e:/programming_works/Typora/Claude电路.md', 'r', encoding='utf-8') as f:
    text = f.read()

# Finding lines that contain duplicated math from Claude:
# Example: P=UIP = UIP=UI -> $P = UI$
# I1+I2=I3...①I_1 + I_2 = I_3 \quad \text{...①}I1​+I2​=I3​...① -> $I_1 + I_2 = I_3 \quad \text{...①}$

import sys

def repl(m):
    # This is quite risky to generalize fully. 
    # Let's target specific sequences found in the file.
    pass

# Instead of complex regex, let's identify the exact lines that have the duplicated math.

lines_to_replace = {
    r"P=UIP = UIP=UI": r"$P = UI$",
    r"P=−UIP = -UIP=−UI": r"$P = -UI$",
    r"P=UI=5×(−2)=−10WP = UI = 5 \times (-2) = -10\text{W} P=UI=5×(−2)=−10W": r"$P = UI = 5 \times (-2) = -10\text{W}$",
    r"∑kik=0\sum_{k} i_k = 0k∑​ik​=0": r"$\sum_{k} i_k = 0$",
    r"I1+I2=I3+I4I_1 + I_2 = I_3 + I_4I1​+I2​=I3​+I4​": r"$I_1 + I_2 = I_3 + I_4$",
    r"I1+I2−I3−I4=0I_1 + I_2 - I_3 - I_4 = 0I1​+I2​−I3​−I4​=0": r"$I_1 + I_2 - I_3 - I_4 = 0$",
    r"I1+I2=I3+I4I_1 + I_2 = I_3 + I_4 I1​+I2​=I3​+I4​": r"$I_1 + I_2 = I_3 + I_4$",
    r"∑kuk=0\sum_{k} u_k = 0k∑​uk​=0": r"$\sum_{k} u_k = 0$",
    r"U1−U2−UR=0U_1 - U_2 - U_R = 0U1​−U2​−UR​=0": r"$U_1 - U_2 - U_R = 0$",
    r"UR=U1−U2U_R = U_1 - U_2 UR​=U1​−U2​": r"$U_R = U_1 - U_2$",
    r"I1+I2=I3...①I_1 + I_2 = I_3 \quad \text{...①}I1​+I2​=I3​...①": r"$I_1 + I_2 = I_3 \quad \text{...①}$",
    r"10−I1⋅1−I3⋅2=0...②10 - I_1 \cdot 1 - I_3 \cdot 2 = 0 \quad \text{...②}10−I1​⋅1−I3​⋅2=0...②": r"$10 - I_1 \cdot 1 - I_3 \cdot 2 = 0 \quad \text{...②}$",
    r"4−I2⋅1−I3⋅2=0...③4 - I_2 \cdot 1 - I_3 \cdot 2 = 0 \quad \text{...③}4−I2​⋅1−I3​⋅2=0...③": r"$4 - I_2 \cdot 1 - I_3 \cdot 2 = 0 \quad \text{...③}$",
    r"I1=3.2AI_1 = 3.2\text{A} I1​=3.2A": r"$I_1 = 3.2\text{A}$",
    r"I2=−0.4AI_2 = -0.4\text{A} I2​=−0.4A": r"$I_2 = -0.4\text{A}$",
    r"I3=2.8AI_3 = 2.8\text{A} I3​=2.8A": r"$I_3 = 2.8\text{A}$",
    r"∑i=0\sum i = 0 ∑i=0": r"$\sum i = 0$",
    r"∑u=0\sum u = 0 ∑u=0": r"$\sum u = 0$",
    r"u=Ri或i=Guu = Ri \quad \text{或} \quad i = Guu=Ri或i=Gu": r"$u = Ri \quad \text{或} \quad i = Gu$",
    r"u=−Riu = -Ri u=−Ri": r"$u = -Ri$",
    r"P=ui=Ri2=Gu2≥0P = ui = Ri^2 = Gu^2 \geq 0P=ui=Ri2=Gu2≥0": r"$P = ui = Ri^2 = Gu^2 \geq 0$",
    r"us=0u_s = 0 us​=0": r"$u_s = 0$",
    r"is=0i_s = 0 is​=0": r"$i_s = 0$",
    r"iCi_C iC​": r"$i_C$",
    r"iBi_B iB​": r"$i_B$",
    r"iC=βiBi_C = \beta i_B iC​=βiB​": r"$i_C = \beta i_B$",
    r"u2u_2 u2​": r"$u_2$",
    r"u1u_1 u1​": r"$u_1$",
    r"u2=μu1u_2 = \mu u_1 u2​=μu1​": r"$u_2 = \mu u_1$",
    r"i2i_2 i2​": r"$i_2$",
    r"i2=gu1i_2 = g u_1 i2​=gu1​": r"$i_2 = g u_1$",
    r"i1i_1 i1​": r"$i_1$",
    r"u2=ri1u_2 = r i_1 u2​=ri1​": r"$u_2 = r i_1$",
    r"i2=βi1i_2 = \beta i_1 i2​=βi1​": r"$i_2 = \beta i_1$",
    r"i1=2Ai_1 = 2\text{A} i1​=2A": r"$i_1 = 2\text{A}$",
    r"i2=6Ai_2 = 6\text{A} i2​=6A": r"$i_2 = 6\text{A}$",
    r"i1=0i_1 = 0 i1​=0": r"$i_1 = 0$",
    r"i2=0i_2 = 0 i2​=0": r"$i_2 = 0$",
    r"I1I_1 I1​": r"$I_1$",
    r"4I4I 4I": r"$4I$",
    r"UxU_x Ux​": r"$U_x$",
    r"UCCVS=5IU_{CCVS} = 5I UCCVS​=5I": r"$U_{CCVS} = 5I$",
    r"12−2I−UCCVS=012 - 2I - U_{CCVS} = 012−2I−UCCVS​=0": r"$12 - 2I - U_{CCVS} = 0$",
    r"12−2I−5I=0  ⟹  I=127≈1.71A12 - 2I - 5I = 0 \implies I = \frac{12}{7} \approx 1.71 \text{A}12−2I−5I=0⟹I=712​≈1.71A": r"$12 - 2I - 5I = 0 \implies I = \frac{12}{7} \approx 1.71 \text{A}$",
    r"I=127I = \frac{12}{7} I=712​": r"$I = \frac{12}{7}$",
    r"UCCVS=5I=607U_{CCVS} = 5I = \frac{60}{7} UCCVS​=5I=760​": r"$U_{CCVS} = 5I = \frac{60}{7}$",
    r"Ux=UCCVS=607U_x = U_{CCVS} = \frac{60}{7} Ux​=UCCVS​=760​": r"$U_x = U_{CCVS} = \frac{60}{7}$",
    r"n−1n-1 n−1": r"$n-1$",
    r"b−(n−1)=b−n+1b-(n-1) = b-n+1 b−(n−1)=b−n+1": r"$b-(n-1) = b-n+1$",
    r"b−n+1b-n+1 b−n+1": r"$b-n+1$",
    r"n=4n=4 n=4": r"$n=4$",
    r"b=5b=5 b=5": r"$b=5$",
    r"5−4+1=25-4+1 = 2 5−4+1=2": r"$5-4+1 = 2$",
    r"I1,I2,I3I_1, I_2, I_3 I1​,I2​,I3​": r"$I_1, I_2, I_3$",
    r"I1+I2=I3I_1 + I_2 = I_3I1​+I2​=I3​": r"$I_1 + I_2 = I_3$",
    r"12−2I1−6I3=012 - 2I_1 - 6I_3 = 012−2I1​−6I3​=0": r"$12 - 2I_1 - 6I_3 = 0$",
    r"6−3I2−6I3=06 - 3I_2 - 6I_3 = 06−3I2​−6I3​=0": r"$6 - 3I_2 - 6I_3 = 0$",
    r"I1=3I_1 = 3 I1​=3": r"$I_1 = 3$",
    r"I2=0I_2 = 0 I2​=0": r"$I_2 = 0$",
    r"I3=3I_3 = 3 I3​=3": r"$I_3 = 3$",
    r"12−2I1−6I3=0  ⟹  I1=6−3I312 - 2I_1 - 6I_3 = 0 \implies I_1 = 6 - 3I_3 12−2I1​−6I3​=0⟹I1​=6−3I3​": r"$12 - 2I_1 - 6I_3 = 0 \implies I_1 = 6 - 3I_3$",
    r"6−3I2−6I3=0  ⟹  I2=2−2I36 - 3I_2 - 6I_3 = 0 \implies I_2 = 2 - 2I_3 6−3I2​−6I3​=0⟹I2​=2−2I3​": r"$6 - 3I_2 - 6I_3 = 0 \implies I_2 = 2 - 2I_3$",
    r"(6−3I3)+(2−2I3)=I3  ⟹  8=6I3  ⟹  I3=43(6-3I_3) + (2-2I_3) = I_3 \implies 8 = 6I_3 \implies I_3 = \frac{4}{3} (6−3I3​)+(2−2I3​)=I3​⟹8=6I3​⟹I3​=34​": r"$(6-3I_3) + (2-2I_3) = I_3 \implies 8 = 6I_3 \implies I_3 = \frac{4}{3}$",
    r"I1=6−4=2I_1 = 6 - 4 = 2 I1​=6−4=2": r"$I_1 = 6 - 4 = 2$",
    r"I2=2−83=−23I_2 = 2 - \frac{8}{3} = -\frac{2}{3} I2​=2−38​=−32​": r"$I_2 = 2 - \frac{8}{3} = -\frac{2}{3}$",
    r"I3=43I_3 = \frac{4}{3} I3​=34​": r"$I_3 = \frac{4}{3}$",
    r"I2I_2 I2​": r"$I_2$",
    r"Ia−IbI_a - I_b Ia​−Ib​": r"$I_a - I_b$",
    r"RkkIk−∑j≠kRkjIj=∑UskR_{kk} I_k - \sum_{j \neq k} R_{kj} I_j = \sum U_{sk}Rkk​Ik​−j=k∑​Rkj​Ij​=∑Usk​": r"$R_{kk} I_k - \sum_{j \neq k} R_{kj} I_j = \sum U_{sk}$",
    r"RkkR_{kk} Rkk​": r"$R_{kk}$",
    r"RkjR_{kj} Rkj​": r"$R_{kj}$",
    r"∑Usk\sum U_{sk} ∑Usk​": r"$\sum U_{sk}$",
    r"IaI_a Ia​": r"$I_a$",
    r"IbI_b Ib​": r"$I_b$",
    r"8Ia−6Ib=128 I_a - 6 I_b = 128Ia​−6Ib​=12": r"$8 I_a - 6 I_b = 12$",
    r"−6Ia+9Ib=−6-6 I_a + 9 I_b = -6−6Ia​+9Ib​=−6": r"$-6 I_a + 9 I_b = -6$",
    r"Ia=2I_a = 2 Ia​=2": r"$I_a = 2$",
    r"Ib=23I_b = \frac{2}{3} Ib​=32​": r"$I_b = \frac{2}{3}$",
    r"I1=Ia=2I_1 = I_a = 2 I1​=Ia​=2": r"$I_1 = I_a = 2$",
    r"I2=−Ib=−23I_2 = -I_b = -\frac{2}{3} I2​=−Ib​=−32​": r"$I_2 = -I_b = -\frac{2}{3}$",
    r"I3I_3 I3​": r"$I_3$",
    r"=Ia−Ib=2−23=43= I_a - I_b = 2 - \frac{2}{3} = \frac{4}{3} =Ia​−Ib​=2−32​=34​": r"$= I_a - I_b = 2 - \frac{2}{3} = \frac{4}{3}$",
    r"GkkUk−∑j≠kGkjUj=∑IskG_{kk} U_k - \sum_{j \neq k} G_{kj} U_j = \sum I_{sk}Gkk​Uk​−j=k∑​Gkj​Uj​=∑Isk​": r"$G_{kk} U_k - \sum_{j \neq k} G_{kj} U_j = \sum I_{sk}$",
    r"UkU_k Uk​": r"$U_k$",
    r"GkkG_{kk} Gkk​": r"$G_{kk}$",
    r"GkjG_{kj} Gkj​": r"$G_{kj}$",
    r"−GkjUj-G_{kj}U_j −Gkj​Uj​": r"$-G_{kj} U_j$",
    r"∑Isk\sum I_{sk} ∑Isk​": r"$\sum I_{sk}$",
    r"UaU_a Ua​": r"$U_a$",
    r"12−Ua2\dfrac{12 - U_a}{2} 212−Ua​​": r"$\dfrac{12 - U_a}{2}$",
    r"6−Ua3\dfrac{6 - U_a}{3} 36−Ua​​": r"$\dfrac{6 - U_a}{3}$",
    r"Ua6\dfrac{U_a}{6} 6Ua​​": r"$\dfrac{U_a}{6}$",
    r"12−Ua2+6−Ua3=Ua6\frac{12 - U_a}{2} + \frac{6 - U_a}{3} = \frac{U_a}{6}212−Ua​​+36−Ua​​=6Ua​​": r"$\frac{12 - U_a}{2} + \frac{6 - U_a}{3} = \frac{U_a}{6}$",
    r"3(12−Ua)+2(6−Ua)=Ua3(12-U_a) + 2(6-U_a) = U_a3(12−Ua​)+2(6−Ua​)=Ua​": r"$3(12-U_a) + 2(6-U_a) = U_a$",
    r"36−3Ua+12−2Ua=Ua36 - 3U_a + 12 - 2U_a = U_a36−3Ua​+12−2Ua​=Ua​": r"$36 - 3U_a + 12 - 2U_a = U_a$",
    r"48=6Ua  ⟹  Ua=8\text{ V}48 = 6U_a \implies U_a = 8 \text{ V}48=6Ua​⟹Ua​=8 V": r"$48 = 6U_a \implies U_a = 8 \text{ V}$",
    r"48=6Ua  ⟹  Ua=8 V48 = 6U_a \implies U_a = 8 \text{ V}48=6Ua​⟹Ua​=8 V": r"$48 = 6U_a \implies U_a = 8 \text{ V}$",
    r"us(t)u_s(t) us​(t)": r"$u_s(t)$",
    r"is(t)i_s(t) is​(t)": r"$i_s(t)$",
}

for old, new in lines_to_replace.items():
    text = text.replace(old, new)


# Save backwards
with open('e:/programming_works/Typora/Claude电路.md', 'w', encoding='utf-8') as f:
    f.write(text)
