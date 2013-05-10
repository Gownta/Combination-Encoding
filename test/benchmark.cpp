#include "benchmark.h"
#include "combencode.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

///////////////////////////////////////////////////////////////////////////////

static const int N = 40;
static const int K = 10;

static int C(int n, int k) {
  if (k == 0) return 1;
  return C(n-1,k-1)*n/k;
}

static int timer(int m, int n, int k) {
  int top = C(n,k);
  int reps = 1000000 / top / k + 1;
  int tot = reps * top;

  int data[128];
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  {
    for (int r = 0; r < reps; ++r) for (int e = 0; e < top; ++e) {
      decode(n,k,e,data,m);
    }
  }
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto elapsed = t2 - t1;
  return elapsed.count()*1000/tot;
}

void benchmark() {
  cout << setw(20) << "Test" << setw(20) << "bottom-up" << setw(20) << "top-down" << setw(20) << "binary" << "\n"
       << "================================================================================" << endl;
  for (int r = 0; r < 1; ++r) {
    for (int k = 1; k < K; ++k) for (int n = k; n < N; ++n) {
      if (n > 25 && k > 3) continue;
      cout << "        C(" << setw(5) << n << ", " << setw(2) << k << ")";
      int t1 = timer(1,n,k);
      int t2 = timer(2,n,k);
      int t3 = timer(3,n,k);
      t1 = timer(1,n,k);
      t2 = timer(2,n,k);
      t3 = timer(3,n,k);

      int best = min(t1, min(t2, t3));
      static const int margin = 1;
      static const string w("\x1b[1;;42m"); // green
      static const string g("\x1b[1;;44m"); // blue
      static const string b("\x1b[1;;41m"); // red
      static const string e("\x1b[0m");     // reset

      const string * t1c = &e;
      if (best + margin >= t1) t1c = &g;
      if (best == t1) t1c = &w;
      const string * t2c = &e;
      if (best + margin >= t2) t2c = &g;
      if (best == t2) t2c = &w;
      const string * t3c = &e;
      if (best + margin >= t3) t3c = &g;
      if (best == t3) t3c = &w;

      cout << *t1c << setw(20) << t1 << *t2c << setw(20) << t2 << *t3c << setw(20) << t3 << e << endl;
    }
    cout << endl;
  }
  cout << endl;
}

