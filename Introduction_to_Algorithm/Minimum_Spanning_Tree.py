# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系
import numpy as np
import heapq as hq

def main() :
    case = 1
    while True :
        n, line = list(map(int, input().split()))
        if not n : break
        
        # 建立n*n的二維陣列，作為圖的資料結構
        # 初始化維-1，若相連則設為w
        dataset = np.full((n,n), -1)
        for i in range(line) :
            a, b, w = list(map(int, input().split()))
            dataset[a-1][b-1] = dataset[b-1][a-1] = w
        
        # 使用方法Kruskal's
        # h 作為 priority queue
        # ans 即為 MST 之和
        # vertex 紀錄此點是否走訪過，用來取代 Disjoinset
        h = []
        ans = 0
        vertex = np.zeros(n, dtype=int)

        # 從第0的點開始
        vertex[0] = 1
        for i in range(n) :
            if dataset[0][i] != -1 :
                # 把此點所有可行Edge加入priority queue
                hq.heappush( h, (dataset[0][i], i) )

        # 持續直到所有點都走訪過
        while ( not np.all(vertex) ) :
            # 取出當前最小的Edge
            cost, cur = hq.heappop(h)
            # 判斷有無走過，確保不會情成cycle
            if not vertex[cur] :
                # 記錄此點以造訪 
                vertex[cur] = 1
                # 加入MST cost
                ans += cost
                for i in range(n) :
                    if not vertex[i] and dataset[cur][i] != -1 :
                        # 把此點所有可行Edge加入priority queue
                        hq.heappush( h, (dataset[cur][i], i) )

        # 印出答案
        print("Case {i}\nMinimum Cost = {ans}".format(i = case, ans = ans))

        print()
        case += 1

main()

"""
4 4
1 2 10
1 3 8
2 4 5
3 4 2
5 7
1 2 2
1 4 10
1 5 6
2 3 5
2 5 9
3 5 8
4 5 12
9 14
1 2 4
2 3 8
3 4 7
4 5 9
5 6 10
6 7 2
7 8 1
8 1 8
2 8 11
3 9 2
9 8 7
9 7 6
3 6 4
4 6 14
0 0
ANS 15 23 37
"""
