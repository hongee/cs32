#include "newMap.h"
#include <iostream>

using namespace std;

Map::Map() {
  m_arr = new Node[DEFAULT_MAX_ITEMS];
  m_len = 0;
  m_max = DEFAULT_MAX_ITEMS;
}

Map::Map(int max) {
  if(max < 0) exit(1);
  m_arr = new Node[max];
  m_len = 0;
  m_max = max;
}

Map::~Map() {
  delete [] m_arr;
}

Map::Map(const Map &src) {
  m_max = src.m_max;
  m_len = src.m_len;
  m_arr = new Node[m_max];
  for(int i = 0; i < m_len; i++) {
    m_arr[i] = src.m_arr[i];
  }
}

Map& Map::operator=(const Map &src) {
  if(&src == this) return *this;
  delete [] m_arr;
  m_max = src.m_max;
  m_len = src.m_len;
  m_arr = new Node[m_max];
  for(int i = 0; i < m_len; i++) {
    m_arr[i] = src.m_arr[i];
  }
  return *this;
}

bool Map::empty() const {
  if(m_len == 0) return true;
  else return false;
}

int Map::size() const {
  return m_len;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).
  if(m_len >= DEFAULT_MAX_ITEMS);

  for(int i = 0; i < m_len; i++) {
    if(m_arr[i].key == key) return false;
  }

  m_arr[m_len].key = key;
  m_arr[m_len].value = value;
  m_len++;
  return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.
  for(int i = 0; i < m_len; i++) {
    if(m_arr[i].key == key) {
      m_arr[i].value = value;
      return true;
    }
  }
  //no matches
  return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).
  if(update(key, value)) {
    return true;
  }
  return insert(key, value);
}

bool Map::erase(const KeyType& key) {
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
  for(int i = 0; i < m_len; i++) {
    if(m_arr[i].key == key) {
      for(int k = i; k < m_len - 1; k++) {
        m_arr[k] = m_arr[k+1];
      }
      m_len--;
      return true;
    }
  }
  return false;
}

bool Map::contains(const KeyType& key) const {
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
  for(int i = 0; i < m_len; i++) {
    if(m_arr[i].key == key) return true;
  }

  return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
  // If key is equal to a key currently in the map, set value to the
  // value in the map that that key maps to, and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
  for(int i = 0; i < m_len; i++) {
    if(m_arr[i].key == key) {
      value = m_arr[i].value;
      return true;
    }
  }
  return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.  (See below for details about this function.)
  if(i >= 0 && i<m_len) {
    key = m_arr[i].key;
    value = m_arr[i].value;
    return true;
  }
  return false;
}

void Map::swap(Map& other) {
  // Exchange the contents of this map with the other one.
  Node* temp_arr = m_arr;
  m_arr = other.m_arr;
  other.m_arr = temp_arr;

  int temp_max = m_max;
  m_max = other.m_max;
  other.m_max = temp_max;

  int temp_len = m_len;
  m_len = other.m_len;
  other.m_len = temp_len;
}

void Map::dump() const {
  cerr << "---begin-dump---" << endl;
  for(int i=0;i<m_len;i++) {
    cerr << "key: " << m_arr[i].key << " | val: " << m_arr[i].value << endl;
  }
  cerr << "----end-dump----" << endl;
}
