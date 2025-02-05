///3
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<long, long> Movement;

const int MAX_M = 100001;

void best_distance(
  int move,
  int used_movements,
  long time_left,
  long c_distance_left,
  std::vector<Movement>& movements, 
  std::vector<long>& distance_per_moves) {
  
  // (1): stopping condition based on time -> order according to time
  if (move >= movements.size() || time_left <= movements[move].second) {
    distance_per_moves[used_movements] = std::min(distance_per_moves[used_movements], c_distance_left);
    return;
  }  
  
  Movement current_movement = movements[move];
  long c_time = current_movement.second;
  long c_distance = current_movement.first;
  
  // dont use current movement
  best_distance(move+1, used_movements, time_left, c_distance_left, movements, distance_per_moves); 
    
  // use current movement
  best_distance(move+1, used_movements+1, time_left - c_time, c_distance_left - c_distance, movements, distance_per_moves); 
}

void solve() {
  int n, m;
  long D, T;
  
  std::cin >> n >> m;
  std::cin>> D >> T;
  
  std::vector<Movement> movements(n);
  std::vector<long> s(m);
  
  for (int i = 0; i < n; i++) {
    long d, t; 
    std::cin >> d >> t;
    
    // d = distance, t = time 
    movements[i] = std::make_pair(d, t);
  }
  
  for (int i = 0; i < m; i++) {
    long s_i; std::cin >> s_i;
    s[i] = s_i;
  } 
  
  std::sort(std::begin(movements), std::end(movements), [](const auto &a, const auto &b){
    // sort according to time
    // select less time intensive movements first because of see: (1)
    return a.second < b.second;
  });
  
  std::vector<long> distance_per_moves(n, std::numeric_limits<long>::max());
  best_distance(0, 0, T, D, movements, distance_per_moves);
  
  long min = MAX_M;
  for (int i = 1; i < n; i++) {
    if (distance_per_moves[i] <= 0) {
      // already reached the goal => early end
      std::cout << "0" << std::endl;
      return;
    }
    
    long c_moves = i;
    long c_distance_left = distance_per_moves[i];
    
    if (c_distance_left != std::numeric_limits<long>::max()) {
      for (int j = 0; j < m; j++) {
        // check if additional gain is enough
        if (c_distance_left <= c_moves * s[j]) {
          min = std::min(min, (long)j+1);
          break;
        }
      }
    }
  }

  if (min < MAX_M) {
    std::cout << min << std::endl;
  } else {
    std::cout << "Panoramix captured" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}