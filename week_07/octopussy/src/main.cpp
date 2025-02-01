///
#include <iostream>
#include <vector>
#include <queue>

#include <utility>
#include <algorithm>

typedef std::pair<int, int> IPair;

int cutoff_index;
std::vector<int> explosion_times;

bool custom_less(IPair a, IPair b) {
  return a.first < b.first;
}

void solve() {
  int n;
  std::cin >> n;
  
  cutoff_index = (n - 1) / 2;
  
  explosion_times = std::vector<int>(n);
  std::vector<IPair> bombs(n);
  
  for (int i = 0; i < n; i++) {
    int t_i;
    std::cin >> t_i;
    explosion_times[i] = t_i;
    bombs[i] = IPair(t_i, i);
  }
  
  sort(bombs.begin(), bombs.end());
  
  std::vector<bool> active(n, true);
  std::queue<int> queue;
  
  bool possible = true;
  int time_step = 0;
  
  int prev_id = bombs[0].second;
  queue.push(prev_id);

  for (int i = 1; i < n; i++) {
    int curr_id = bombs[i].second;

    // neutralize bombs in queue
    while (!queue.empty()) {
      int id = queue.front(); queue.pop();
      
      if (active[id]) {
        // neutralize
        active[id] = false;
        time_step++;
        
        if (id < cutoff_index) {
          // add two bombs to queue -> these also need to be neturalized
          // in order to neutralize the current one
          queue.push(2*id + 1);
          queue.push(2*id + 2);
        }
      }
    }
    
    // only check this time
    // if time is higher than explosion time of original bomb -> fail
    if (time_step > explosion_times[prev_id]) {
      possible = false;
      break;
    }
    
    // add next bomb to queue
    queue.push(curr_id);
    prev_id = curr_id;
  }
  
  std::cout << (possible ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}