import matplotlib.pyplot as plt
import sys
f = open ( 'task.txt' , 'r')
grid = [ map(int,line.split(',')) for line in f ]

class Stack:
     def __init__(self):
         self.items = []

     def isEmpty(self):
         return self.items == []

     def push(self, item):
         self.items.append(item)

     def pop(self):
         return self.items.pop()

     def peek(self):
         return self.items[len(self.items)-1]

     def size(self):
         return len(self.items)
        
sx=1
sy=1
source=grid[sx][sy]
dx=17
dy=17
destination=grid[dx][dy]

decx=Stack()
decy=Stack()

pathx=Stack()
pathy=Stack()

cx=sx
cy=sy

def d_to_destination(vx,vy):
    d=abs(dx-vx)+abs(dy-vy)
    return d
def d_from_source(vx,vy):
    d=abs(vx-sx)+abs(vy-sy)
    return d
def dead_end():
    global cx,cy
    if (decx.size()==0):
        sys.exit("There is no path to the destination")
    else:
        









plt.imshow(grid)
plt.imshow(grid, cmap='Greys',  interpolation='nearest')
plt.show()