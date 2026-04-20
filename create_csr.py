import numpy as np
import sys
d=open(sys.argv[1],'r').readlines()
a=[]
cols=[]
vals=[]
row_ptr=[0]
for l in d:
    row=[int(a) for a in l.split()]
    for c in row:
      cols.append(c)
      vals.append(np.random.rand())
    row_ptr.append(len(row)+row_ptr[-1])	
with open("csr.mat",'w') as fp:
  fp.write('%d %d\n'%(len(row_ptr)-1,len(vals)))
  fp.write(" ".join(str(i) for i in row_ptr))
  fp.write("\n")
  fp.write(" ".join(str(i) for i in cols))
  fp.write("\n")
  fp.write(" ".join(str(i) for i in vals))
