# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

import math

global numOfData
global targets
targets = []
global targets_copy
targets_copy = []
global binary
global resultList
resultList = []

class treeNode:
    def __init__(self,text=None,freq=None):
         # 初始化節點
         self.text = text
         self.freq = freq
         self.left = None
         self.right = None
    

def ReadInput():
    global binary
    global numOfData

    # 一開始先讀取資料筆數
    while numOfData > 0:
        # 讀取資料內容並存到全域變數
        target = []
        temp = input().split()
        target.append(temp[0])
        target.append(int(temp[1]))
        targets.append(target)
        numOfData -= 1

    # 這是要解碼的那個 0101 串
    binary = str(input())
    return

def BuildTree():
    global targets_copy
    # 複製一份全域List到，建樹的過程都在操作這個複製起來的，以避免動到原始的資料
    for i in range(len(targets)):
        targets_copy.append(treeNode(targets[i][0],targets[i][1]))

    # 在合併成一顆樹以前，都不斷合併
    while len(targets_copy) > 1:
        # 找出頻率最低的，同時記錄他的頻率是多少，以及他在List中的位置
        minFreq_1 = math.inf
        index_1 = 0
        temp_1 = targets_copy[0]
        for i in range(len(targets_copy)):
            if targets_copy[i].freq < minFreq_1:
                minFreq_1 = targets_copy[i].freq
                temp_1 = targets_copy[i]
                index_1 = i
                
        # 找出頻率次低的，同時記錄他的頻率是多少，以及他在List中的位置
        minFreq_2 = math.inf
        index_2 = 0
        temp_2 = targets_copy[0]
        for i in range(len(targets_copy)):
            if targets_copy[i].freq < minFreq_2 and targets_copy[i] != temp_1:
                minFreq_2 = targets_copy[i].freq
                temp_2 = targets_copy[i]
                index_2 = i

        # 新增一個節點，把剛剛那兩個節點當成他的左右子樹
        temp_3 = treeNode()
        temp_3.left = temp_1
        temp_3.right = temp_2
        # 這個新的節點的頻率就是底下兩個節點的頻率相加
        temp_3.freq = temp_1.freq + temp_2.freq
        # 並將此樹新增到List中
        targets_copy.append(temp_3)
        # 再將剛剛那兩顆樹的本體清理乾淨
        if index_1 > index_2:
            del targets_copy[index_1]
            del targets_copy[index_2]
        else:
            del targets_copy[index_2]
            del targets_copy[index_1]


    return

def DFS(start, code):
    # 如果這個位置沒有節點
    if start is None:
        # 返回上一個
        return
    if start.text != None:
        # code會用0和1來代表左或右，並將結果記錄到 resultList 中
        temp = [start.text,code]
        resultList.append(temp)

    # 分別以目前所找到的節點作為下一次 DFS 的起點
    DFS(start.left, code + str(0))
    # 分別以目前所找到的節點作為下一次 DFS 的起點
    DFS(start.right, code + str(1))

def ListAllResult():

    # 走訪霍夫曼樹，DFS會將走訪的過程記錄至全域變數
    DFS(targets_copy[0], "")

    # 接著將每個內容(字元)的走訪過程輸出
    for i in range(len(targets)):
        targetChar = targets[i][0]
        j = 0
        while j < len(resultList):
            if resultList[j][0] == targetChar:
                print(targetChar, " ", resultList[j][1])
                del resultList[j]
            else:
                j += 1

    return

def Decode():
    global binary

    print("Decode = ", end="")

    # 如果待走訪列表中還有東西，就要繼續走訪並印出結果
    while len(binary) > 0:
        temp = targets_copy[0]
        # 只有樹葉節點會有 text，所以用這樣來判斷走到樹葉了沒
        while temp.text == None:
            # 並根據0或1，選擇往左還是往右走
            if binary[0] == "0":
                # 0 就往左
                temp = temp.left
                # 並同時消去待走訪列表中已走訪的部分
                binary = binary[1:]
            elif binary[0] == "1":
                # 1 就往右
                temp = temp.right
                # 並同時消去待走訪列表中已走訪的部分
                binary = binary[1:]
        # 走到樹葉後就印出結果
        print(temp.text, end="")

    print("\n")
    return

# 一開始先讀取資料筆數
numOfData = int(input())
# 這是第幾回合，因為是要給使用者看的，所以從一開始數
num = 1
# 如果還有資料要讀，就繼續
while numOfData != 0:
    # 讀 Input
    ReadInput()
    # 用該 Input 建成一顆霍夫曼樹
    BuildTree()
    # 印出這是第幾回合
    print("\nHuffman Codes #" + str(num))
    # 走訪霍夫曼樹，紀錄走左邊還是走右邊，以判斷0或1，並在走到樹葉時輸出結果
    ListAllResult()
    # 走訪霍夫曼樹，並根據0或1，選擇往左還是往右走，並走到樹葉時輸出該節點的內容(字元)
    Decode()
    # 清除資料，準備下一次的計算與儲存
    targets = []
    targets_copy = []
    resultList = []
    # 看看下個回合有幾筆資料要讀
    numOfData = int(input())
    # 回合數+1
    num += 1

# input()

"""
6
a 45
b 13
c 12
d 16
e 9
f 5
01001101
6
A 2
B 6
C 15
D 12
E 8
F 3
010101001100
7
t 3
o 4
_ 5
b 2
e 2
r 1
n 1
11100100100111000110010110100111101110010010011
0
ANS
Huffman Codes #1
a   0
b   101
c   100
d   111
e   1101
f   1100
Decode = ace
Huffman Codes #2
A   0100
B   011
C   11
D   10
E   00
F   0101
Decode = FACE
Huffman Codes #3
t   111
o   00
_   10
b   010
e   011
r   1100
n   1101
Decode = to_be_or_not_to_be
"""