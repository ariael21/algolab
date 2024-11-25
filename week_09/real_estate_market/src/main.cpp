///4
#include <iostream>
#include <vector>

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

typedef traits::vertex_descriptor vertex_desc;
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

void solve() {
  // approach: max-flow-min-cost
  
  const int max_bid = 100;
  
  int N, M, S;
  std::cin >> N >> M >> S;
  
  // create graph
  int num_nodes = N + M + S;
  
  graph G(num_nodes);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  std::vector<int> state_vertices(S, -1); 
  
  for (int i = 0; i < S; i++) {
    int l_i;
    std::cin >> l_i;
    
    int state_vertex = i;
    
    // add edge: source, target, capacity, cost
    //  - capacity of l_i (= maximum amount of properties that can be bought per state)
    adder.add_edge(state_vertex, v_sink, l_i, 0);
    
    // store the vertex index for each state
    state_vertices[state_vertex] = state_vertex; // store state vertices
  }
  
  std::vector<int> property_vertices(M, -1);
  
  for (int i = 0; i < M; i++) {
    int s_i;
    std::cin >> s_i;
    
    int property_vertex = i + S;
    int state_vertex = state_vertices[s_i - 1]; // load state vertices
    
    // from property to state
    adder.add_edge(property_vertex, state_vertex, 1, 0);
    
    property_vertices[i] = property_vertex; // store property vertices
  }
  
  for (int i = 0; i < N; i++) {
    int buyer_vertex = i + S + M;
    
    // from source to buyer
    adder.add_edge(v_source, buyer_vertex, 1, 0);
    
    for (int j = 0; j < M; j++) {
      int b_i_j;
      std::cin >> b_i_j;
      
      int property_vertex = property_vertices[j]; // load property vertices
      
      // from buyer to property
      // negate price to transform max-flow-max-cost to max-flow-min-cost problem
      // to avoid negative costs => add max_bid
      adder.add_edge(buyer_vertex, property_vertex, 1, max_bid - b_i_j);
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = boost::find_flow_cost(G);
  
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      flow += c_map[*e] - rc_map[*e];     
  }
  
  // remove max_bid of each used edge
  cost -= flow * max_bid;
  
  std::cout << flow << " " << (cost == 0 ? 0 : -cost) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}