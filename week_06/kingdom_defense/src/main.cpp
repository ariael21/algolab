#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int l, p;
  std::cin >> l >> p;
  
  long required_flow = 0;
  
  // create flow graph
  graph G(l);
  
  // create edge adder
  edge_adder adder(G);
  
  // add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  std::vector<int> demands(l);
  
  for (int i = 0; i < l; i++) {
    int available, needed;
    std::cin >> available >> needed;
    
    // store the incoming flow
    demands[i] = available - needed;
  }
  
  for (int i = 0; i < p; i++) {
    int from, to, minimum, maximum;
    std::cin >> from >> to >> minimum >> maximum;
    
    // add edge with max capacity
    if (maximum - minimum != 0) {
      adder.add_edge(from, to, maximum - minimum); 
    }
    
    // add from source to to an edge with capacity minimum
    demands[to] += minimum;
    
    // update the incoming flow to vertice from: subtract minimum
    demands[from] -= minimum;
  }
  
  for (int i = 0; i < l; i++) {
    if (demands[i] > 0) {
      // if the demand is positive => add an edge from the source
      adder.add_edge(v_source, i, demands[i]);
    } else if (demands[i] < 0) {
      // if the demand is negative => add edge to sink
      adder.add_edge(i, v_sink, -demands[i]);
      
      // increase the required flow by the demand
      required_flow += -demands[i];
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  std::cout << (required_flow == flow ? "yes" : "no" ) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}