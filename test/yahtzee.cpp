#include "yahtzee.h"
#include "combencode.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

static const int N = 100000;
static const int R = 100;

void yahtzee() {
  srand(time(NULL));

  // generate random dice rolls
  int rolls[N][5];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < 5; ++j) {
      rolls[i][j] = rand() % 6 + 1;
    }
    std::sort(&rolls[i][0], &rolls[i][5]);
  }

  // encode the rolls
  int encodings[N];
  for (int i = 0; i < N; ++i) {
    //for (int j = 0; j < 5; ++j) cout << rolls[i][j] << ",";
    //cout << "    ";

    int num[6] = { 0 };
    for (int j = 1; j <= 6; ++j) {
      num[j-1] = count(&rolls[i][0], &rolls[i][5], j);
    }

    //for (int j = 0; j < 6; ++j) cout << num[j] << ",";
    //cout << "    ";

    int data[5];
    int sum = 0;
    for (int j = 0; j < 5; ++j) {
      data[j] = sum + 1 + num[j];
      sum = data[j];
    }

    //for (int j = 0; j < 5; ++j) cout << data[j] << ",";
    //cout << "    ";

    encodings[i] = encode(10,5,data);
    
    //cout << encodings[i] << endl;
  }

  // time the decodes
  string methods[3] = { "bottom-up", "top-down ", "binary   " };
  int acc = 0;
  for (int m = 1; m <= 3; ++m) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    int data[N][5];
    for (int r = 0; r < R; ++r) {
      for (int i = 0; i < N; ++i) {
        decode(10,5,encodings[i],&data[i][0]);
      }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    cout << methods[m-1] << " in " << (t2 - t1).count() << endl;
    acc += data[rand() % N][rand() % 5];
  }
  cout << acc << "\r" << "                    " << endl;
}
