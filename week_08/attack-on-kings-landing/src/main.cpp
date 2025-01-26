#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/graph/max_cardinality_matching.hpp>
    
typedef boost::adjacency_list<
  boost::vecS, 
  boost::vecS, 
  boost::undirectedS,
  boost::no_property, 
  boost::property<boost::edge_weight_t, int> 
    > graph;
    
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

void solve() {
  int n, m, b, p, d;
  std::cin >> n >> m >> b >> p >> d;
  
  int num_vertices = n + p;
  
  std::set<int> barracks;
  std::set<int> plazas;
  
  // 0) store barrack and plaza location
  for (int i = 0; i < b; i++) {
    int b_i; 
    std::cin >> b_i;
    
    barracks.insert(b_i);
  }
  
  for (int i = 0; i < p; i++) {
    int p_i;
    std::cin >> p_i;
    
    plazas.insert(p_i);
  }
  
  // 1) create the graph
  graph g(num_vertices);
  weight_map weights = boost::get(boost::edge_weight, g);
  
  std::vector<int> copy_index(n, -1);
  
  int used_plazas = 0;
  
  for (int i = 0; i < m; i++) {
    int x, y, l;
    std::cin >> x >> y >> l;
    
    // check if either end is a plaza
    if (plazas.find(x) != plazas.end()) {
      // x is a plaza
      if (copy_index.at(x) == -1) {
        // need to make a copy
        copy_index.at(x) = n + used_plazas;
        used_plazas += 1;
      } 
        
      // add an edge from copy_index.at(x) to y
      edge_desc e = boost::add_edge(copy_index.at(x), y, g).first;
      weights[e] = l;
      
    } else if (plazas.find(y) != plazas.end()) {
      // y is a plaza
      if (copy_index.at(y) == -1) {
        // need to make a copy
        copy_index.at(y) = n + used_plazas;
        used_plazas += 1;
      } 
      
      // add an edge from copy_index.at(y) to x
      edge_desc e = boost::add_edge(x, copy_index.at(y), g).first;
      weights[e] = l;
    }
    
    // add an edge between the two vertices with weight l
    edge_desc e = boost::add_edge(x, y, g).first;
    weights[e] = l;
  }
  
  // 2) check which vertices are reachable
  
  std::vector<bool> reachable(n + p, false);
  
  for (int barrack : barracks) {
    std::vector<int> dist_map(num_vertices);
    
    reachable[barrack] = true;

    // create a distance map from each barrack to all the nodes in the graph
    boost::dijkstra_shortest_paths(g, barrack,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, g))));
      
    // check for each vertice if it is reachable  
    for (int j = 0; j < num_vertices; j++) {
      if (!reachable[j] && dist_map[j] <= d) {
        reachable[j] = true;
      }
    }
  }
  
  // 3) clear unreachable vertices
  for (int i = 0; i < num_vertices; i++) {
    if (!reachable[i]) {
      boost::clear_vertex(i, g);
    }
  }
  
  // 4) calculate the maximum matching of the graph
  std::vector< boost::graph_traits< graph >::vertex_descriptor > mate(num_vertices);
  boost::checked_edmonds_maximum_cardinality_matching(g, &mate[0]);
  int matching_size = boost::matching_size(g, &mate[0]);
  
  std::cout << matching_size << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for(int i = 0; i < t; i++) {
    solve();
  }
}