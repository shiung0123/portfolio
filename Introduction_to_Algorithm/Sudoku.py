# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

import time

# ds start
class Box:
    def __init__(self):
        self.value = 0
        self.isSet = False
        self.options = [i for i in range(1,10)]

table = [[Box for y in range(9)] for x in range(9)]
# ds end

def NextBox(y, x):
    if x == 8:
        x = 0
        y += 1
    else:
        x += 1
    return y, x

def Ended(y, x):
    if x == 8 and y == 8: # If ended
        return True
    else:
        return False

def NotPossible(y, x, option):
    # This option cant be in that row, column, or square
    for i in range(9):
        if table[y][i].value == option:
            return True
        elif table[i][x].value == option:
            return True

    a = (x//3) * 3 # StartingPointOfThatBlock_x
    b = (y//3) * 3 # StartingPointOfThatBlock_y
    for i in range(3):
        for j in range(3):
            if table[b+i][a+j].value == option:
                return True

    return False

def Try(y, x): # Remove not possible options
    # Check possible options for each boxes
    while table[y][x].isSet: # If this box is not set
        if Ended(y, x): # If ended
            return True
        y, x = NextBox(y, x)

    for option in table[y][x].options: # Check each option
        if NotPossible(y, x, option): # If this option is not possible
            # Then skip this option
            continue
        else: # If possible, than try it
            if Ended(y, x): # If ended
                table[y][x].value = option
                return True
            else:
                # Store now option to now box
                table[y][x].value = option
                # Then test it
                a, b = NextBox(y, x)
                if Try(a, b) == True:
                    return True
                else: # If this way is not possible
                    # Cancel now value
                    table[y][x].value = 0
                    continue # And then skip this option

    else: # If all options not possible
        return False

# main start
# Read
for i in range(9):
    temp_str = input()
    for j in range(9):
        table[i][j] = Box()
        table[i][j].value = int(temp_str[j])
        if temp_str[j] != '0':
            table[i][j].isSet = True

# start_time = time.time()
Try(0, 0)
# end_time = time.time()

print("")
for i in range(9):
    for j in range(9):
        print(table[i][j].value, end='')
    print("")

# print("")
# print('Time : ', end_time - start_time)
# print("")
# main end