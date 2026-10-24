/**
 * Exempel på in- och utdatahantering för maxflödeslabben i kursen
 * ADK.
 *
 * Använder scanf/printf i C.
 *
 * Author: Per Austrin
 */

#include <stdio.h>
#include <stdlib.h>

// int x; //antalet X hörn
// int y;  //antalet y hörn
// int V;  //x+y+2 Flödes input
// int e;
// int E; // e+x+y
// int lista;


// int myMax(x,y){
//   if (x>y){return x;}
//   return y;
// }

int *readBipartiteGraph() {
  int x, y, e, a, b;
  int count = 0;
  // Läs antal hörn och kanter
  scanf("%d%d%d", &x, &y, &e);
  int *allt = malloc(sizeof(int)*(2*e + 3));
  allt[count] = x;
  count++;
  allt[count] = y;
  count++;
  allt[count] = e;
  count++;

  for (int i = 0; i < e; ++i) {
    scanf("%d %d", &a, &b);
    allt[count] = a;
    count++;
    allt[count]= b;
    count++;
  }
  return allt;
}

void writeFlowGraph(int *arr ) {
  int x = arr[0], y= arr[1], e = arr[2];
  int v = x + y + 2;
  int s = v-1;
  int t = v;
  int count = 3;
  e = x+y+e;

  // Skriv ut antal hörn och kanter samt källa och sänka
  printf("%d\n%d %d\n%d\n", v, s, t, e);
<<<<<<< HEAD
  for (int i = 0; i < arr[2]; ++i) {
=======
  for (int i = 0; i < e; ++i) {
>>>>>>> fd60ec9ae0f61f0827d848f94173a050a3292e84
    int u = arr[count];
    count++;
    int v = arr[count];
    count++;
    // Kant från u till v med kapacitet c

    printf("%d %d %d\n", u, v, 1);
  }
  for (int i = 1; i <= x; i++) {
    printf("%d %d %d\n", s, i, 1);
  }
  for (int i = 1; i <= y; i++) {
    printf("%d %d %d\n", i + x, t, 1);
  }

  // Var noggrann med att flusha utdata när flödesgrafen skrivits ut!
  fflush(stdout);

  // Debugutskrift
  fprintf(stderr, "Skickade iväg flödesgrafen\n");
}


int *readMaxFlowSolution() {
  int v, e, s, t, f;
  int count = 0;
  // Läs in antal hörn, kanter, källa, sänka, och totalt flöde
  // (Antal hörn, källa och sänka borde vara samma som vi i grafen vi
  // skickade iväg)
  scanf("%d%d%d%d%d", &v, &s, &t, &f, &e);
  int *evenmore = malloc(sizeof(int)*(2*e + 5));

  evenmore[count] = v;
  count++;
  evenmore[count] = s;
  count++;
  evenmore[count] = t;
  count++;
  evenmore[count] = f;
  count++;
  evenmore[count] = e;
  count++;

  for (int i = 0; i < evenmore[4]; ++i) {
    int u, v0, f;
    // Flöde f från u till v
    scanf("%d%d%d", &u, &v0, &f);
    if (u == s || v0 == t || u == t || v0 == s) {
      e--;
      continue;
    }
    if (f == 0){ e--; continue;}

    evenmore[count] = u;
    count++;
    evenmore[count] = v0;
    count++;
  }

  evenmore[4] = e;

  return evenmore;
}


void writeBipMatchSolution(int x, int y, int *arr) {
  int maxMatch = arr[4];

  // Skriv ut antal hörn och storleken på matchningen
  printf("%d %d\n%d\n", x, y, maxMatch);

  for (int i = 0; i < maxMatch; ++i) {
    int a, b;
    a = arr[5 + 2*i];
    b = arr[5 + 2*i + 1];
    // Kant mellan a och b ingår i vår matchningslösning
    printf("%d %d\n", a, b);
  }

}


int main(void) {
  
  int *graph = readBipartiteGraph();

  writeFlowGraph(graph);

  int *flowGraph = readMaxFlowSolution();

  writeBipMatchSolution(graph[0], graph[1], flowGraph);

  // debugutskrift
  fprintf(stderr, "Bipred avslutar\n");
  free(graph);
  free(flowGraph);
  return 0;
}



  //malloc
  // Läs in kanterna
  // int *arr[x+y];
  // int grantal[x+y];
  // int m = myMax(x,y);
  // for (int j = 0 ; j<(x+y);j++){
  //   arr[j] = malloc(sizeof(int)*m);
  // }

      // allt[i] = a;
    // allt[e+i]= b;

    // // arr[a][grantal[a]] = b;
    // // grantal[a] += 1;
    // // arr[b][grantal[b]] = a;
    // // grantal[b] += 1;