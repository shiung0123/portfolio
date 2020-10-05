# 演算法分析機測
# 學號：10627116/10627123
# 姓名：許逸翔/黃致堯
# 中原大學資訊工程學系

from itertools import combinations

def IsATriangle(a, b, c):
	if (a + b > c) and (a + c > b) and (b + c > a):
		return True
	else:
		return False
	
while True:
	while True:
		try:
			# n = int(input("Choose your n : "))
			n = int(input(""))
		except:
			print("Give me int")
			continue
		if (n >= 3 and n <= 100):
			break
		elif (n == 0):
			# print("Exit mission 2")
			exit(0)
		elif (n < 3):
			print("n too small")
		elif (n > 100):
			print("n too big")

	TriangleNum = 0

	potentialTriangles = list(combinations(range(1,n+1), 3))

	for potentialTriangle in potentialTriangles:
		if IsATriangle(potentialTriangle[0], potentialTriangle[1], potentialTriangle[2]):
			TriangleNum += 1

	print(str(TriangleNum))


