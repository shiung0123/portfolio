# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

# 建立DP中小問題的資料節點，存硬幣數量和切割方式
class Data:
    coin_num = 0
    split = 0
    def __init__(self, coin_num, split):
        self.coin_num = coin_num
        self.split = split

def main() :
    case = 1
    while True :
        n = int(input())
        if not n : break

        # 讀取input，並答案陣列，存放每種硬幣應有的數量，初始化為0
        denomination = list(map(int, input().split()))
        ans = [ 0 for i in range(len(denomination)) ]
        
        # DP 從 1 ~ n，以陣列角度是 0 ~ n+1，忽略0
        table = []
        for coin in range(n+1) :
            if coin == 0 : 
                table.append(Data(0, -1))
            else :
                min_coin = 2147483647
                min_split = 0
                # 取得目前金額，可使用的硬幣種類
                able_denomination = [ i for i in denomination if i <= coin ]
                for i in range(len(able_denomination)) :
                    # Min { DP[coin - bj] + 1 } bj = able_denomination[i]
                    if 1 + table[coin - able_denomination[i]].coin_num < min_coin :
                        # 更好的解法，就更新
                        min_coin = 1 + table[coin - able_denomination[i]].coin_num
                        min_split = i
                # 保存下目前金額的最佳解
                table.append(Data(min_coin, min_split))
        
        # DP完畢，反推目前金額為n時所需的硬幣數量
        while n :
            # 紀錄此種硬幣所需的數量
            ans[table[n].split] += 1
            n -= denomination[table[n].split]

        # 印出答案
        print("Case {i}".format(i = case))
        for i in range(len(ans)) :
            if ans[i] : 
                print("{d}元{num}枚".format(d = denomination[i], num = ans[i]))
        
        print()
        case = case + 1

main()

"""
51
1 5 10 50
36
1 5 10 25
8
1 4 6
26
1 5 8 15
0
ANS
1*1 50*1
1*1 10*1 25*1
4*2
1*1 5*2 15*1
"""
