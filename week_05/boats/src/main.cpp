///
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> Boat;

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<Boat> boats(n);
  
  for (int i = 0; i < n; i++) {
    int l_i, p_i;
    std::cin >> l_i >> p_i;
    
    boats[i] = std::make_pair(l_i, p_i);
  }
  
  // sort the boats, such that the boats are sorted according to their assigned ring
  // the boat with the leftmost ring comes first
  std::sort(boats.begin(), boats.end(),
    [](const Boat& e1, const Boat& e2) -> bool {
      return e1.second < e2.second;
  });
  
  // dp[i] = min. distance from start (right boundary) using i+1 boats
  
  // base case: 
  // leftmost boat is always attached to the ring, such that its 
  // rightmost point is exactly at the ring 
  std::vector<int> dp(n);
  dp[0] = boats[0].second;
  
  int i = 1;
  int boat_index = 1;
  
  while (true) {
    int p_i = boats[boat_index].second;
    int l_i = boats[boat_index].first;

    // case 1: 
    // current right boundary is overlapping with new boat:
    // 1.1) replace the previous boat with this one (if we get a lower right boundary)
    // 1.2) or we dont choose this boat
    if (dp[i-1] > p_i) {
      
      if (i == 1) {
        std::cout << "error: something went wrong!!" << std::endl;
      }
      
      // check if we can add current node: p_i, l_i instead of p_i-1, l_i-1 
      // => to make dp[i-1] smaller
      
      // previous right boundary:
      int previous_boundary = dp[i-2];
      int new_boundary = std::max(p_i, previous_boundary + l_i);

      // 1) add the one which produces the min. right boundary
      dp[i-1] = std::min(dp[i-1], new_boundary);

      // we used one additional boat
      boat_index++;
    } else {
      // case 2: 
      // add node
      dp[i] = std::max(p_i, dp[i-1] + l_i);
      
      boat_index++;
      i++;
    }
    
    if (boat_index == n) {
      // end if all boats are used up
      break;
    }
  }
  
  std::cout << i << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}