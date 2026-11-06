#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int V, E, M;
    scanf("%d%d%d", &V, &E, &M);

    int x, y;
    int *scenes = malloc(sizeof(int)*(2*(E+V+1)));
    int *valence = malloc(sizeof(int)*V);
    
    for (int i = 0; i < V; i++) {
        valence[i] = 0;
    }

    for (int i = 0; i < E; i++){
        scanf("%d%d", &x, &y);
        scenes[2*i] = x;
        scenes[2*i+1] = y;
    }

    for (int i = 0; i < V; i++) {
        scenes[2*(E+i)] = i+1;
        scenes[2*(E+i)+1] = V+1;
    }

    scenes[2*(E+V)] = V+2;
    scenes[2*(E+V)+1] = 1;

    int R = V+2;
    int S = E + V + 1;
    int P = M+2;

    printf("%d\n%d\n%d\n", R, S, P);

    for (int i = 0; i < V; i++) {
        printf("%d", M);
        for (int j = 3; j <= M+2; j++) {
            printf(" %d", j);
        }
        printf("\n");
    }

    printf("1 1\n");
    printf("1 2\n");

    for (int i = 0; i < S; i++) {

        int x, y;
        x = scenes[2*i];
        y = scenes[2*i+1];

        printf("%d %d %d\n", 2, x, y);

    }

    return 0;

}