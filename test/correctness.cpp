#include <iostream>
#include "combencode.h"

using namespace std;
extern int C(int, int);

bool test_encode(int n, int k, const int * data, int expected) {
  cout << expected << " =? encode(" << n << ", " << k << ", {";
  for (int i = 0; i < k-1; ++i) cout << data[i] << ", ";
  cout << data[k-1] << "})... " << flush;

  int tmp[128];
  for (int i = 0; i < k; ++i) tmp[i] = data[i];

  int e = encode(n, k, tmp);

  bool passed = e == expected;
  if (passed) cout << "passes";
  else             cout << "failed (" << e << ")";
  cout << endl;

  return passed;
}

bool test_decode(int n, int k, const int * expected, int e) {
  cout << "decode(" << n << ", " << k << ", " << e << ") =? {";
  for (int i = 0; i < k-1; ++i) cout << expected[i] << ", ";
  cout << expected[k-1] << "}... " << flush;

  int tmp[128];
  for (int i = 0; i < 128; ++i) tmp[i] = 0xdec0ded;

  decode(n, k, e, tmp);

  bool passed = true;
  if (tmp[k] != 0xdec0ded) passed = false;
  else for (int i = 0; i < k; ++i) passed = passed && expected[i] == tmp[i];
  
  if (passed) cout << "passes";
  else {
    cout << "failed ({";
    for (int i = 0; i < k-1; ++i) cout << tmp[i] << ", ";
    cout << tmp[k-1] << "}" << (tmp[k] == 0xdec0ded ? "" : ", x") << ")";
  }
  cout << endl;

  return passed;
}

bool test(int n, int k, const int * data, int expected) {
  return test_encode(n, k, data, expected) && test_decode(n, k, data, expected);
}

int main() {
  int ret = 0;

  int e = 5;
  for (int i1 = 1; i1 < 4; ++i1) for (int i2 = i1+1; i2 < 5; ++i2) {
    int testn[] = { i1, i2 };
    ret |= !test(4, 2, testn, e--);
  }

  int test2[] = { 1, 2, 3, 4, 5 };
  ret |= !test(5, 5, test2, 0);

  int test3[] = { 1 };
  ret |= !test(1,1,test3,0);
  ret |= !test(5,1,test3,4);

  return ret;
}

