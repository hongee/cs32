#include "Map.h"
#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
  WeightMap m;
  WeightMap k;

  m.enroll("A", 100);
  m.enroll("J", 120);

  assert(m.weight("A") == 100); //weight func test
  assert(m.adjustWeight("A", -200) == false);
  assert(m.adjustWeight("Z", -2) == false);
  assert(m.adjustWeight("Z", 150) == false);

  assert(m.size() == 2);

  m.print();

  cout << "Passed all tests" << endl;
}
