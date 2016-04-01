
#include "Map.h"
#include <iostream>

using namespace std;

//init Map with nullptrs on head and tail
Map::Map()
 : m_size(0), head(nullptr), tail(nullptr)
{}

Map::Map(const Map& src)
{
  m_size = 0;
  head = nullptr;
  tail = nullptr;

  Node* p;
  for(p = src.head; p != nullptr; p = p->next) {
    insert(p->m_key, p->m_value);
  }
}

Map::~Map()
{
  Node* p;
  for(p = head; p!=nullptr; p=p->next) {
    delete p->previous;
    if(p->next == nullptr) {
      delete p;
      break;
    }
  }
}

Map& Map::operator=(const Map &src)
{
  //sanity check
  if(this == &src) return *this;

  //delete every node;
  Node* p;
  for(p = head; p!=nullptr; p=p->next) {
    delete p->previous;
    if(p->next == nullptr) {
      delete p;
      break;
    }
  }

  tail = nullptr;
  head = nullptr;
  m_size = 0;

  //recreate list
  for(p = src.head; p != nullptr; p = p->next) {
    insert(p->m_key, p->m_value);
  }

  return *this;
}

Map::Node::Node()
 : next(nullptr), previous(nullptr)
{}

Map::Node::Node(const KeyType& k, const ValueType& v)
 : m_key(k), m_value(v), next(nullptr), previous(nullptr)
{}

bool Map::erase(const KeyType& key)
{
    Node* n = find(key);

    if(n == nullptr) //not found
        return false;

    Node* n_prev = n->previous;
    Node* n_next = n->next;

    //if previous node is nullptr, this node is at the HEAD
    //hence set HEAD to next
    if(n_prev != nullptr)
      n_prev->next = n_next;
    else
      head = n_next;

    //if next is nullptr, this node is at the TAIL
    //set TAIL to previous
    if(n_next != nullptr)
      n_next->previous = n_prev;
    else
      tail = n_prev;

    m_size--;
    delete n;

    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* n = find(key);
    if (n == nullptr)  // not found
        return false;
    value = n->m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)
        return false;

    //set p to HEAD, go to next until desired index
    Node* p = head;
    for(int k = 0; k < i; k++) {
      p = p->next;
    }

    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other)
{
    //swap head and tails
    Node* temp_head = head;
    head = other.head;
    other.head = temp_head;

    Node* temp_tail = tail;
    tail = other.tail;
    other.tail = temp_tail;
    //yay!

    // Swap sizes
    int t = m_size;
    m_size = other.m_size;
    other.m_size = t;
}

Map::Node* Map::find(const KeyType& key) const
{
      // Do a linear search through the LL.
      Node* p;
      for(p = head; p!=nullptr; p = p->next) {
        if(p->m_key == key) return p;
      }
      return p;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
                           bool mayInsert, bool mayUpdate)
{
    Node* n = find(key);
    if (n != nullptr)  // found
    {
        if (mayUpdate)
            n->m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    //not found, allowed to insert, inserting

    n = new Node(key, value);

    if(tail == nullptr) {
      //there are no elements in the array
      //implies HEAD is also nullptr
      head = n;
      tail = n;
    } else {
      //we are at the last element!
      tail->next = n;
      n->previous = tail;
      tail = n;
    }

    m_size++;
    return true;
}

void Map::dump() const
{
  cerr << endl << "-- BEGIN DUMP --" << endl;
  Node* p;
  for(p = head; p!=nullptr; p = p->next) {
    cerr << "K: " << p->m_key << " | V: " << p->m_value << endl;
  }
  cerr << "-- END DUMP --" << endl;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
  bool noDiffDuplicate = true;
  const Map* source_map_ptr;

  //if result is aliased with m1, set the source to m2
  //otherwise we'll use m1 as the source, and assign m2 to result
  //(its fine if they're aliased)
  if(&result == &m1)
    source_map_ptr = &m2;
  else {
    result = m2;
    source_map_ptr = &m1;
  }

  for(int i = 0; i < source_map_ptr->size(); i++) {
    KeyType k;
    ValueType v1,v2;
    source_map_ptr->get(i,k,v1);
    //check if key exists in result
    if(result.get(k,v2)) {
      //if values not the same delete this key from result
      if(v2 != v1) {
        result.erase(k);
        noDiffDuplicate = false;
      }
    } else {
      //otherwise update
      result.insert(k, v1);
    }
  }

  return noDiffDuplicate;
}

void subtract(const Map& m1, const Map& m2, Map& result) {

  //hold a temp map = m1 to prevent aliasing issue
  Map temp = m1;

  //remove m2 from m1
  for(int i = 0; i < m2.size(); i++) {
    KeyType k;
    ValueType v;
    m2.get(i,k,v);
    temp.erase(k);
  }

  result = temp;
}
