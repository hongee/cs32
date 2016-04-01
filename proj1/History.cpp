#include <iostream>
#include "history.h"

using namespace std;

History::History(int nRows, int nCols)
{
    m_row = nRows;
    m_col = nCols;

    //initialise grid
    for(int i=0;i < m_row;++i) {
      for(int k=0;k < m_col;++k) {
        m_grid[i][k] = 0;
      }
    }
}

bool History::record(int r, int c)
{
  if(r < 1 || c < 1 || r > m_row || c > m_col) return false;
  m_grid[r-1][c-1]++;
  return true;
}

void History::display() const
{
  clearScreen();
  for(int i=0;i < m_row;++i) {
    for(int k=0;k < m_col;++k) {
      if(m_grid[i][k] == 0) {
        cout << '.';
      } else if (m_grid[i][k] <= 26) {
        char out = 'A' + m_grid[i][k] - 1;
        cout << out;
      } else {
        cout << 'Z';
      }
    }
    cout << endl;
  }
  cout << endl;
}
