int** MakeSpiral(int n) {
  int** grid = new int*[n];
  for (int i = 0; i < n; i++) {
    grid[i] = new int[n];
  }
  //          R   D  L  U
  int dx[] = {0, 1, 0, -1};
  int dy[] = {1, 0, -1, 0};
  int x = 0;
  int y = 0;
  int direction = 0;
  const int kDirectoinsSize = 4;
  for (int i = 1; i <= n * n; i++) {
    grid[x][y] = i;
    int tx = x + dx[direction];
    int ty = y + dy[direction];
    bool x_is_out = tx < 0 || tx >= n;
    bool y_is_out = ty < 0 || ty >= n;
    if (x_is_out || y_is_out || grid[tx][ty] != 0) {
      direction = (direction + 1) % kDirectoinsSize;
      tx = x + dx[direction];
      ty = y + dy[direction];
    }
    x = tx;
    y = ty;
  }
  return grid;
}
