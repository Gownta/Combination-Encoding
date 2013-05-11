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

static void decode_bottom_up(int n, int k, int e, int * data) {
  // bottom-up search
  // INVARIANT: c = C(n-j,k-i)

  int i = 0;
  int j = 1;
  int c = C(n-j,k-i);

  while (i < k-1) {
    // increment j while it cannot be the first element
    while (c > e) {
      c = c * (n-j-k+i) / (n-j);
      j++;
    }

    // j is the first element
    data[i] = j;

    // adjust variables for next loop
    e -= c;
    c  = c * (k-i) / (n-j);
    j += 1;
    i += 1;
  }

  // final element optimization
  data[k-1] = n - e;
}

static void decode_top_down(int n, int k, int e, int * data) {
  // top-down search

  for (int i = 0; i < k-1; ++i) {
    int j = n-k+i+1;
    int c = 0;

    int nextc = 1; // C(n-(j-1),k-i)
    while (nextc <= e) {
      c = nextc;
      nextc = nextc * (n-j+2) / (n-j-k+i+2);
      j--;
    }

    e -= c;
    data[i] = j;
  }

  data[k-1] = n - e;
}

static void decode_binary(int n, int k, int e, int * data) {
  // uneven binary search
  // INVARIANT: the next element in the sequence is greater than lb
  // INVARIANT: c = C(upper_bound, k-i)

  int lb = 0;
  int inc = n/k;
  
  for (int i = 0; i < k-1; ++i) {
    int c;

    // test increments of the lower bound by n/k
    while (lb+inc <= n-i+1 && (c = C(n-(lb+inc), k-i)) > e) lb += inc;

    // set up a feasible upper bound and an infeasible lower bound
    int low_fail  = lb;
    int high_pass = lb+inc;
    if (high_pass > n-i+1) {
      high_pass = n-i+1;
      c = 0;
    }

    // perform binary search for i in the range (lb, lb+inc]
    int mid;
    while ((mid = (low_fail + high_pass) / 2) != low_fail) {
      int mc = C(n-mid, k-i);
      if (mc <= e) {
        high_pass = mid;
        c = mc;
      } else {
        low_fail = mid;
      }
    }

    e -= c;
    data[i] = high_pass;
  }

  // final element optimization
  data[k-1] = n - e;
}

void decode(int n, int k, int e, int * data, int method) {
  if      (method == 1) decode_bottom_up(n, k, e, data);
  else if (method == 2) decode_top_down(n, k, e, data);
  else                  decode_binary(n, k, e, data);
}

/*
void decode(int n, int k, int e, int * data, int method) {
  for (int i = 0; i < k-1; ++i) {
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
  data[k-1] = n - e;
}
*/

