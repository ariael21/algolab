///
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  boost::no_property,
  boost::property<boost::edge_weight_t, int>
    > Graph;
    
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

typedef boost::property_map<Graph, boost::edge_weight_t>::type Weightmap;

int dijkstra(const Graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  
  std::vector<int> dist_map(n); //exterior property
  
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(
      boost::make_iterator_property_map(
        dist_map.begin(),
        boost::get(boost::vertex_index, G)
      )
    )
  );
  
  // for (int i = 0; i < n; i++) {
  //   std::cout << dist_map.at(i) << ", ";
  // }
  
  return dist_map.at(t);
}

void solve() {
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;
  
  int levels = k + 1;
  
  int N = levels * n;
  int Y = y + n * k;
  
  Graph g(N);
  Weightmap weightmap = boost::get(boost::edge_weight, g);
    
  // 1) load graph
  for (int i = 0; i < m; i++) {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    
    if (d == 1) {
      // river -> connect two layers
      for (int j = 0; j < k; j++) {
        // add two edges
        
        // source from upper layer and target in layer one below
        Edge e1;
        bool inserted1;
        boost::tie(e1, inserted1) = boost::add_edge(a + j * n, b + (j + 1) * n, g);
        
        if (inserted1) {
          weightmap[e1] = c;
        }
        
        // source in layer below and target in upper layer
        Edge e2;
        bool inserted2;
        boost::tie(e2, inserted2) = boost::add_edge(a + (j + 1) * n, b + j * n, g);
        
        if (inserted2) {
          weightmap[e2] = c;
        }
      }
    }
    
    for (int j = 0; j <= k; j++) {
      Edge e;
      bool inserted;
      boost::tie(e, inserted) = boost::add_edge(a + j * n, b + j * n, g);
      
      if (inserted) {
        weightmap[e] = c;
      }
    }
  }
  
  std::cout << dijkstra(g, x, Y) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}