#include "combencode.h"

// return n-choose-k
static int C(int n, int k) {
  if (n < k) return 0;
  if (2*k > n) k = n - k;
  int c = 1;
  for (int i = 0; i < k; ++i) c = c * (n-i) / (i+1);
  return c;
}

int encode(int n, int k, const int * data) {
  int e = 0;
  for (int i = 0; i < k; ++i) e += C(n-data[i], k-i);
  return e;
}

#ifndef GOWNTA_TEST
#define GOWNTA_TEST 0
#endif

void decode(int n, int k, int e, int * data, int method) {
  for (int i = 0; i < k; ++i) {
    // compute min{ j : C(n-j,k-i) <= e }
    // cache C(n-j,k-i)
    int j;
    int c;

    if (!(GOWNTA_TEST) || method == 1) {
      // bottom-up search
      j = 1+i;
      c = C(n-j,k-i);
      while (c > e) {
        c = c * (n-j-k+i) / (n-j);
        j++;
      }
    } else if (method == 2) {
      // top-down search
      j = n-k+i+1;
      c = 0;
      int nextc = 1; // C(n-(j-1),k-i)
      while (nextc <= e) {
        c = nextc;
        nextc = nextc * (n-j+2) / (n-j-k+i+2);
        j--;
      }
    } else {
      // binary search
      int low_false = i;
      int high_true = n-k+i+1;
      c = 0;
      int mid;
      while ((mid = (low_false + high_true) / 2) != low_false) {
        int mc = C(n-mid, k-i);
        bool possible = mc <= e;
        if (possible) {
          high_true = mid;
          c = mc;
        } else {
          low_false = mid;
        }
      }
      j = high_true;
    }

    e -= c;
    data[i] = j;
  }
}

