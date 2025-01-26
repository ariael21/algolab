///
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

void solve() {
  int n, m;
  std::cin >> n >> m;
  
  std::vector<int> strengths(n);
  for (int i = 0; i < n; i++) {
    int s_i;
    std::cin >> s_i;
    
    strengths[i] = s_i;
  }
  
  std::vector<int> boxes(m);
  std::multiset<int, std::greater<int> > boxes_set;
  for (int i = 0; i < m; i++) {
    int w_i;
    std::cin >> w_i;
    
    boxes[i] = w_i;
    
    boxes_set.insert(w_i);
  }
  
  // sort boxes according to weight
  std::sort(boxes.begin(), boxes.end(), std::greater<int>());
  
  // use all people available to carry the biggest possible box they can each carry
  std::sort(strengths.begin(), strengths.end(), std::greater<int>());
  
  if (strengths[0] < boxes[0]) {
    std::cout << "impossible" << std::endl;
    return;
  }
  
  int timesteps = 0;
  
  while(!boxes_set.empty()) {
    // go through all people and choose one box for each person per timestep
    for (int i = 0; i < n; i++) {
      int person_strength = strengths[i];
      
      // returns box which is not less than the given key (person_strength)
      // thus we always choose the heaviest box which the current person can carry
      auto heaviest_carriable_box = boxes_set.lower_bound(person_strength);
      
      if (heaviest_carriable_box != boxes_set.end()) {
        boxes_set.erase(heaviest_carriable_box);
      } else {
        break; // round over => person cannot carry any box
      }
    }
    
    timesteps++;
  }

  int time_used = timesteps * (2 + 1);
  time_used -= 1; // they are already upstairs
  
  std::cout << time_used << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t; 
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}