#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int index(int x, int y, int n) {
  return n*x + y;
}

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<std::vector<bool>> board(n, std::vector<bool>(n));
  
  int v = 0;
  
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      int i;
      std::cin >> i;
      
      board[x][y] = i == 1 ? true : false;
      v += i;
    }
  }
  
  int N = n*n;
  
  graph G(N);
  
  for (int y = 0; y < n; y++) {
    for (int x = 0; x < n; x++) {
      if (board[x][y]) {
        // connect to neighbors

        int i = index(x,y,n);
        
        if (x-1 >= 0 && y-2 >= 0 && board[x-1][y-2]) {
          boost::add_edge(i, index(x-1,y-2,n), G);
        }
        
        if (x-1 >= 0 && y+2 < n && board[x-1][y+2]) {
          boost::add_edge(i, index(x-1,y+2,n), G);
        }
        
        if (x+1 < n && y-2 >= 0 && board[x+1][y-2]) {
          boost::add_edge(i, index(x+1,y-2,n), G);
        }
        
        if (x+1 < n && y+2 < n && board[x+1][y+2]) {
          boost::add_edge(i, index(x+1,y+2,n), G);
        }

        if (x-2 >= 0 && y-1 >= 0 && board[x-2][y-1]) {
          boost::add_edge(i, index(x-2,y-1,n), G);
        }
        
        if (x-2 >= 0 && y+1 < n && board[x-2][y+1]) {
          boost::add_edge(i, index(x-2,y+1,n), G);
        }
        
        if (x+2 < n && y-1 >= 0 && board[x+2][y-1]) {
          boost::add_edge(i, index(x+2,y-1,n), G);
        }
        
        if (x+2 < n && y+1 < n && board[x+2][y+1]) {
          boost::add_edge(i, index(x+2,y+1,n), G);
        }
      }
    }
  }
  
  std::vector<vertex_desc> mate_map(N);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  
  int max_is = v - matching_size;
  
  std::cout << max_is << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}