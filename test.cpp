#include <string>
#include <iostream>

class Foo
{
  public:
    Foo(int x, int y, int z) : a(x), b(y), c(z) {};
    Foo(double x, double y, double z)  : a(x), b(y), c(z) {};
  private:
    int a,b,c;
};

struct Bar
{
  std::string k{"asd"};
};


int main() {
  Foo moo(1,2,3);
  Foo* b = new Foo(1,2,4);
  Bar c;
  std::cout << c.k;
  delete b;
}
