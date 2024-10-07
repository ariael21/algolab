#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// create an undirected graph type
typedef boost::adjacency_list<
  boost::vecS,          // stores edges as std::vector
  boost::vecS,          // stores vertices as std::vector
  boost::undirectedS,   // specify the graph to be undirected
  boost::no_property,   // vertex property (none)
  boost::property<boost::edge_weight_t, int> // edge property (weighted graph)
    > WeightedUndirectedGraph;

// define edge descriptor type
typedef boost::graph_traits<WeightedUndirectedGraph>::edge_descriptor Edge; 

// define vertex descriptor type
typedef boost::graph_traits<WeightedUndirectedGraph>::vertex_descriptor Vertex;

// define weight map
typedef boost::property_map<
  WeightedUndirectedGraph,
  boost::edge_weight_t
    >::type WeightMap;

int get_mst_weight(WeightedUndirectedGraph g) {
  // create empty mst
  std::vector<Edge> mst_edges;
  
  // use boost's kruskal algorithm to compute the mst
  kruskal_minimum_spanning_tree(g, std::back_inserter(mst_edges));
  
  // get graph weights 
  WeightMap weight_map = get(boost::edge_weight, g);
  
  // sum up all edge weights
  int mst_weight = 0;
  for (Edge e : mst_edges) {
    mst_weight += weight_map[e];
  }
  
  return mst_weight;
}

int get_longest_distance(WeightedUndirectedGraph g, Vertex start) {
  // get number of vertices
  int n = boost::num_vertices(g);
  
  // empty distances vector
  std::vector<int> distances(n);
  
  // empty predecessors vector
  std::vector<Vertex> predecessors(n);
  
  // run dijkstra
  dijkstra_shortest_paths(g, start,
    boost::distance_map(&distances[0]).predecessor_map(&predecessors[0]));
  
  int max_distance = *std::max_element(distances.begin(), distances.end());
  
  return max_distance;
}

void solve() {
  int n, m; std::cin >> n >> m;
  
  // initialize graph
  WeightedUndirectedGraph g(n);
  
  // initialize weight map
  WeightMap weight_map = get(boost::edge_weight, g);
  
  for (int i = 0; i < m; i++) {
    // read values
    int u, v, w; std::cin >> u >> v >> w;
    
    // add edge (u,v) to the graph
    Edge e;
    bool inserted;
    
    // sets inserted to true, if the edges assignment was successfull
    boost::tie(e, inserted) = boost::add_edge(u, v, g);
    
    if (inserted) {
      weight_map[e] = w; 
    }
  }

  int mst_weight = get_mst_weight(g);
  
  Vertex start = 0;
  int max_distance = get_longest_distance(g, start);
 
  std::cout << mst_weight << max_distance << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}