#include "Map.h"  // class template from problem 1

class Coord
{
  public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    Coord() : m_r(0), m_c(0) {}
    double r() const { return m_r; }
    double c() const { return m_c; }
  private:
    double m_r;
    double m_c;
};

int main()
{
    Map<int, double> mid;
    mid.insert(42, -1.25);         // OK
    Map<Coord, int> mpi;
    mpi.insert(Coord(40,10), 32);  // error!
}
