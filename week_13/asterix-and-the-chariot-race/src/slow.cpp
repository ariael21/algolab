///
#include <iostream>
#include <vector>
#include <utility>
#include <limits>

typedef std::pair<long, long> Pair;

const long LONG_MAX = std::numeric_limits<long>::max();

Pair min_cost(int root, bool covered_parent, std::vector<std::vector<int>>& tree, std::vector<int>& costs) {
  long take_cost = 0;
  long not_take_cost = 0;
  
  int children_amount = tree[root].size();
  
  if (children_amount == 0) {
    // CASE 0: reached leaf
    
    if (covered_parent) {
      // parent covers this node -> should not be taken
      take_cost = LONG_MAX;
    } else {
      // parent does not cover this node -> should take this node
      take_cost = costs[root];
      not_take_cost = LONG_MAX;
    }
  } else {
    // CASE 1: take root
    take_cost += costs[root];
    
    for (int i = 0; i < children_amount; i++) {
      Pair cost = min_cost(tree[root][i], true, tree, costs);
      take_cost += std::min(cost.first, cost.second); // does not matter if we take or do not take child
    }
    
    // CASE 2: do not take root
    if (covered_parent) {
      // CASE 2.1: parent is covered -> does not matter which one we take
      
      for (int i = 0; i < children_amount; i++) {
        Pair cost = min_cost(tree[root][i], false, tree, costs);
        not_take_cost += std::min(cost.first, cost.second); // does not matter if we take or do not take child
      }
    } else {
      // CASE 2.2: parent is not covered -> we need to take at least one child
      long min_diff = LONG_MAX;
      bool one_covered = false;
      
      for (int i = 0; i < children_amount; i++) {
        Pair cost = min_cost(tree[root][i], false, tree, costs);
        
        // calculate min diff across all children
        long diff = std::abs(cost.first - cost.second);
        min_diff = std::min(min_diff, diff);
        
        if (cost.first <= cost.second) {
          one_covered = true;
        }
        
        not_take_cost += std::min(cost.first, cost.second); 
      }
      
      // if none of the children took the node => take the one which has min diff
      if (!one_covered) {
        not_take_cost += min_diff;
      }
    }
  }
  
  return std::make_pair(take_cost, not_take_cost);
}

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<std::vector<int>> tree(n);
  for (auto node: tree) {
    node.reserve(n);
  }
  
  for (int i = 0; i < n-1; i++) {
    int u, v;
    std::cin >> u >> v;
    
    tree[u].push_back(v);
  }
  
  std::vector<int> c(n);
  
  for (int i = 0; i < n; i++) {
    int c_i;
    std::cin >> c_i;
    c[i] = c_i;
  }
  
  Pair costs = min_cost(0, false, tree, c);
  
  long result = std::min(costs.first, costs.second);
  
  std::cout << result << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}