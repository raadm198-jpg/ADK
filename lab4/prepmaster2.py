Vertices = 10   # Number of Vertices
Edges = 9    # Number of Edges
M = 2    # Number of Colors
ListOfEdges = [(1,2),(1,3),(1,4),(1,5),(1,6),(1,7),(1,8),(1,9),(1,10)]

Cats = Vertices
RoomNumber = M    #number of colors given in Graph Coloring
Blocks = ListOfEdges

R = []
for i in range(Cats):
    R.append([])
    for j in range(RoomNumber):
        R[i].append(j+1)

RoomCapacity = []
for k in range(RoomNumber):
    RoomCapacity.append(Cats)
print(RoomCapacity)
# Certificate is a array of arrays, where the cats are div
Assignment = [1,2,2,2,2,2,2,2,2,2]

def verify(Assignment, Blocks,R,Cats, RoomCapacity):
    for b in Blocks:
        if Assignment[b[0]-1] == Assignment[b[1]-1]:
            return False
    for cat in range(Cats):
        if not (Assignment[cat] in R[cat]):
            return False
    for c in range(len(RoomCapacity)):
        count = 0
        for i in Assignment:
            if i == c+1:
                count += 1
        if count > RoomCapacity[c]:
            return False
    return True

print(verify(Assignment, Blocks,R,Cats,RoomCapacity))