//2d array pointers

//Arrays are stored in contiguous memory all the time
// int arr[2][3];
//  [1] [2] [3]  |  [4] [5] [6]

class History {
  public:
    History(int nRows, int nCols) : m_rows(nRows), m_cols(nCols) {
      grid = new int*[m_rows];
      for(int r=0;r < m_rows;r++) {
        grid[r] = new int[m_cols];
        for(int c = 0; c < m_cols; c++) {
          m_grid[r][c] = 0;
        }
      }
    }
  private:
    int m_rows;
};
