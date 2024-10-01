///1
#include <iostream>
#include <vector>
#include <cmath>

int max_winnings(std::vector<int>* x, std::vector<std::vector<int>>* memo, int i, int j, int m, int k, int current) {
  // only one element left
  if (i == j) {
    if (current == k) {
      // if its the turn of person k -> return coin value
      return x->at(i);
    } else {
      return 0; 
    }
  }
  
  // update index pointing to next person
  int next = current + 1;
  if (next == m) {
    next = 0;
  }
  
  // gather memoisation data
  int right = memo->at(i+1).at(j);
  int left = memo->at(i).at(j-1);
  
  // check if values need to be calculated
  if (right == -1) {
    right = max_winnings(x, memo, i+1, j, m, k, next);
    memo->at(i+1).at(j) = right;
  }
  
  if (left == -1) {
    left = max_winnings(x, memo, i, j-1, m, k, next);
    memo->at(i).at(j-1) = left;
  }
  
  if (current == k) {
    // its the turn of person k
    // choose maximum winnings
    return std::max((x->at(i) + right), (x->at(j) + left));
  } else {
    // its not the turn of person k
    // choose minimum winnings, as we cannot asume anything about their choices
    return std::min(right, left); 
  }
}

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  
  std::vector<int> x(n);
  
  // initialize coin values
  for (int i = 0; i < n; i++) {
    std::cin >> x.at(i);
  }
  
  // initialize memoization
  std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
  
  int max = max_winnings(&x, &memo, 0, n-1, m, k, 0);
  
  std::cout << max << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; 
  
  for (int i = 0; i < t; i++) {
    testcase();
  }
}