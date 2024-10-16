#include <iostream>
#include <vector>
#include <cmath>

/*
- coin_values: contains the values for all n coins
- memo: contains the precalculations for the subproblems
- i: left index of the current interval
- j: right index of the current interval
- m: number of passengers
- k: number of the person for which we are calculating the largest possible winnings
- current: the number of the person which is currently choosing a coin
*/
int max_winnings(std::vector<int>* coin_values, std::vector<std::vector<int>>* memo, int i, int j, int m, int k, int current) {
  // only one element left
  if (i == j) {
    if (current == k) {
      // if its the turn of person k -> return coin value
      return coin_values->at(i);
    } else {
      return 0; 
    }
  }
  
  // update index pointing to next person
  int next = (current + 1) % m;
  
  // gather memoisation data
  int right = memo->at(i+1).at(j);
  int left = memo->at(i).at(j-1);
  
  // check if values need to be calculated
  if (right == -1) {
    right = max_winnings(coin_values, memo, i+1, j, m, k, next);
    memo->at(i+1).at(j) = right;  // save calculated values
  }
  
  if (left == -1) {
    left = max_winnings(coin_values, memo, i, j-1, m, k, next);
    memo->at(i).at(j-1) = left;   // save calculated values
  }
  
  if (current == k) {
    // its the turn of person k
    
    // choose maximum winnings
    return std::max(
      (coin_values->at(i) + right), // case 1: add left coin
      (coin_values->at(j) + left)   // case 2: add right coin
    );
  } else {
    // its not the turn of person k
    
    // choose minimum winnings, as we cannot asume anything about their choices
    return std::min(
      right,  // case 1
      left    // case 2
    ); 
  }
}

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  
  // initialize coin values
  std::vector<int> coin_values(n);
  
  for (int i = 0; i < n; i++) {
    std::cin >> coin_values.at(i);
  }
  
  // initialize memoization
  std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
  
  // calculate max winnings for person k
  std::cout << max_winnings(&coin_values, &memo, 0, n-1, m, k, 0) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; 
  
  for (int i = 0; i < t; i++) {
    testcase();
  }
}