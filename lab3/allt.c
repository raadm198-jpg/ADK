#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//nödvändig struct
typedef struct {
    int* data;
    int frontIndex;
    int rearIndex;
    int size;
    int capacity;
} Queue;

typedef struct {
    int u;
    int v;
    int capacity;
    int flow;
    int residual;
    int direction; // 1 for forward edge, -1 for backward edge
    int reverseIndex; // Index of the reverse edge in the adjacency list
} Edge;

typedef struct {
    Edge *neighbors;
    int valence;
    int listSize;
} NeighborList;

typedef struct {
    int v;
    int s;
    int t;
    int e;
    NeighborList *adjacencyList;

} readFlowValues;

typedef struct {

    int* ReverseNodePath;
    int* ReversePathIndex;
    int distance;

} fakePaths;


int *readBipartiteGraph() {
  int x, y, e, a, b;
  int count = 0;
  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);
  int *allt = malloc(sizeof(int)*(3*(e+x+y) + 3));
  allt[count] = x;
  count++;
  allt[count] = y;
  count++;
  allt[count] = e+x+y;
  count++;

  for (int i = 0; i < e; ++i) {
    scanf("%d %d", &a, &b);
    allt[count] = a;
    count++;
    allt[count]= b;
    count++;
    allt[count] = 1; // Assuming capacity is 1 for each edge
    count++;
  }
  int s = x + y + 1; // Source node index
  int t = x + y + 2; // Sink node index
  for (int i = 0; i < x; ++i) {
    allt[count] = s; // Assuming capacity is 0 for edges from source to left partition
    count++;
    allt[count] = i + 1; // Left partition node index
    count++;
    allt[count] = 1; // Assuming capacity is 1 for edges from source to left partition
    count++;
  }
  for (int i = 0; i < y; ++i) {
    allt[count] = i + x + 1; // Right partition node index
    count++;
    allt[count] = t; // Assuming capacity is 0 for edges from right partition to sink
    count++;
    allt[count] = 1; // Assuming capacity is 1 for edges from right partition to sink
    count++;
  }
  return allt;
}


void init(Queue *q, int v) {
    q->frontIndex = 0;
    q->rearIndex = 0;
    q->size = 0;
    q->data = malloc(v * sizeof(int));
    q->capacity = v;
}

void push(Queue *q, int value) {
    q->data[q->rearIndex] = value;
    q->rearIndex = (q->rearIndex + 1) % q->capacity;
    q->size++;
}

void pop(Queue *q) {
    q->frontIndex = (q->frontIndex + 1) % q->capacity;
    q->size--;
}

int front(Queue *q) {
    return q->data[q->frontIndex];
}

int end(Queue *q) {
    int index = (q->rearIndex - 1 + q->capacity) % q->capacity;
    return q->data[index];
}

void destroy(Queue *q) {
    free(q->data);
}

void addNeighbor(NeighborList *list, Edge edge) {
    if (list->valence >= list->listSize) {
        int newListSize = list->listSize == 0 ? 4 : list->listSize * 2;

        Edge *temp = realloc(list->neighbors, newListSize * sizeof(Edge));

        list->neighbors = temp;
        list->listSize = newListSize;
    }

    list->neighbors[list->valence++] = edge;
}


NeighborList *listMaker(int *graph, int v, int e) {

  NeighborList *adjacencyList = calloc(v, sizeof(NeighborList));
  for (int i = 3; i < 3*e+3; i += 3) {
    int u0 = graph[i], v0 = graph[i+1], c = graph[i+2];
    // Flöde f från u till v

    int reverseIndex = adjacencyList[v0-1].valence; // Index of the reverse edge in v0's list
    int forwardIndex = adjacencyList[u0-1].valence; // Index of the forward edge in u0's list

    Edge edge = {u0, v0, c, 0, c, 1, reverseIndex};
    addNeighbor(&adjacencyList[u0-1], edge);

    Edge reverseEdge = {v0, u0, 0, 0, 0, -1, forwardIndex};
    addNeighbor(&adjacencyList[v0-1], reverseEdge);

  }
  return adjacencyList;
}


