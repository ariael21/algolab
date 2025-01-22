///2
#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <tuple>

typedef std::tuple<long, long, long> Tuple;

const long LONG_MAX = std::numeric_limits<int>::max();

Tuple min_cost(int root, std::vector<std::vector<int>>& tree, std::vector<int>& costs) {
  long take_cost = 0;
  long covered_cost = 0;
  long not_covered_cost = 0;
  
  int children_amount = tree[root].size();
  
  if (children_amount == 0) {
    // BASE CASE: reached leaf
    take_cost = costs[root];
    covered_cost = costs[root];  // leaf nodes cannot be covered by lower nodes
    not_covered_cost = 0;
    
  } else {
    long cheapest_child_cost_diff = LONG_MAX;
    bool child_taken = false;
    
    long children_not_covered = 0;
    long children_covered = 0;
    
    for (int i = 0; i < children_amount; i++) {
      Tuple cost = min_cost(tree[root][i], tree, costs);
      
      long child_take_c = std::get<0>(cost);
      long child_covered_c = std::get<1>(cost);
      long child_not_covered_c = std::get<2>(cost);
      
      // 1) take current node:
      children_not_covered += child_not_covered_c; // non of the children must be covered
      
      // 2) do not cover current node: (all children must be covered)
      children_covered += child_covered_c;
      
      // 3) cover current node: (either select or one child is taken)
      long diff = child_take_c - child_covered_c;
      cheapest_child_cost_diff = std::min(cheapest_child_cost_diff, diff);
    }
    
    take_cost = costs[root] + children_not_covered;
    not_covered_cost = children_covered;
    
    long one_child_taken = not_covered_cost + cheapest_child_cost_diff;
    covered_cost = std::min(take_cost, one_child_taken);
    
    not_covered_cost = std::min(take_cost, not_covered_cost);
  }
  
  return std::make_tuple(take_cost, covered_cost, not_covered_cost);
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
  
  Tuple cost = min_cost(0, tree, c);
  
  int take_c = std::get<0>(cost);
  int covered_c = std::get<1>(cost);
  int not_covered_c = std::get<2>(cost);
  
  std::cout << covered_c << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}