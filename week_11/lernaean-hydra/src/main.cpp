///4
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int get_overlap(std::vector<std::vector<int>>& patterns, int first, int second, int k) {
  int x_0 = patterns[second][0];
  
  for (int i = 1; i < k; i++) {
    int s_i = patterns[first][i];
    
    if (s_i == x_0) {
      bool difference = false;
      
      for (int j = 1; i + j < k; j++) {
        int s_i_j = patterns[first][i + j];
        int x_j = patterns[second][j];
        
        if (s_i_j != x_j) {
          difference = true;
          break;
        }
      }
      
      if (!difference) {
        return k - i;
      }
    }
  }
  
  return 0;
}

bool allow_connection(std::vector<std::vector<int>>& patterns, int index, int overlap, int k) {
  int group = patterns[index][k-1];
  
  for (int i = 0; i < k; i++) {
    int x_i = patterns[index][i];
    
    if (i < overlap) {
      if (x_i < group - (overlap - i)) {
        return false;
      }
    } else {
      if (x_i < group) {
        return false;
      }
    }
  }
  
  return true;
}

void solve() {
  int n, m, k, d;
  std::cin >> n >> m >> k >> d;
  
  std::vector<std::vector<int>> patterns(m, std::vector<int>(k));
  std::vector<std::vector<int>> groups(n);
  
  for (int i = 0; i < m; i++) {
    
    for (int j = 0; j < k; j++) {
      int h_j;
      std::cin >> h_j; 
      patterns[i][j] = h_j;
    }
    
    int group = patterns[i][k-1];
    groups[group].push_back(i);
  }
  
  int N = m + 2;
  int start = N - 2;
  int end = N - 1;
  
  weighted_graph G(N);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < n-1; i++) {
    for (int j = 0; j < groups[i].size(); j++) {
      int first = groups[i][j];
      
      for (int h = 0; h < groups[i+1].size(); h++) {
        int second = groups[i+1][h];
        
        int overlap = get_overlap(patterns, first, second, k);

        if (allow_connection(patterns, second, overlap, k)) {
          edge_desc e;
          e = boost::add_edge(first, second, G).first; 
          weights[e] = k - overlap;
        }
      }
    }
  }
  
  for (int i = 0; i < groups[0].size(); i++) {
    edge_desc e;
    e = boost::add_edge(start, groups[0][i], G).first; 
    weights[e] = 0;
  }
  
  for (int i = 0; i < groups[n-1].size(); i++) {
    edge_desc e;
    e = boost::add_edge(groups[n-1][i], end, G).first; 
    weights[e] = k;
  }
  
  int shortest_path = dijkstra_dist(G, start, end);
  
  if (shortest_path > k * n) {
    std::cout << "Impossible!" << std::endl;
  } else {
    std::cout << shortest_path << std::endl;
  }
}

int main() {
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}