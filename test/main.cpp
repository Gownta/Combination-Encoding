#include <iostream>
#include "test.h"

using namespace std;

int main() {
  bool works = test();
  if (works) cout << "PASSED";
  else       cout << "FAILED";
  cout << "\n" << endl;
  return !works;
}

