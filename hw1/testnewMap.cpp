#include "newMap.h"
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

    Map g;

    cerr << "OPERATOR" << endl;
    g = m;
    g.dump();

    cerr << "COPY" << endl;
    Map f(m);
    f.dump();

    cerr << "MISC" << endl;
    Map zoo(100);
    cerr << zoo.empty();
    zoo = g;
    zoo.dump();

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

    Map zos(-1);

    cout << "Passed all tests" << endl;
}
