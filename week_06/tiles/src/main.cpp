#include <iostream>
#include <vector>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list< 
  boost::vecS, 
  boost::vecS, 
  boost::undirectedS 
  > graph;
  
typedef boost::graph_traits<graph>::edge_iterator edge_iterator;

void solve() {
  int w, h;
  std::cin >> w >> h;
  
  int num_vertices = w * h;
  
  graph g(num_vertices);
  
  // map to store which tiles in the rectangle are part of the graph
  std::vector<std::vector<bool>> map(w, std::vector<bool>(h, false));
  
  // count active vertices
  int active_vertices = 0;
  
  // store the map in a boolean 2D array
  for (int y = 0; y < h; y++) {
    std::string value;
    std::cin >> value;
    
    for (int x = 0; x < w; x++) {
      char c = value[x];
      
      // std::cout << c << std::endl;
      
      if (c != 'x') {
        map[x][y] = true;
        // std::cout << "(" << x << "," << y << ")" << std::endl;
        active_vertices += 1;
      }
    }
  }
  
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int current_vertex = (y * w) + x;
      
      if (map[x][y]) {
        // check all surrounding points:
        // // 1) left
        // if (x != 0 && map[x-1][y]) {
        //   add_edge(current_vertex, current_vertex - 1, g);
        // }
        
        // 2) top
        if (y != 0 && map[x][y-1]) {
          add_edge(current_vertex, current_vertex - w, g);
          
          // std::cout << "(" << x << "," << y << ")" << " - " << "(" << x << "," << y-1 << ")" << std::endl;
        }
        
        // 3) right
        if (x != w-1 && map[x+1][y]) {
          add_edge(current_vertex, current_vertex + 1, g);
          // std::cout << "(" << x << "," << y << ")" << " - " << "(" << x+1 << "," << y << ")" << std::endl;
        }
        
        // // 4) bottom
        // if (y != h-1 && map[x][y+1]) {
        //   add_edge(current_vertex, current_vertex + w, g);
        // }
      }
    }
  }
  
  std::vector< boost::graph_traits< graph >::vertex_descriptor > mate(num_vertices);
  
  bool success = boost::checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
  
  int matching_size;
  
  if (success) {
    matching_size = boost::matching_size(g, &mate[0]);
  }
  
  if (success && active_vertices % 2 == 0 && (active_vertices / 2 == matching_size)) {
    // std::cout << "active_vertices / 2 = " << (active_vertices / 2) << " matching_size = " << matching_size << std::endl;
    
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  for (int i = 0; i < n; i++) {
    solve();
  }
}