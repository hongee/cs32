#include <iostream>
#include <string>
#include "Map.h"
#include "WeightMap.h"

bool WeightMap::enroll(std::string name, double startWeight) {
  // If a person with the given name is not currently in the map,
  // there is room in the map, and the startWeight is not negative,
  // add an entry for that person and weight and return true.
  // Otherwise make no change to the map and return false.
  if(startWeight < 0) return false;
  return m_map.insert(name, startWeight);
}

double WeightMap::weight(std::string name) const {
  // If a person with the given name is in the map, return that
  // person's weight; otherwise, return -1.
  double v = -1;
  m_map.get(name, v);
  return v;
}

bool WeightMap::adjustWeight(std::string name, double amt) {
  // If no person with the given name is in the map or if
  // weight() + amt is negative, make no change to the map and return
  // false.  Otherwise, change the weight of the indicated person by
  // the given amount and return true.  For example, if amt is -8.2,
  // the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
  // pounds.
  double o = weight(name);
  if(o == -1 || (o + amt < 0)) return false;
  return m_map.update(name, o + amt);
}

int WeightMap::size() const {
  // Return the number of people in the WeightMap.
  return m_map.size();
}

void WeightMap::print() const {
  // Write to cout one line for every person in the map.  Each line
  // has the person's name, followed by one space, followed by that
  // person's weight.
  for(int i = 0; i < size(); i++) {
    std::string k;
    double v;
    m_map.get(i,k,v);
    std::cout << k << " " << v << std::endl;
  }
}
