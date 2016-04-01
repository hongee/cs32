#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{

    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);



    Map j;
    j.insert("dsdasd", 123123123);
    j.insert("pika", 213);
    j.insertOrUpdate("pika", 123);


    m.dump();
    j.dump();
    m.swap(j);

    cout << "-----" << endl;
    m.dump();
    j.dump();

    cout << "Passed all tests" << endl;
}
