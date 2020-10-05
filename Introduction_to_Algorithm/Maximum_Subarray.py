# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

def FindMaxArr(arr, start, end) :
    if start == end : return start, end, arr[start]
    else :
        mid = (start + end) // 2
        left_start, left_end, left_sum = FindMaxArr(arr, start, mid)
        right_start, right_end, right_sum = FindMaxArr(arr, mid+1, end)
        cross_start, cross_end, cross_sum = FindMaxCrossArr(arr, start, mid, end)

        if left_sum > cross_sum and left_sum > right_sum : return left_start, left_end, left_sum
        elif cross_sum > right_sum : return cross_start, cross_end, cross_sum
        else : return right_start, right_end, right_sum

def FindMaxCrossArr(arr, start, mid, end) :
    
    max_start = start
    max_left_sum = -2147483648
    sum = 0
    for i in range(mid, start-1, -1) :
        sum += arr[i]
        if sum > max_left_sum :
            max_left_sum = sum
            max_start = i

    max_end = end
    max_right_sum = -2147483648
    sum = 0
    for i in range(mid+1, end+1, 1) :
        sum += arr[i]
        if sum > max_right_sum :
            max_right_sum = sum
            max_end = i

    return max_start, max_end, max_left_sum + max_right_sum

def main() :
    while True :
        n = int(input())
        if not n : break

        arr = []
        while len(arr) < n :
            arr += list(map(int, input().split()))
    
        start, end, sum = FindMaxArr(arr, 0, len(arr) - 1)
        print("Low = {start}, High = {end}, Sum = {sum}".format(start = start+1, end = end+1, sum = sum))

main()
