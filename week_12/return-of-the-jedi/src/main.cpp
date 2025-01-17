///
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef std::pair<int, int> Edge;

int get_id(int i1, int i2) {
  return std::max(i1, i2) * 1000 + std::min(i1, i2);
}

void solve() {
  int n, i;
  std::cin >> n >> i;
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  std::vector<edge_desc> graph_edges;
  graph_edges.reserve(n*n);
  
  edge_desc e;
  for (int j = 0; j < n-1; j++) {
    for (int k = 1; k < n-j; k++) {
      int cost;
      std::cin >> cost;
      
      e = boost::add_edge(j, j+k, G).first; 
      weights[e] = cost;
      
      graph_edges.push_back(e);
    }
  }

  std::vector<std::vector<bool>> in_mst(n, std::vector<bool>(n, false));
  
  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  std::vector<std::vector<int>> adjacency_list(n);
  for (auto neighbours : adjacency_list) {
    neighbours.reserve(n);
  }
  
  int mst_cost = 0;
  
  std::map<int, int> edge_weight_map;

  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    int u = boost::source(*it, G);
    int v = boost::target(*it, G);
    int cost = weights[*it];
    
    in_mst[u][v] = true;
    in_mst[v][u] = true;
    
    adjacency_list[u].push_back(v);
    adjacency_list[v].push_back(u);
    
    edge_weight_map.insert({get_id(u, v), cost});

    mst_cost += cost;
  }
  
  std::vector<std::vector<int>> longest_edge_between(n, std::vector<int>(n, 0));
  
  for (int j = 0; j < n; j++) {
    // go through all vertices and compute max distance connecting them
    std::vector<bool> reached(n, false);
    std::queue<Edge> queue;
    
    for (auto next_id: adjacency_list[j]) {
      queue.push(std::make_pair(j, next_id));
    }
    
    reached[j] = true;
    
    while (!queue.empty()) {
      Edge e = queue.front(); queue.pop();
      
      int current_edge_cost = edge_weight_map[get_id(e.first, e.second)];
      int previous_edge_cost = longest_edge_between[j][e.first];

      longest_edge_between[j][e.second] = std::max(previous_edge_cost, current_edge_cost);

      reached[e.second] = true;
      
      for (auto next_id: adjacency_list[e.second]) {
        if (!reached[next_id]) {
          queue.push(std::make_pair(e.second, next_id));
        }
      }
    }
  }
  
  int min_difference = -1;

  for (auto edge : graph_edges) {
    int u = boost::source(edge, G);
    int v = boost::target(edge, G);
    
    if (!in_mst[u][v]) {
      int abs_difference = std::abs(longest_edge_between[u][v] - weights[edge]);
      
      min_difference = (min_difference == -1 || abs_difference < min_difference) ? abs_difference : min_difference;

      if (min_difference == 0) {
        break;
      }
    }
  }
  
  std::cout << mst_cost + min_difference << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
    // std::cout << "---------------" << std::endl;
  }
}