#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<
  boost::vecS, 
  boost::vecS, 
  boost::directedS,
  boost::no_property, 
  boost::property<boost::edge_weight_t, long> 
    > weighted_graph;
    
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

struct Edge {
  int other;
  long points;
};

void solve() {
  int n, m, k;
  long x;
  std::cin >> n >> m >> x >> k;
  
  int start_vertex = 0;
  
  std::vector<std::vector<Edge>> from_edges(n);
  std::vector<std::vector<Edge>> to_edges(n);
  
  for (int i = 0; i < m; i++) {
    int u, v; 
    long p;
    std::cin >> u >> v >> p;
    
    from_edges.at(u).push_back({v, p});
    to_edges.at(v).push_back({u, p});
  }
  
  for (int i = 0; i < n; i++) {
    if (from_edges[i].size() == 0) {
      // no outgoing edges from vertex i
      
      int num_in = to_edges[i].size();
      
      for (int j = 0; j < num_in; j++) {
        // skip sink vertex -> we get to go from the sinks to the start for free
        // achieve this by adding an edge directly from the 'from' vertex to the 'start' vertex
        int from = to_edges[i][j].other;
        int points = to_edges[i][j].points;
        
        from_edges[from].push_back({start_vertex, points});
      }
    }
  }
  
  std::vector<long> prev_scores(n, -1);
  prev_scores[0] = 0;
  
  for (int i = 0; i < k; i++) {
    // simulate k moves
    
    std::vector<long> current_scores(n, -1);
    
    for (int v = 0; v < n; v++) {
      // for each vector v
      int num_edges = from_edges[v].size();
      
      if (prev_scores[v] != -1 && num_edges > 0) {
        
        for (int e = 0; e < num_edges; e++) {
          int from = v;
          int to = from_edges[v][e].other;
          long points = from_edges[v][e].points;
          
          long prev_score = std::max(prev_scores[to], current_scores[to]);
          long additional = std::max(prev_scores[from], (long) 0);
          
          long new_score = additional + points;

          if (prev_score < new_score) {
            current_scores[to] = new_score;
          }
        }
      }
    }
    
    for (int v = 0; v < n; v++) {
      // check if any score is high enough
      if (current_scores[v] >= x) {
        std::cout << i + 1 << std::endl;
        return;
      }
    }
    
    std::swap(prev_scores, current_scores);
  }
  
  std::cout << "Impossible" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}