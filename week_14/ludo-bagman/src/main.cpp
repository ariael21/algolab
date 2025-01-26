#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int get_e_node(int team_index) {
  return team_index * 3;
}

int get_w_node(int team_index, int e) {
  return (e + team_index) * 3;
}

void solve() {
  int e, w, m, d, p, l;
  std::cin >> e >> w >> m >> d;
  std::cin >> p >> l;
  
  int n = e + 2*e + w + 2*w + 4;
  
  graph G(n);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  int sink_node = n-1;
  int source_node = n-2;
  int E_node = n-3;
  int W_node = n-4;
  
  for (int i = 0; i < e; i++) {
    int e_node = get_e_node(i);
    int d_node = e_node + 1;
    int n_node = e_node + 2;
    
    adder.add_edge(E_node, e_node, p, 0);
    adder.add_edge(e_node, d_node, p, 0);
    adder.add_edge(e_node, n_node, p, 0);
    
    adder.add_edge(source_node, n_node, l, 0);
  }
  
  int total_in_flow = p - (e * l);
  adder.add_edge(source_node, E_node, total_in_flow, 0);
  
  for (int i = 0; i < w; i++) {
    int w_node = get_w_node(i, e);
    int d_node = w_node + 1;
    int n_node = w_node + 2;
    
    adder.add_edge(w_node, W_node, p, 0);
    adder.add_edge(d_node, w_node, p, 0);
    adder.add_edge(n_node, w_node, p, 0);
    
    adder.add_edge(n_node, sink_node, l, 0);
  }
  
  int total_out_flow = p - (w * l);
  adder.add_edge(W_node, sink_node, total_out_flow, 0);
  
  for (int i = 0; i < m; i++) {
    // non-difficult matches
    int u, v, r;
    std::cin >> u >> v >> r;
    
    int e_node = get_e_node(u);
    int w_node = get_w_node(v, e);
    
    // get n-nodes of each
    int e_n_node = e_node + 2;
    int w_n_node = w_node + 2;
    
    adder.add_edge(e_n_node, w_n_node, 1, r);
  }
  
  for (int i = 0; i < d; i++) {
    // difficult matches
    int u, v, r;
    std::cin >> u >> v >> r;
    
    int e_node = get_e_node(u);
    int w_node = get_w_node(v, e);
    
    // get n-nodes of each
    int e_d_node = e_node + 1;
    int w_d_node = w_node + 1;
    
    adder.add_edge(e_d_node, w_d_node, 1, r);
  }
  
  if (m < e*l || m < w*l) {
    std::cout << "No schedule!" << std::endl;
    return;
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source_node, sink_node);

  int flow = 0;
  out_edge_it edge, eend;
  
  for (boost::tie(edge, eend) = boost::out_edges(boost::vertex(source_node,G), G); edge != eend; ++edge) {
      flow += c_map[*edge] - rc_map[*edge];     
  }

  if (flow != p) {
    std::cout << "No schedule!" << std::endl;
  } else {
    int cost = boost::find_flow_cost(G);
    
    std::cout << cost << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}