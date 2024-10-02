#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  
  int toppled = 0;
  int maximal_reach = 1;  // first dominoe is being pushed over

  for (int i = 0; i < n; i++) {
    int h_i; std::cin >> h_i;
    
    if (maximal_reach - 1 >= i) {
      // the current dominoe is still being pushed over
      toppled += 1;
      
      int current_position = i + 1;
      
      // calculate the reach of the current dominoe
      int current_reach = current_position + h_i - 1; 
      
      // check if the current dominoe extends the current maximal reach
      if (current_reach > maximal_reach) {
        maximal_reach = new_end;
      }
    }
  }
  
  std::cout << toppled << std::endl;
}

int main() {
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    testcase();
  }
}