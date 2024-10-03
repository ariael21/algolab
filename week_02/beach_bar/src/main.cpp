///3
#include <iostream>
#include <vector>
#include <algorithm>

void add_coordinates(std::vector<int>* coordinates, int start, int end) {
  int distance = std::abs(start - end);
  int middle = start + (distance / 2);
  
  coordinates->push_back(middle);
  
  if (distance % 2 == 1) {
    coordinates->push_back(middle + 1);
  }
}

void testcase() {
  int n; std::cin >> n;
  std::vector<int> x(n);
  
  for (int i = 0; i < n; i++) {
    std::cin >> x.at(i);
  }
  
  std::sort(x.begin(), x.end());
  
  // window indexes 
  int i = 0;
  int j = 0;
  
  int amount = 1;
  int distance = 0;
  int window_size = 0;
  
  // store values for maximum amount of points and maximum distance found
  int max_amount = 1;
  int max_distance = 0;
  
  std::vector<int> coordinates;
  coordinates.push_back(x.at(i));
  
  bool running = true;
  
  while (running) {
    if (j == n) {
      break;
    }
    
    int x_i = x.at(i);
    int x_j = x.at(j);
    
    // calculate total size of window
    window_size = std::abs(x_i - x_j);
    
    // calculate distance to outermost point
    distance = window_size / 2;
    
    // if the window has an odd size 
    // -> the longer distance to one of the outermost points is +1 longer, due to integer rounding
    if (window_size % 2 == 1) {
      distance += 1;
    }

    if ((amount > max_amount && distance <= 100) || 
      (amount == max_amount && distance < max_distance)) {
      // better solution found
      // either:
      //  1) more points within range
      //  2) found smaller maximal distance
      
      // clear previously saved coordinates
      coordinates.clear();

      // add coordinates for current window
      add_coordinates(&coordinates, x_i, x_j);
      
      // update new maximum amount and new maximum distance
      max_amount = amount;
      max_distance = distance;
      
    } else if (amount == max_amount && distance == max_distance) {
      // found window with same maximum amount of points and same maximum distance
      // -> add additional coordinate
      add_coordinates(&coordinates, x_i, x_j);
    }
    
    // updated window
    if (distance > 100) {
      i += 1;
      amount -= 1;
    } else if (j < n) {
      j += 1;
      amount += 1;
    }
  }
  
  std::cout << max_amount << " " << max_distance << std::endl;

  for (auto i = 0; i < coordinates.size(); i++) {
    std::cout << coordinates.at(i) << " ";
  }
  
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}