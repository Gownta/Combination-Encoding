#include "test.h"
#include "combencode.h"

#include <iostream>
#include <iomanip>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

static int C(int n, int k) {
  if (k == 0) return 1;
  return C(n-1,k-1)*n/k;
}

static void inc1(int n, int k, int * data) {
  int i = k-1;
  while (i >= 0 && data[i] >= n) {
    data[i] = 1;
    i--;
  }
  if (i >= 0) data[i]++;
}

static void inc(int n, int k, int * data) {
  bool valid;
  do {
    inc1(n,k,data);
    valid = true;
    for (int i = 0; i < k-1; ++i) if (data[i] >= data[i+1]) {
      valid = false;
      break;
    }
  } while (!valid);
}

static void print (int k, const int * data) {
  cout << "{";
  for (int i = 0; i < k-1; ++i) cout << data[i] << ", ";
  cout << data[k-1] << "}";
} 

///////////////////////////////////////////////////////////////////////////////

static bool test(int n, int k, int e, const int * data, int method) {
  cout << setw(2) << e << " === ";
  print(k, data);

  cout << ": encoding... " << flush;
  bool passed = encode(n,k,data) == e;
  if (passed) cout << "passed";
  else        cout << "failed";

  cout << ", decoding... " << flush;
  int tmp[128];
  for (int i = 0; i < 128; ++i) tmp[i] = 0xdec0ded;
  decode(n,k,e,tmp, method);
  bool success = tmp[k] == 0xdec0ded;
  for (int i = 0; i < k; ++i) success = success && tmp[i] == data[i];
  if (success) cout << "passed";
  else         cout << "failed";

  cout << endl;
  return passed && success;
}

static bool testall(int n, int k, int method) {
  int data[128];
  for (int i = 0; i < k; ++i) data[i] = i+1;

  int c = C(n,k);
  
  cout << "====================\n"
       << "Testing n = " << n << ", k = " << k << " : C(" << n << ", " << k << ") = " << c
       << " (using method " << method << ")"
       << "\n" << endl;

  bool ret = true;
  for (int e = c-1; e >= 0; --e, inc(n,k,data)) {
    ret = ret && test(n,k,e,data,method);
  }

  cout << "\n";
  if (ret) cout << "Passed";
  else cout << "Failed";
  cout << " (" << n << "," << k << ")\n\n" << endl;
  return ret;
}

bool test() {
  bool ret = true;
  for (int m = 1; m <= 3; ++m) {
    ret = ret && testall(1,1,m);
    ret = ret && testall(4,1,m);
    ret = ret && testall(4,2,m);
    ret = ret && testall(4,3,m);
    ret = ret && testall(4,4,m);
    ret = ret && testall(7,3,m);
  }
  cout << "\n" << endl;
  return ret;
}

