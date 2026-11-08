#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int V, E, M;
    scanf("%d%d%d", &V, &E, &M);
    if (M > V) M = V;

    int x, y;
    int *scenes = malloc(sizeof(int) * 2 * (E + V + 2));
    int *valence = calloc(V, sizeof(int));

    for (int i = 0; i < E; i++){
        scanf("%d%d", &x, &y);
        scenes[2*i] = x;
        scenes[2*i+1] = y;
    }

    for (int i = 0; i < E; i++) {
        valence[scenes[2*i] - 1]++;
        valence[scenes[2*i+1] - 1]++;
    }

    int count = E;

    for (int i = 0; i < V; i++) {
        if (valence[i] == 0) {
            scenes[2*count] = i + 1;
            scenes[2*count+1] = V + 1;
            count++;
        }
    }

    scenes[2*count] = 1;
    scenes[2*count+1] = V + 1;
    count++;

    scenes[2*count] = 1;
    scenes[2*count+1] = V + 2;
    count++;

    int R = V+2;
    int S = count;
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