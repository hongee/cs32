#include <string>
using namespace std;

const char WALL = 'X';
const char OPEN = '.';
const char SEEN = 'o';

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr < 0  ||  sr > nRows  ||  sc < 0  || sc > nCols  ||
        er < 0  ||  er > nRows  ||  ec < 0  || ec > nCols  ||
        maze[sr][sc] != OPEN  ||  maze[er][ec] != OPEN)
       return false;
    /*
    If the start location is equal to the ending location, then we've
    solved the maze, so return true.
Mark the start location as visted.
For each of the four directions,
    If the location one step in that direction (from the start
  location) is unvisited,
      then call pathExists starting from that location (and
      ending at the same ending location as in the
      current call).
     If that returned true,
         then return true.
Return false.
    */
    if (sr == er  &&  sc == ec)
        return true;

    maze[sr][sc] = SEEN;
    if(maze[sr-1][sc] == OPEN) return pathExists(maze, nRows, nCols, sr-1, sc, er, ec);
    if(maze[sr][sc+1] == OPEN) return pathExists(maze, nRows, nCols, sr, sc+1, er, ec);
    if(maze[sr+1][sc] == OPEN) return pathExists(maze, nRows, nCols, sr+1, sc, er, ec);
    if(maze[sr][sc-1] == OPEN) return pathExists(maze, nRows, nCols, sr, sc-1, er, ec);
    return false;
}
