#include <iostream>
#include <vector>
#include <algorithm>

int max_winnings(std::vector<int> &v, std::vector<std::vector<int>> &memo, int start, int end, bool turn) {
  if (start == end) {
    if (turn) {
      return v.at(start);
    } else {
      return 0;
    }
  }
  
  int v_start = v.at(start);
  int v_end = v.at(end);
  
  int right_winnings = memo.at(start + 1).at(end);
  int left_winnings = memo.at(start).at(end - 1);
  
  if (right_winnings == -1) {
    right_winnings = max_winnings(v, memo, start + 1, end, !turn);
    memo.at(start + 1).at(end) = right_winnings;
  }
  
  if (left_winnings == -1) {
    left_winnings = max_winnings(v, memo, start, end - 1, !turn);
    memo.at(start).at(end - 1) = left_winnings;
  }
  
  if (turn) {
    return std::max(
        (v_start + right_winnings),
        (v_end + left_winnings)
      );
  } else {
    return std::min(right_winnings, left_winnings);
  }
}

void solve() {
  int n; std::cin >> n;
  
  std::vector<int> v(n);
  
  for (int i = 0; i < n; i++) {
    std::cin >> v.at(i);
  }
  
  std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
  
  int max = max_winnings(v, memo, 0, n - 1, true);
  
  std::cout << max << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}