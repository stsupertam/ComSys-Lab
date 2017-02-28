import time

with open('mat10000.dat') as f:
    polyShape = []
    for line in f:
        line = line.split() # to deal with blank 
        if line:            # lines (ie skip them)
            line = [int(i) for i in line]
            polyShape.append(line)
del polyShape[0]

start = time.time()

def matmult(a,b):
    zip_b = zip(*b)
    return [[sum(ele_a*ele_b for ele_a, ele_b in zip(row_a, col_b)) 
             for col_b in zip_b] for row_a in a]

#print matmult(a,a)
matmult(polyShape,polyShape)

end = time.time()
print(end - start)

