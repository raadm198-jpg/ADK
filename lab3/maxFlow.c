#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct {
    int* data;
    int frontIndex;
    int rearIndex;
    int size;
    int capacity;
} Queue;

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


void addNeighbor(NeighborList *list, Edge edge) {
    if (list->valence >= list->listSize) {
        int newListSize = list->listSize == 0 ? 4 : list->listSize * 2;

        Edge *temp = realloc(list->neighbors, newListSize * sizeof(Edge));

        list->neighbors = temp;
        list->listSize = newListSize;
    }

    list->neighbors[list->valence++] = edge;
}


readFlowValues readMaxFlow() {
  int v, e, s, t;
  int count = 0;
  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d", &v, &s, &t, &e);



  NeighborList *adjacencyList = calloc(v, sizeof(NeighborList));
  readFlowValues flowValues = {v, s, t, e, adjacencyList};


  //Edge edge = {0, 1, 10, 0, 10};
  //addNeighbor(&adjacencyList[0], edge);

  for (int i = 0; i < e; ++i) {
    int u0, v0, c;
    // Flöde f från u till v
    
    scanf("%d%d%d", &u0, &v0, &c);

    int reverseIndex = adjacencyList[v0-1].valence; // Index of the reverse edge in v0's list
    int forwardIndex = adjacencyList[u0-1].valence; // Index of the forward edge in u0's list

    Edge edge = {u0, v0, c, 0, c, 1, reverseIndex};
    addNeighbor(&adjacencyList[u0-1], edge);

    Edge reverseEdge = {v0, u0, 0, 0, 0, -1, forwardIndex};
    addNeighbor(&adjacencyList[v0-1], reverseEdge);


  }

  return flowValues;
}


//BFS(G, start):
//    queue = [start]
//    visited = {start}

//    while queue is not empty:
//        v = queue.dequeue()
//
//        for each neighbor u of v:
//            if u not in visited:
//                visited.add(u)
//                queue.enqueue(u)


typedef struct {

    int* ReverseNodePath;
    int* ReversePathIndex;
    int distance;

} fakePaths;

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


void writeFlowGraphSolution(NeighborList *arr, int v, int s, int t, int e, int f) {
  
    int count = 0;

    for (int i = 0; i < v; i++) {
        for (int j = 0; j < arr[i].valence; j++) {
            Edge edge = arr[i].neighbors[j];

            if (edge.direction == 1 && edge.flow > 0) {
                count++;
            }
        }
    }
  
  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d %d\n%d\n", v, s, t, f, count);


  for (int i = 0; i < v; ++i) {
    
    for (int j = 0; j < arr[i].valence; ++j) {
      Edge edge = arr[i].neighbors[j];
      if (edge.direction == 1 && edge.flow > 0) printf("%d %d %d\n", edge.u, edge.v, edge.flow);
    }
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  fprintf(stderr, "Skickade iväg flödesgrafen\n");
}

void destroyGraph(NeighborList *adjacencyList, int v) {
    for (int i = 0; i < v; i++) {
        free(adjacencyList[i].neighbors);
    }

    free(adjacencyList);
}

int main(void) {
  
  readFlowValues graph = readMaxFlow();
  
  int maxFlow = edumundK(graph.adjacencyList, graph.v, graph.s, graph.t);

  writeFlowGraphSolution(graph.adjacencyList, graph.v, graph.s, graph.t, graph.e, maxFlow);

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  destroyGraph(graph.adjacencyList, graph.v);
  return 0;
}