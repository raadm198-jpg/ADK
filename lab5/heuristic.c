#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



int main() {
    int N, S, K;
    scanf("%d%d%d", &N, &S, &K);

    int **rolls = malloc(sizeof(int) * N);
    int **actors_matrix = malloc(sizeof(int) * (K)); // kanske ändra så superskådisar får plats



    for (int i = 0; i < N; i++) {
        rolls[i] = malloc(sizeof(int) * (N+1));
    }

    for (int i = 0; i < K; i++) {
        actors_matrix[i] = malloc(sizeof(int) * (N+1));
        actors_matrix[i][0] = 0;
    }

    for (int i = 0; i < N; i++) {
        int actors_n;
        scanf("%d", &actors_n);
        rolls[i][0] = actors_n;

        for(int j = 1; j < actors_n; j++) {
            int actor;
            scanf("%d", &actor);
            rolls[i][j] = actor;

            int idx = actors_matrix[j][0];
            actors_matrix[j][idx+1] = i+1;
            actors_matrix[j][0] += 1;
        }
    }

    int **conflicts = malloc(sizeof(int*) * N);

    for (int i = 0; i < N; i++) {
        conflicts[i] = calloc(N, sizeof(int));
    }

    int *scene = malloc(sizeof(int)*N);

    for (int i = 0; i < S; i++) {
        int conflict_count;
        scanf("%d", &conflict_count);

        for(int j = 0; j < conflict_count; j++) {
            int conflict;
            scanf("%d", &conflict);
            scene[j] = conflict - 1;
        }

        for (int j = 0; j < conflict_count; j++) {
            for (int k = j; k < conflict_count; k++) {
                conflicts[scene[k]][scene[j]] = 1;
                conflicts[scene[j]][scene[k]] = 1;
            }
        }
    }

    int **actors_solution = malloc(sizeof(int) * (K+N+1));
    for (int i = 0; i < K; i++) {
        actors_solution[i] = malloc(sizeof(int) * (N+1));
        actors_solution[i][0] = 0;
    }

    int *booked_rolls = calloc(N, sizeof(int));

    int check = 0;

    for (int i = 1; i < actors_matrix[0][0]; i++) {
        for (int j = 1; j < actors_matrix[1][0]; j++) {
            if (conflicts[actors_matrix[0][i]][actors_matrix[1][j]] == 0) {

                actors_solution[0][0] = 1;
                actors_solution[1][0] = 1;

                actors_solution[0][1] = actors_matrix[0][i];
                actors_solution[1][1] = actors_matrix[1][j];
                booked_rolls[actors_matrix[0][i]] = 1;
                booked_rolls[actors_matrix[1][j]] = 1;
                check = 1;
            }
            if (check == 1) break;
        }
        if (check == 1) break;
    }

    for (int i = 2; i < K; i++) {
        for (int j = 1; j < actors_matrix[i][0]+1; j++) {
            if (booked_rolls[actors_matrix[i][j]] == 0) {
                int idx = actors_solution[i][0] + 1;
                
                actors_solution[i][idx] = actors_matrix[i][j];
                booked_rolls[actors_matrix[i][j]] = 1;
                actors_solution[i][0] += 1;


            }
        }
    }

    int super_actors = 0;

    for (int i = 0; i < N; i++) {
        if (booked_rolls[i] == 0) {
            actors_solution[super_actors+N][0] = 1;
            actors_solution[super_actors+N][1] = i+1;
            super_actors++;
        }
    }


    return 0;
}