#include <iostream>
#include "test.h"
#include "benchmark.h"
#include "yahtzee.h"

using namespace std;

int main() {
  bool works = test();
  if (!works) {
    cout << "\nTests failed" << endl;
    return 1;
  }
  cout << endl;

  //benchmark();

  yahtzee();
  
  return 0;
}

