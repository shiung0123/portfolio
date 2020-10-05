# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

class node:
    def __init__(self,value):
         # 初始化節點
         self.value = value
         # 一個節點可以連向多個節點，所以可以有很多鄰居
         self.neighbors = []

edgeList = []
mainTour = []

global vertexNum
global edgeNum

# 這個函式會收到兩個應相連的節點，那兩個節點會被稱作num_1和num_2
def AddRelationship(num_1 ,num_2):
    # 在圖中搜尋num_1是不是已存在
    for i in range(len(edgeList)):
        if edgeList[i].value == num_1:
            # 在圖中搜尋num_2是不是已存在
            for j in range(len(edgeList)):
                # 若已存在，就讓num_2和num_1互相成為鄰居
                if edgeList[j].value == num_2:
                    edgeList[i].neighbors.append(edgeList[j])
                    edgeList[j].neighbors.append(edgeList[i])
                    break
            # 若不存在，就先新增節點num_2，再讓num_2和num_1互相成為鄰居
            else:
                edgeList.append(node(num_2))
                edgeList[-1].neighbors.append(edgeList[i])
                edgeList[i].neighbors.append(edgeList[-1])
            break
    # 若不存在，就新增那兩個節點，並讓他們成為對方的鄰居
    else:
        edgeList.append(node(num_1))
        edgeList.append(node(num_2))
        edgeList[-1].neighbors.append(edgeList[-2])
        edgeList[-2].neighbors.append(edgeList[-1])

    return

def ReadInput():
    global vertexNum
    global edgeNum

    # 讀取節點數以及邊數
    temp = input().split()
    vertexNum = int(temp[0])
    edgeNum = int(temp[1])

    # 如果得到的結果是 0 0，就會 return False
    if vertexNum == 0 and edgeNum == 0:
        return False

    # 依據剛剛讀到的邊數，讀取兩兩相連的節點
    for i in range(edgeNum):
        temp = input().split()
        # 並將這份連結加入到圖中
        AddRelationship(int(temp[0]),int(temp[1]))


    return True

def EulerTourExist():
    # 遍尋每個節點
    for i in range(len(edgeList)):
        # 只要有一個節點的鄰居數不是偶數，Euler Tour就不存在
        if len(edgeList[i].neighbors) %  2 != 0:
            return False
    # 反之如果都是偶數，就存在
    return True

def FindEulerTour():
    global mainTour

    for i in range(len(edgeList)):
        # 從圖中的隨便一個點開始走訪 (這裡是從[0]開始)
        temp = edgeList[i]
        # 如果他還有鄰居，就往他的鄰居走
        if len(temp.neighbors) > 0:
            # start = True 代表剛開始走
            start = True
            subTour = []
            subTour.append(temp.value)
            # 只要還沒走回原點，就繼續走訪，總有一天會走回去
            # 目前在原點的話，有兩種情形，一種是還沒出發，一種是已經一圈繞回來
            # 因此才要用 start 來排除正準備出發的情形
            while edgeList[i] != temp or start:
                start = False
                # 記錄目前的節點
                prev_temp = temp
                # 並移到下一個節點
                temp = temp.neighbors[0]
                # 記錄這個移動的過程
                subTour.append(temp.value)
                # 並且過河拆橋，移除經過的邊
                prev_temp.neighbors.pop(0)
                temp.neighbors.remove(prev_temp)
                
            # 繞完了一圈之後
            # 如果目前都還沒有Tour的記錄，就以這圈的過程為主
            if len(mainTour) == 0:
                mainTour = subTour.copy()
            # 如果已經有記錄了，就插入這圈的記錄他們之中
            else:
                index = mainTour.index(subTour[0])
                for j in range(len(subTour) - 1):
                    mainTour.insert(index, subTour[-2-j])


    return

def ShowEulerTour():
    
    print("< ", end='')

    # 就只是輸出剛剛記錄的串列內容而已
    for i in range(len(mainTour)):
        print(mainTour[i], end='')
        # 用來判斷要不要加逗號
        if i < len(mainTour) - 1:
            print(", ", end='')
            
    print(" >\n\n", end='')
        
    return

# 第幾回合，給使用者看的，所以從 1 開始數
round = 1
# 讀取 Input，如果得到的結果是 0 0，就會跳離迴圈，結束程式
while ReadInput():
    # 印出現在是第幾回合
    print("\nCase ", round)
    
    if EulerTourExist():
        FindEulerTour()
        ShowEulerTour()
    else:
        print("No Euler Tours\n")

    # 回合數加 1
    round += 1

    # 清除剛剛的資料，準備下一次的計算
    edgeList.clear()
    mainTour.clear()
    
"""
3 3
1 2
1 3
2 3
6 10
1 2
1 3
2 3
2 4
2 5
3 4
3 5
4 5
4 6
5 6
4 5
1 2
1 3
1 4
2 4
3 4
6 11
1 2
1 3
1 4
1 5
2 3
2 4
2 5
3 4
3 6
4 5
5 6
6 12
1 2
1 3
1 4
1 5
2 3
2 4
2 5
3 4
3 6
4 5
4 6
5 6
0 0
ANS
< 1, 2, 3, 1 >
< 1, 2, 4, 5, 6, 4, 3, 5, 2, 3, 1 >
No Euler Tours
< 1, 2, 4, 1, 5, 2, 3, 4, 5, 6, 3, 1 >
No Euler Tours
"""