#include <iostream>
#include <queue>
#include <string>

using namespace std;

const char VISITED = 'v';

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
  queue<Coord> coordqueue;

  coordqueue.push(Coord(sr, sc));
  maze[sr][sc] = VISITED;

  int i = 1;

  while(!coordqueue.empty()) {
    Coord current = coordqueue.front();
    coordqueue.pop();
    int cc = current.c();
    int cr = current.r();

    cout << i << ": " << cr << " | " << cc << endl;

    if(cr == er && cc == ec ) {
      return true;
    }
    //North
    if(maze[cr-1][cc] == '.') {
      coordqueue.push(Coord(cr-1, cc));
      maze[cr-1][cc] = VISITED;
    }
    //East
    if(maze[cr][cc+1] == '.') {
      coordqueue.push(Coord(cr, cc+1));
      maze[cr][cc+1] = VISITED;
    }
    //South
    if(maze[cr+1][cc] == '.') {
      coordqueue.push(Coord(cr+1, cc));
      maze[cr+1][cc] = VISITED;
    }
    //West
    if(maze[cr][cc-1] == '.') {
      coordqueue.push(Coord(cr, cc-1));
      maze[cr][cc-1] = VISITED;
    }
    i++;
  }
  return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
