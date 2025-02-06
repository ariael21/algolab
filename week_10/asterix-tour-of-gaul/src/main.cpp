///
#include <iostream>
#include<boost/graph/graphviz.hpp>

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
typedef traits::vertex_descriptor vertex_desc;

const int COST_SHIFT = 100;

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}familys_size_k
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
  int n, m;
  std::cin >> n >> m;
  
  int N = n + 2;
  
  graph G(N);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  int v_source = N-2;
  int v_sink = N-1;
  
  int total_transported = 0;
  
  int c;
  int prev_c = 0;
  for (int i = 0; i < n-1; i++) {
    std::cin >> c;

    if (i == 0) {
      adder.add_edge(v_source, i, c, 0);
    }

    total_transported += c;
    
    adder.add_edge(i, i+1, c, COST_SHIFT);
    
    if (i != 0 && prev_c != c) {
      int difference = std::abs(prev_c - c);
      
      if (prev_c < c) {
        // increase capacity
        adder.add_edge(v_source, i, difference, 0);
      } else {
        // decrease capacity
        adder.add_edge(i, v_sink, difference, 0);
      }
    }
    
    prev_c = c;
  }
  
  adder.add_edge(n-1, v_sink, c, 0);
  
  for (int i = 0; i < m; i++) {
    int a, b, d;
    std::cin >> a >> b >> d;
    
    int cost = (b - a) * COST_SHIFT - d;
    adder.add_edge(a, b, 1, cost);
  }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = boost::find_flow_cost(G);
  
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      flow += c_map[*e] - rc_map[*e];
  }
    
  int corrected_cost = total_transported * COST_SHIFT - cost;   

  std::cout << corrected_cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}