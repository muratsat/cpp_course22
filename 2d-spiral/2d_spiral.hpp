int** MakeSpiral(int n) {
  int** grid = new int*[n];
  for (int i = 0; i < n; ++i) {
    grid[i] = new int[n]();
  }
  int dx[] = {0, 1, 0, -1};
  int dy[] = {1, 0, -1, 0};
  int x = 0;
  int y = 0;
  int dir = 0;
  const int kDirections = 4;
  for (int i = 1; i <= n * n; i++) {
    grid[x][y] = i;
    int tx = x + dx[dir];
    int ty = y + dy[dir];
    bool is_tx_out = tx < 0 || tx >= n;
    bool is_ty_out = ty < 0 || ty >= n;
    if (is_tx_out || is_ty_out || grid[tx][ty] != 0) {
      dir = (dir + 1) % kDirections;
      tx = x + dx[dir];
      ty = y + dy[dir];
    }
    x = tx;
    y = ty;
  }
  return grid;
}