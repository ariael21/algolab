#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  
  double e = 0;
  int prefix_sum = 0;
  
  for (int i = 0; i < n; i++) {
    int x_i;
    std::cin >> x_i;
    
    prefix_sum += x_i;
    
    if (prefix_sum % 2 == 0) {
      e += 1;
    }
  }
  
  double o = n - e;
  
  // the even pairs are split into three parts:
  // 1) the even pairs using the first element (i = 0)
  // 2) pairs which dont start at i = 0 but are calculated using two even prefix sums
  // 2) pairs which dont start at i = 0 but are calculated using two odd prefix sums
  int pairs = e 
    + e * ((e - 1) / 2)   // e choose 2 -> combinations of two even prefix sums
    + o * ((o - 1) / 2);  // o choose 2 -> combinations of two odd prefix sums
  
  std::cout << pairs << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    testcase();
  }
}