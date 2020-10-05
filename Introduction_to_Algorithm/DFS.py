# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

# 建立Map中的資料節點
# visit判斷是否造訪過，避免重複造訪
# threat判斷此點的周圍是否有危險
# type記錄點的種類
class Node:
    visit = False
    threat = False
    type = -1
    # G = 1, P、normal = 0, # = -1
    # 沒有T是因為讀入時會被直接記錄在threat上

def Walk(data, i, j, num) :
    # 避免重複早訪，也避免是牆壁
    if not data[i][j].visit and data[i][j].type != -1 :
        # 設定造訪過
        data[i][j].visit = True
        # 如果是G，則答案+1
        if data[i][j].type == 1 :
            num += 1
        # 繼續遞迴，但如果感應到threat就不遞迴
        if not data[i][j].threat :
            num = Walk(data, i-1, j, num)
            num = Walk(data, i+1, j, num)
            num = Walk(data, i, j-1, num)
            num = Walk(data, i, j+1, num)
    return num

def main() :
    while True :
        column, row = list(map(int, input().split()))
        if not column : break

        # 建立空的Map
        data = [[Node() for j in range(column)] for i in range(row)]

        start_i = 0
        start_j = 0

        # 讀取input資料到Map
        for i in range(row) :
            str = input()
            for j in range(column) :
                if str[j] == 'P' : 
                    # 紀錄起始點
                    start_i = i
                    start_j = j
                    data[i][j].type = 0
                elif str[j] == '.' :
                     data[i][j].type = 0
                elif str[j] == 'G' :
                    data[i][j].type = 1
                elif str[j] == 'T' :
                    # 若是陷阱，設定周圍四格(上下左右)的threat
                    data[i-1][j].threat = True
                    data[i+1][j].threat = True
                    data[i][j-1].threat = True
                    data[i][j+1].threat = True
        
        # 進行 Greddy 的 DFS
        num = Walk(data, start_i, start_j, 0)

        # 印出答案
        print(num)
        print()

main()

"""
7 4
#######
#P.GTG#
#..TGG#
#######
8 6
########
#...GTG#
#..PG.G#
#...G#G#
#..TG.G#
########
10 9
##########
#.GTGGGGG#
#..TT..TT#
#........#
#........#
######...#
#GGGG..#.#
#GTGG..#P#
##########
0 0
ANS 1 4 6
"""

