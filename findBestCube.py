import sys
def findBestCube(n,p):
    if p==1: return [n]
    side=0
    for i in range(1,int (round(n**(1/p),5))+1):
        if (n%i==0):
            side=i
    n = n//side
    sides = []
    sides.append(side)
    for i in findBestCube(n, p-1):
        sides.append(i)
    return sides

print(findBestCube(int(sys.argv[1]), int(sys.argv[2])))
