#include <iostream>
#include <vector>

#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace boost {
    struct edge_component_t {
        enum { num = 555 };
        typedef edge_property_tag kind;
    } edge_component;
}

// Graph definition with custom edge component property
typedef boost::adjacency_list<
  boost::vecS,  // Vertex storage
  boost::vecS,  // Edge storage
  boost::undirectedS,  // Undirected graph
  boost::no_property,  // No vertex properties
  boost::property<boost::edge_component_t, std::size_t>  // Edge properties for biconnected components
> Graph;

typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;

void solve() {
  int n, m; 
  std::cin >> n >> m;
  
  // approach:
  // - create graph
  // - calculate bi connected components
  // - count edges in each component
  // - component with only one edges -> important bridge
  
  // 0) create graph
  Graph g(n);
  
  for (int i = 0; i < m; i++) {
    int e1, e2; 
    std::cin >> e1 >> e2;
    boost::add_edge(e1, e2, g);
  }
  
  // 1) find biconnected components
  boost::property_map<Graph, boost::edge_component_t>::type component_map = boost::get(boost::edge_component, g);

  boost::biconnected_components(g, component_map);

  // 2) count the edges within each component
  std::vector<int> edges_in_component_count(n, 0);
  
  EdgeIterator edges_begin, edges_end;
  for(boost::tie(edges_begin, edges_end) = boost::edges(g); edges_begin != edges_end; edges_begin++) {
    edges_in_component_count.at(component_map[*edges_begin]) += 1;
  }
  
  // 3) find edges which are in a component with only one edge -> bridge
  std::vector<std::pair<int, int>> bridges;
  
  for(boost::tie(edges_begin, edges_end) = boost::edges(g); edges_begin != edges_end; edges_begin++) {
    if (edges_in_component_count.at(component_map[*edges_begin]) == 1) {
      int source = boost::source(*edges_begin, g);
      int target = boost::target(*edges_begin, g);
      
      bridges.push_back(std::make_pair(std::min(source, target), std::max(source, target))); 
    }
  }
  
  // 4) print bridges
  std::sort(bridges.begin(), bridges.end());
  
  std::cout << bridges.size() << std::endl;
  for (std::pair<int, int> pair : bridges) {
    std::cout << pair.first << " " << pair.second << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}