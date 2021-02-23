#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
using namespace std;


int NL(int **M, vector<int> tulips, int k, int n, int matrix) {
  M[0][0] = 0;

  for (int i = 1; i < k; i++) {
    M[i][0] = 1000;
  }

  for (int j = 1; j < n; j++) {
    M[0][j] = 1000;
  }

  for (int i = 1; i < k; i++) {
    for (int j = 1; j < n; j++) {

      if (matrix == 0) {
        if ((j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1)) {
          M[i][j] = min(M[i][j - 1], M[i - 1][j - 1]);
        }
        else {
          M[i][j] = min(M[i][j - 1], M[i - 1][j - 1]) + tulips[j - 1];
        }
      }

      else {
        if ((j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1)) {
          M[i][j] = min(M[i][j - 1], M[i - 1][j - 1]) + tulips[j - 1];
        }
        else {
          M[i][j] = min(M[i][j - 1], M[i - 1][j - 1]);
        }
      }
    }
  }

  int res = M[0][n-1];
	for(int i = 1; i < k; i++){
		res = min(res,M[i][n-1]);
	}
	return res;
}


int main() {
  vector<int> tulips;
  int k;

  while(scanf("%d ", &k) != EOF) {
    char first_tulip = '\0';
    char tulip = '\0';
    int count = 1;

    scanf("%c ", &first_tulip);

    while(scanf("%c", &tulip) != EOF) {
      if(tulip == '\n'){
        tulips.push_back(count);
        break;
      }

      if(tulip == first_tulip) {
        count++;
      }
      else {
        tulips.push_back(count);
        count = 1;
        first_tulip = tulip;
      }
    }

    int **M1 = (int**) malloc((k + 1) * sizeof(int*));
    for (int i = 0; i < k + 1; i++) {
      int *N1 = (int*) malloc((tulips.size() + 1) * sizeof(int));
      M1[i] = N1;
    }

    int sol1 = NL(M1, tulips, k + 1, tulips.size() + 1, 0);

    int **M2 = (int**) malloc((k + 1) * sizeof(int*));
    for (int i = 0; i < k + 1; i++) {
      int *N2 = (int*) malloc((tulips.size() + 1) * sizeof(int));
      M2[i] = N2;
    }

    int sol2 = NL(M2, tulips, k + 1, tulips.size() + 1, 1);

    tulips.clear();
    cout << min(sol1, sol2) << "\n";
  }
  return 0;
}
