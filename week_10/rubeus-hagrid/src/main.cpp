#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

typedef std::pair<long, long> lPair; // first: chamber_id, second: path_length
typedef std::vector<std::vector<lPair>> nodeVector;
typedef std::vector<long> lVector;

struct Result {
  long max_earnings;
  long nodes;
  long used_time;
};

Result compute(int root, nodeVector& tree, lVector& galleons) {
  long max_earnings = 0;
  long nodes = 0;
  long used_time = 0;
  
  if (tree[root].size() == 0) {
    // 1) base case: reach a leaf
    return { galleons[root], 1, 0 };
  } else {
    // 2) compute galleon losses per subtree
    int num_children = tree[root].size();
    
    std::vector<Result> child_results(num_children);
    
    for (int i = 0; i < num_children; i++) {
      auto child = tree[root][i];
      auto result = compute(child.first, tree, galleons);
      
      result.used_time += 2 * child.second; // + cost to travel to and back
      result.max_earnings -= result.nodes * child.second; // subtract max earnings by travel time to subtree
      
      child_results[i] = result;
    }
    
    std::sort(child_results.begin(), child_results.end(),
      [](const Result& e1, const Result& e2) -> bool {
        long loss_e1_first = e1.used_time * e2.nodes; // loss if we choose e1 before e2
        long loss_e2_first = e2.used_time * e1.nodes;  // loss if we choose e2 before e1
        
        bool e1_first = loss_e1_first < loss_e2_first; // we go into e1 first, if we loose less money
       
        return e1_first;
    });
    
    for (auto child_result : child_results) {
      long subtree_loss = used_time * child_result.nodes;
      max_earnings += child_result.max_earnings - subtree_loss;
      
      nodes += child_result.nodes;
      used_time += child_result.used_time;
    }
    
    max_earnings += galleons[root]; // add this node earnings
    nodes += 1; // add this node
    
    return {max_earnings, nodes, used_time};
  }
}

void solve() {
  long n;
  std::cin >> n;
  
  nodeVector tree(n+1);
  lVector galleons(n+1);
  
  for (long i = 1; i <= n; i++) {
    long g_i;
    std::cin >> g_i;
    
    galleons[i] = g_i;
  }
  
  for (long i = 0; i < n; i++) {
    long u, v, l;
    std::cin >> u >> v >> l;
    
    tree[u].emplace_back(std::make_pair(v, l));
  }
  
  std::cout << compute(0, tree, galleons).max_earnings << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  long t;
  std::cin >> t;
  
  for (long i = 0; i < t; i++) {
    solve();
  }
}