fakePaths BFS(NeighborList *adjacencyList, int v, int s, int t) {

    fakePaths paths;

    int* distance = calloc(v, sizeof(int));
    int* ReverseNodePath = calloc(v, sizeof(int));
    int* ReversePathIndex = calloc(v, sizeof(int));
    
    for (int i = 0; i < v; i++) {
        distance[i] = -1; // Mark all vertices as unvisited
        //ReverseNodePath[i] = -1; // Initialize ReverseNodePath
    }

    Queue q;
    init(&q, v);

    distance[s-1] = 0; // Distance to source is 0
    push(&q, s-1); // Enqueue source vertex

    while (q.size > 0) {
        int current = front(&q);
        pop(&q);

        for (int i = 0; i < adjacencyList[current].valence; i++) {
            Edge edge = adjacencyList[current].neighbors[i];
            int neighbor = edge.v - 1;

            // If the neighbor hasn't been visited and there's residual capacity
            if (distance[neighbor] == -1 && edge.residual > 0) {
                distance[neighbor] = distance[current] + 1;
                ReverseNodePath[neighbor] = current; // Store the ReverseNodePath
                ReversePathIndex[neighbor] = i; // Store the index of the edge in the adjacency list
                push(&q, neighbor);

                // If we reached the sink, we can stop early
                if (neighbor == t-1) {
                    paths.ReverseNodePath = ReverseNodePath;
                    paths.ReversePathIndex = ReversePathIndex;
                    paths.distance = distance[t-1];

                    free(distance);
                    destroy(&q);

                    return paths;
                }
            }
        }
    }

    // No ReverseNodePath to sink was found
    if (distance[t-1] == -1) {
        free(ReverseNodePath);
        free(ReversePathIndex);

        ReverseNodePath = NULL;
        ReversePathIndex = NULL;
    }


    destroy(&q);

    paths.ReverseNodePath = ReverseNodePath;
    paths.ReversePathIndex = ReversePathIndex;
    paths.distance = distance[t-1];
    free(distance);

    return paths;
}

int minst(int a, int b) {
    return (a < b) ? a : b;
}

int edumundK (NeighborList *adjacencyList, int v, int s, int t) {
    int f = 0;

    while (1) {
        fakePaths path = BFS(adjacencyList, v, s, t);

        if (path.ReverseNodePath == NULL) break;

        int bottleneck = INT_MAX;
        int current = t - 1;
        int residual;

        while (current != s - 1) {
            int parent = path.ReverseNodePath[current];
            int edgeIndex = path.ReversePathIndex[current];

            residual = adjacencyList[parent].neighbors[edgeIndex].residual;

            bottleneck = minst(bottleneck, residual);

            current = parent;
        }

        current = t - 1;

        while (current != s - 1) {
            int parent = path.ReverseNodePath[current];
            int edgeIndex = path.ReversePathIndex[current];
            int reverseEdgeIndex = adjacencyList[parent].neighbors[edgeIndex].reverseIndex;

            Edge *edge = &adjacencyList[parent].neighbors[edgeIndex];
            Edge *reverseEdge = &adjacencyList[current].neighbors[reverseEdgeIndex];

            edge->flow += bottleneck;
            reverseEdge->flow = -edge->flow;

            edge->residual = edge->capacity - edge->flow;
            reverseEdge->residual = reverseEdge->capacity - reverseEdge->flow;

            current = parent;
        }

        f += bottleneck;
        free(path.ReverseNodePath);
        free(path.ReversePathIndex);
    }

    return f;
}

void destroyGraph(NeighborList *adjacencyList, int v) {
    for (int i = 0; i < v; i++) {
        free(adjacencyList[i].neighbors);
    }

    free(adjacencyList);
}

int *resultMaxFlow(NeighborList *adjacencyList, int maxFlow, int x, int y,int s, int t) {
  int *result = malloc(sizeof(int) * (2 * maxFlow));
  int count = 0;

  for (int i = 0; i < x; ++i) {
      for (int j = 0; j < adjacencyList[i].valence; ++j) {
          Edge edge = adjacencyList[i].neighbors[j];
          if (edge.flow > 0 && edge.v != s && edge.v != t) {
              result[count++] = edge.u;
              result[count++] = edge.v;
          }
      }
  }

  destroyGraph(adjacencyList, x + y + 2);
  
  return result;
}






void writeBipMatchSolution(int x, int y, int *arr, int maxMatch) {

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", x, y, maxMatch);

  for (int i = 0; i < maxMatch; ++i) {
    int a, b;
    a = arr[2*i];
    b = arr[2*i + 1];
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", a, b);
  }

}

int main(void) {
  
  int *graph = readBipartiteGraph();
  int x = graph[0], y= graph[1], e = graph[2];
  int v = x + y + 2;
  int s = x + y + 1;
  int t = x + y + 2;

  NeighborList *adjacencyList = listMaker(graph, v, e);
  
  int maxFlow = edumundK(adjacencyList, v, s, t);
  
  int *flowGraph = resultMaxFlow(adjacencyList, maxFlow, x, y, s, t);

  writeBipMatchSolution(x, y, flowGraph, maxFlow);

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  free(graph);
  free(flowGraph);
  return 0;
}
