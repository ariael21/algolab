///1
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int n, m;
  std::cin >> n >> m;
  
  int num_vertices = n + m;
  
  // initialize graph
  graph G(num_vertices);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // graph structure:
  // 0 ... m - 1 -> matches
  // m ... m + n -> players
  
  for (int i = 0; i < m; i++) {
    int a, b, c;
    std::cin >> a >> b >> c;
    
    // add edge from source to current match
    adder.add_edge(v_source, i, 1);
    
    if (c == 0) {
      // edge from match to player a
      adder.add_edge(i, m + a, 1);
      // edge from match to player b
      adder.add_edge(i, m + b, 1);
    } else if (c == 1) {
      adder.add_edge(i, m + a, 1);
    } else if (c == 2) {
      adder.add_edge(i, m + b, 1);
    }
  }
  
  int expected_flow = 0;
  
  for (int i = 0; i < n; i++) {
    int s_i;
    std::cin >> s_i;
    
    expected_flow += s_i;
    
    // add edge from i-th vertex to sink with capactiy s_i
    adder.add_edge(m + i, v_sink, s_i);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  // make sure to check the necessary things
  std::cout << ((flow == m && flow == expected_flow) ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}