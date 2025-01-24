///3
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<long, long> Movement;

const int MAX_m = 100001;

int min_gulps(
    int index, 
    std::vector<Movement>& movements, 
    std::vector<long>& s, 
    long c_dist, 
    long c_time, 
    long T, 
    long D, 
    int c_movements
  ) {
  int n = movements.size();
  Movement c_movement = movements[index];
  
  long m_time = c_movement.second;
  long m_distance = c_movement.first;
  long new_c_time = c_time + m_time;
  
  int new_c_movements = c_movements + 1;
  
  if (index == n-1) {
    // last element -> return value
    
    // case 1: don't take current element
    long distance = c_dist;
    int local_moves = c_movements;
    
    // case 0: take the current element
    if (new_c_time < T) { 
      local_moves = new_c_movements;
      distance = distance + m_distance;
    }
    
    int min = MAX_m;
    if (distance >= D) {
      min = 0;
    } else {
       for (int i = 0; i < s.size(); i++) {
        if (distance + local_moves * s[i] >= D) {
          min = i+1;
          break;
        }
      }
    }
    
    return min;
  } else {
    // case 1: don't take current element
    int min = min_gulps(index+1, movements, s, c_dist, c_time, T, D, c_movements);
    
    // case 2: take current element
    if (new_c_time < T) {
      c_movements++;
      long new_c_dist = c_dist + m_distance;
      int c2_gulps = min_gulps(index+1, movements, s, new_c_dist, new_c_time, T, D, c_movements);
      
      min = std::min(min, c2_gulps);
    }
    
    return min;
  }
}

void solve() {
  int n, m;
  long D, T;
  
  std::cin >> n >> m;
  std::cin>> D >> T;
  
  std::vector<Movement> movements(n);
  std::vector<long> s(m);
  
  for (int i = 0; i < n; i++) {
    long d, t; std::cin >> d >> t;
    movements[i] = std::make_pair(d, t);
  }
  
  for (int i = 0; i < m; i++) {
    long s_i; std::cin >> s_i;
    s[i] = s_i;
  } 
  
  int min = min_gulps(0, movements, s, 0, 0, T, D, 0);

  if (min < MAX_m) {
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