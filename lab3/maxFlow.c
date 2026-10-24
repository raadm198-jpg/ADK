#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *neighbors;
    int valence;
    int capacity;
} NeighborList;

void addNeighbor(NeighborList *list, int neighbor) {
    if (list->valence >= list->capacity) {
        int newCapacity = list->capacity == 0 ? 4 : list->capacity * 2;
        int *temp = realloc(list->neighbors, newCapacity * sizeof(int));

    list->neighbors = temp;
    list->capacity = newCapacity;
    }

    list->neighbors[list->valence++] = neighbor;
}

int *readMaxFlow() {
  int v, e, s, t;
  int count = 0;
  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d%d", &v, &s, &t, &e);

  NeighborList *adjacencyList = calloc(v, sizeof(NeighborList));


  int *capacityM = malloc(sizeof(int)*(v));
  int *flowM = malloc(sizeof(int)*(v));
  int *restM = malloc(sizeof(int)*(v));

  for (int i = 0; i < evenmore[4]; ++i) {
    int u, v0, c;
    // Flöde f från u till v
    
    scanf("%d%d%d", &u, &v0, &c);


  }

  evenmore[4] = e;

  return evenmore;
}