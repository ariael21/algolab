#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include <boost/graph/max_cardinality_matching.hpp>

// create an undirected graph type
typedef boost::adjacency_list<
  boost::vecS,          // stores edges as std::vector
  boost::vecS,          // stores vertices as std::vector
  boost::undirectedS,   // specify the graph to be undirected
  boost::no_property    // vertex property (none)
    > Graph;

typedef boost::graph_traits<Graph>::edge_descriptor Edge; 
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

void solve() {
  int n, c, f; std::cin >> n >> c >> f;
  
  // 1) 
  // - map interests to unique integer
  // - store each interest for each person
  std::unordered_map<std::string, int> dictionary;
  std::vector<std::unordered_set<int>> vertex_interests(n);
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      std::string interest; 
      std::cin >> interest;
      
      int value;
      
      std::unordered_map<std::string, int>::iterator iterator = dictionary.find(interest);
      
      if (iterator == dictionary.end()) {
        value = dictionary.size() + 1;
        dictionary.insert({ interest, value });
      } else {
        value = iterator->second;
      }
      
      // store interest value for person i
      vertex_interests.at(i).insert(value);
    }
  }
  
  // 2) initialize graph
  Graph g(n);
  
  for (int u = 0; u < n; u++) {
    for (int v = u + 1; v < n; v++) {
      // check how many matches (u, v) have
      int matches = 0;
      
      if (matches <= f) {  // only do it if we have not enough matches
        for (int interest_u : vertex_interests.at(u)) {
          if (vertex_interests.at(v).find(interest_u) != vertex_interests.at(v).end()) {
            // u and v have a matching interest
            matches += 1;
          }
        }        
      }
      
      // if they have more than f matches -> add edge
      if (matches > f) {
        boost::add_edge(u, v, g);
      }
    }
  }
  
  // 3) find maximum cardinality matching
  std::vector<Vertex> mate_map(n);
  
  boost::edmonds_maximum_cardinality_matching(
    g, 
    boost::make_iterator_property_map(
      mate_map.begin(), 
      boost::get(boost::vertex_index, g)
    )
  );
    
  int matching_size = boost::matching_size(
    g,
    boost::make_iterator_property_map(
      mate_map.begin(),
      boost::get(boost::vertex_index, g)
    )
  );
  
  if (n / 2 > matching_size) {
    std::cout << "optimal" << std::endl;
  } else {
    std::cout << "not optimal" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}