# 10627116 許逸翔 資訊二甲

# 排列組合
def permutation(numList, pickList, limit):
    global count

    if (len(pickList) == limit):
        print("[", count, "]:", end = '')
        for i in range(0, len(pickList)):
            print(pickList[i], ' ', sep ='', end = '')
        print()
        count = count + 1

    for n in range(0, len(numList)):
        num = numList.pop(0)
        pickList.append(num)
        permutation(numList, pickList, limit)
        numList.append(num)
        pickList.pop(len(pickList)-1);

# 任務一
def task1(numList,pickList):

    while True:
        try:
            limit = int(input("input:"))
        except ValueError:
            print("Error: not a postive number or too large!")
            continue
        if (limit < 1 or limit > 2001):
            print("Error: not a postive number or too large!")
            continue
        break

    for i in range(1, limit+1):
        numList.append(i)
    permutation(numList, pickList, limit)
    print( "L = ", limit+1)

# 任務二
def task2(numList,pickList):

    while True:
        try:
            limit = int(input("input:"))
        except ValueError:
            print("Error: not a postive number or too large!")
            continue
        if (limit < 2 or limit > 9):
            print("Error: not a postive number or too large!")
            continue

        for i in range(1, limit+1):
            while True:
                try:
                    temp =int(input())
                except ValueError:
                    print("Error: not a difference number!")
                    continue
                if (temp < 0 or temp > 2147483647 or temp in numList):
                    print("Error: not a difference number!")
                    continue
                numList.append(temp)
                break

        break

    tStart = time.time()
    permutation(numList, pickList, limit)
    tEnd = time.time()
    print( "T = ", '{:3f}'.format((tEnd - tStart)*1000), 'ms')

# 任務三
def task3(numList,pickList):

    while True:
        try:
            limit = int(input("input:"))
        except ValueError:
            print("Error: not a postive number or too large!")
            continue
        if (limit < 1 or limit > 9):
            print("Error: not a postive number or too large!")
            continue
        break

    for i in range(1, 10):
        numList.append(i)
    tStart = time.time()
    permutation(numList, pickList, limit)
    tEnd = time.time()
    print( "T = ", '{:3f}'.format((tEnd - tStart)*1000), 'ms')

# MAIN
def main():
    global count
    global mode

    while True:
        
        print("           MENU           ")
        print("* 0. Quit                *")
        print("* 1. N numbers from 1..N *")
        print("* 2. M numbers from input*")
        print("* 3. M numbers from 1..9 *")

        while True:
            try:
                mode = int(input("choice:"))
            except ValueError:
                print("Error: Command not found!")
                continue
            if(mode < 0 or mode > 3):
                print("Error: Command not found!")
                continue
            break

        count = 1
        numList = []
        pickList = []

        if (mode == 0):
            return
        elif (mode == 1):
            task1(numList,pickList)
        elif (mode == 2):
            task2(numList,pickList)
        elif (mode == 3):
            task3(numList,pickList)

        print()

# Program Beginning
import time
count = -1
mode = -1
print("\n Progame Warning : Don't input more than one integer in one line.\n")
main()


