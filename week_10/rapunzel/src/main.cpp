#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <deque>
#include <set>
#include <limits>

typedef std::deque<int> IStack;
typedef std::vector<int> IV;
typedef std::set<int> ISet;
typedef std::vector<std::vector<int>> IVV;
typedef std::map<int, int> IIMap;

const int MAX_INT = std::numeric_limits<int>::max();

void check(IStack& dfs_stack, IIMap& recent_h, int m, int k, ISet& start_pos) {
  auto stack_start = dfs_stack.begin();
  
  if (start_pos.find(*(stack_start + m-1)) == start_pos.end()) {
      
    int min = recent_h.begin()->first;
    int max = recent_h.rbegin()->first;
    
    if (max - min <= k) {
      start_pos.insert(*(stack_start + m-1));
    }
  }
}

void remove(IIMap& map, int brightness) {
  map[brightness]--;

  if (map[brightness] == 0) {
    map.erase(brightness);
  }
}

void add(IIMap& map, int brightness) {
  if (map.find(brightness) != map.end()) {
    map[brightness]++;
  } else {
    map.insert(std::make_pair(brightness, 1));
  }
}

void dfs(
  int node, 
  int depth, 
  int m, 
  int k, 
  IStack& dfs_stack, 
  IV& h, 
  ISet& start_pos, 
  IVV& adjacency_list, 
  IIMap& recent_h) {

  depth++; // go one level deeper  
  dfs_stack.push_front(node);
  add(recent_h, h[node]); // add new brightness
  
  // remove brightness which is further away than m steps
  if (depth > m-1) {
    auto stack_start = dfs_stack.begin();
    remove(recent_h, h[*(stack_start + m)]);
  }
  
  // check if previous m nodes result in a valid sequence 
  if (depth >= m-1) { 
    check(dfs_stack, recent_h, m, k, start_pos);
  }
  
  // go down to all children
  for(int child : adjacency_list[node]) {
    dfs(child, depth, m, k, dfs_stack, h, start_pos, adjacency_list, recent_h);
  }
  
  // now we step out again:
  
  // add brightness again
  if (depth > m-1) {
    auto stack_start = dfs_stack.begin();
    add(recent_h, h[*(stack_start + m)]);
  }
  
  remove(recent_h, h[node]); // remove current brightness
  dfs_stack.pop_front(); // remove current node from stack
  depth--; // go up one level
}

void solve() {
  int n, m, k;
  std::cin >> n >> m >> k;
  
  IV h(n);
  for (int i = 0; i < n; i++) {
    int h_i;
    std::cin >> h_i;
    h[i] = h_i;
  }
  
  IVV adjacency_list(n);
  for (auto neighbors : adjacency_list) {
    neighbors.reserve(n);
  }
  
  for (int i = 0; i < n-1; i++) {
    int u, v;
    std::cin >> u >> v;
    adjacency_list[u].push_back(v);
  }
  
  IStack dfs_stack;
  IIMap recent_h;
  ISet start_pos;
  
  dfs(0, -1, m, k, dfs_stack, h, start_pos, adjacency_list, recent_h);
  
  if (start_pos.size() > 0) {
    for (int i : start_pos) {
      std::cout << i << " ";
    }
    
    std::cout << std::endl;
  } else {
    std::cout << "Abort mission" << std::endl;
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