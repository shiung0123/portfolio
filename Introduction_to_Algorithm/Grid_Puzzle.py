# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

def min_cost(arr) :
    # 0 1 2
    # 3 4 5
    # 6 7 8
    ans = []
    # (3 + 6 + 7) * 2 + 1 + 4 + 5 + 8888
    ans.append( ( arr[3] + arr[6] + arr[7] ) * 2 + arr[1] + arr[4] + arr[5] + 4 )
    # (3 + 4 + 7) * 2 + 1 + 5 + 6 + 8888
    ans.append( ( arr[3] + arr[4] + arr[7] ) * 2 + arr[1] + arr[5] + arr[6] + 4 )
    # (1 + 4 + 7) * 2 + 3 + 5 + 6 + 8888
    ans.append( ( arr[1] + arr[4] + arr[7] ) * 2 + arr[3] + arr[5] + arr[6] + 4 )
    # (1 + 2 + 5) * 2 + 3 + 4 + 7 + 8888
    ans.append( ( arr[1] + arr[2] + arr[5] ) * 2 + arr[3] + arr[4] + arr[7] + 4 )
    # (1 + 4 + 5) * 2 + 2 + 3 + 7 + 8888
    ans.append( ( arr[1] + arr[4] + arr[5] ) * 2 + arr[2] + arr[3] + arr[7] + 4 )
    # (3 + 4 + 5) * 2 + 1 + 2 + 7 + 8888
    ans.append( ( arr[3] + arr[4] + arr[5] ) * 2 + arr[1] + arr[2] + arr[7] + 4 )

    return min(ans)

def main() :

    arr = []
    while len(arr) < 9 :
        arr += list(map(int, input().split()))
    
    ans = min_cost(arr)
    print("Minimum Sum of Costs = {ans}".format(ans = ans))

main()
