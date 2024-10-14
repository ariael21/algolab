#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
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

int dijkstra(const Graph &graph, int start, int end) {
  int n = boost::num_vertices(graph);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(
    graph, 
    start,
    boost::distance_map(
      boost::make_iterator_property_map(
        dist_map.begin(), 
        boost::get(boost::vertex_index, graph)
      )
    )
  );

  return dist_map[end];
}

void solve() {
  // approach:
  // - calculate the MST for all the networks (kruskal)
  // - connect the networks and add the best edge
  // - calculate the length of the shortest path from a to b (dijkstra)
  
  int n, e, s, a, b; 
  std::cin >> n >> e >> s >> a >> b;
  
  std::vector<Graph> graphs(s, Graph(n));
  std::vector<Weightmap> weightmaps(s);
  
  // initialize weightmaps for the s insect networks
  for (int i = 0; i < s; i++) {
    weightmaps[i] = boost::get(boost::edge_weight, graphs[i]);
  }
  
  // load graphs
  for (int i = 0; i < e; i++) {
    int t_1, t_2; 
    std::cin >> t_1 >> t_2;
    
    for (int j = 0; j < s; j++) {
      int w_j; 
      std::cin >> w_j;
      
      Edge e;
      bool inserted;
      boost::tie(e, inserted) = boost::add_edge(t_1, t_2, graphs[j]);
      
      if (inserted) {
        weightmaps[j][e] = w_j;
      }
    }
  }
  
  for (int i = 0; i < s; i++) {
    int hive;
    std::cin >> hive;
  }
  
  Graph tree(n);
  Weightmap weightmap = boost::get(boost::edge_weight, tree);
  
  for (int i = 0; i < s; i++) {
    std::vector<Edge> mst;
    
    // calculate mst
    boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(mst));
    
    for (Edge e : mst) {
      bool inserted;
      
      Vertex u = source(e, graphs[i]);
      Vertex v = target(e, graphs[i]);
      
      // only add optimal eges
      if (!boost::edge(u, v, tree).second) {
        // does not exist
        Edge new_edge;
      
        boost::tie(new_edge, inserted) = boost::add_edge(u, v, tree);
        
        if (inserted) {
          weightmap[new_edge] = weightmaps[i][e];
        }
      } else {
        Edge old_edge = boost::edge(u, v, tree).first;
        
        // add smallest edge
        weightmap[old_edge] = std::min(weightmap[old_edge], weightmaps[i][e]);
      }
    }
  }
  
  // calculate the shortest path in the created graph
  std::cout << dijkstra(tree, a, b) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}