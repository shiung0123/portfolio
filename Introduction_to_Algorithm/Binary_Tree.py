# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

def cmp_asb(a , b) :
    if len(a) < len(b) : return True
    elif len(a) > len(b) : return False
    else : return a < b


def translate(arr) :
    if len(arr) <= 1 : return arr
    else :
        root = arr[0]
        smaller = [i for i in arr if cmp_asb(i, root)]
        bigger = [i for i in arr if cmp_asb(root, i)]

        return translate(smaller) + translate(bigger) + [root]


def main() :
    while True :
        arr = list(input().split())
        if arr[0] == '0' : break

        arr = translate(arr)

        for i in arr : print(i, end = ' ')
        print()

main()
