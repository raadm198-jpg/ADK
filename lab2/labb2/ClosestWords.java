/* Labb 2 i DD2350 Algoritmer, datastrukturer och komplexitet    */
/* Se labbinstruktionerna i kursrummet i Canvas                  */
/* Ursprunglig författare: Viggo Kann KTH viggo@kth.se           */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int closestDistance = -1;

  int partDist(int[][] matrix, String w1, String w2, int w1len, int w2len, int startlength) {
    if (w1len == 0)
      return w2len;
    if (w2len == 0)
      return w1len;

    int cost = 0;

    for (int i = 1; i < w1len+1; i++) {
      for (int j = 1+startlength; j < w2len+1; j++) {
        cost = w1.charAt(i-1) == w2.charAt(j-1) ? 0 : 1;
        matrix[i][j] = Math.min(matrix[i][j-1] + 1, Math.min(matrix[i-1][j-1] + cost, matrix[i-1][j] + 1));
      }
    }

    return matrix[w1len][w2len];
  }

  int distance(int[][] matrix, String w1, String w2, int startlength) {
    return partDist(matrix, w1, w2, w1.length(), w2.length(), startlength);
  }

  public ClosestWords(String w, List<String> wordList) {

    int[][] matrix = new int[w.length()+1][50];
    for (int i = 0; i < w.length()+1; i++) {matrix[i][0] = i;}
    for (int i = 1; i < 50; i++) {matrix[0][i] = i;}

    String lastWord = "";
    int startlength = 0;
    for (String s : wordList) {
      if (lastWord.length() > 0) {
        for (int i = 0; i < Math.min(s.length(), lastWord.length()); i++) {
          if (s.charAt(i) == lastWord.charAt(i)) {
            startlength++;
            continue;
          }
          break;
        }
      }
      int dist = distance(matrix, w, s, startlength);
      // System.out.println("d(" + w + "," + s + ")=" + dist);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      }
      else if (dist == closestDistance)
        closestWords.add(s);
      lastWord = s;
      startlength = 0;
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}

// bara lite ändringar

// Processing folder: ./test
// Processing testcase: testmedordlista
// CPU time for this test: 26 ms
// Processing testcase: testmedordlista2
// CPU time for this test: 1 ms

//Processing folder: large
//Processing testcase: testmedordlista3
//CPU time for this test: 110 ms
//Processing testcase: testmedordlista4
//CPU time for this test: 231 ms

// lite fler ändringar

// Processing folder: ./test
// Processing testcase: testmedordlista
// CPU time for this test: 20 ms
// Processing testcase: testmedordlista2
// CPU time for this test: 1 ms

// Processing folder: large
// Processing testcase: testmedordlista3
// CPU time for this test: 87 ms
// Processing testcase: testmedordlista4
// CPU time for this test: 93 ms