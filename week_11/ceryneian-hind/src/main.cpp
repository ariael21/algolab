#include <iostream>
#include <vector>
#include <limits>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

const long MAX_LONG = std::numeric_limits<long>::max();

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
  int n, m;
  std::cin >> n >> m;
  
  std::vector<int> convenience_score(n);
  
  for (int i = 0; i < n; i++) {
    int s_i;
    std::cin >> s_i;
    
    convenience_score[i] = s_i;
  }
  
  graph G(n);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    
    adder.add_edge(u, v, MAX_LONG);
  }
  
  int total_in_flow = 0;
  
  for (int i = 0; i < n; i++) {
    if (convenience_score[i] < 0) {
      adder.add_edge(i, v_sink, -convenience_score[i]);
    } else {
      adder.add_edge(v_source, i, convenience_score[i]);
      total_in_flow += convenience_score[i];
    } 
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  long max_convenience_score = total_in_flow - flow;
  
  if (max_convenience_score > 0) {
    std::cout << max_convenience_score << std::endl;
  } else {
    std::cout << "impossible" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    // std::cout << ".." << std::endl;
    solve();
  }
}