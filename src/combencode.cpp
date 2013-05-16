#include "combencode.h"

///////////////////////////////////////////////////////////////////////////////
// Compute binomial coefficients

static int C(int n, int k) {
  if (n < k) return 0;
  if (2*k > n) k = n - k;
  int c = 1;
  for (int i = 0; i < k; ++i) c = c * (n-i) / (i+1);
  return c;
}

///////////////////////////////////////////////////////////////////////////////
// Encode and decode functions

static int encode_normal(int n, int k, const int * data) {
  int e = 0;
  for (int i = 0; i < k; ++i) e += C(n-data[i], k-i);
  return e;
}

static int encode_linear(int n, int k, const int * data) {
  int e = 0;
  int i = 0;
  int j = 1;
  int c = C(n-j, k);
  for (;;) {
    while (j != data[i]) {
      c = c * (n-j-k+i) / (n-j);
      j++;
    }
    e += c;
    if (i == k-1) break;
    c  = c * (k-i) / (n-j);
    i += 1;
    j += 1;
  }
  return e;
}

static void decode_linear(int n, int k, int e, int * data) {
  // bottom-up search
  // INVARIANT: the next element in the sequence is at least equal to j
  // INVARIANT: c = C(n-j,k-i)

  int j = 1;
  int c = C(n-j,k);

  for (int i = 0; i < k-1; ++i) {
    // increment j while it is not the i'th element
    while (c > e) {
      c = c * (n-j-k+i) / (n-j);
      j++;
    }

    // the i'th element is j
    data[i] = j;

    // adjust variables for next loop
    e -= c;
    c  = c * (k-i) / (n-j);
    j += 1;
  }

  // final element optimization
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
    while (lb+inc <= n-k+i+1 && (c = C(n-(lb+inc), k-i)) > e) lb += inc;

    // set up a feasible upper bound and an infeasible lower bound
    int low_fail  = lb;
    int high_pass = lb+inc;
    if (high_pass > n-k+i+1) {
      high_pass = n-k+i+1;
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

///////////////////////////////////////////////////////////////////////////////
// Dispatchers

int encode(int n, int k, const int * data, int method) {
  if (method == 1) return encode_linear(n, k, data);
  /* else */       return encode_normal(n, k, data);
}

void decode(int n, int k, int e, int * data, int method) {
  if (method == 1) decode_linear(n, k, e, data);
  else             decode_binary(n, k, e, data);
}

