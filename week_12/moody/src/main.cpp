///
#include <iostream>
#include <algorithm>

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

typedef traits::vertex_descriptor                               vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
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
    return e;
  }
};

void solve() {
  int n, m, s, p;
  std::cin >> n >> m >> s >> p;
  
  graph G(n+1);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  long inflow = 1;
  
  edge_desc valve = adder.add_edge(v_source, s, inflow, 0);
  
  for (int i = 0; i < m; i++) {
    int u, v, c, d;
    std::cin >> u >> v >> c >> d;
    
    adder.add_edge(u, v, c, d);
    adder.add_edge(v, u, c, d); // CAUTION: Add both directions -> our network is undirected
  }
  
  long max = boost::push_relabel_max_flow(G, s, p);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, p);
  long flow_cost_1 = boost::find_flow_cost(G);
  
  long flow_cost_i = flow_cost_1;
  long step_size = std::max(max / 2, (long)1);
  long prev_flow_cost = flow_cost_1;
  long prev_inflow = inflow;
  
  while (true) {
    // check previous result
    bool previously_increased = false;
    if (prev_flow_cost == flow_cost_1 * prev_inflow) {
      // increase
      previously_increased = true;
      inflow = std::min(prev_inflow + step_size, max);
    } else {
      // decrease
      inflow = std::max(prev_inflow - step_size, (long) 0);
    }
    
    c_map[valve] = inflow; 
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, p);
    flow_cost_i = boost::find_flow_cost(G);
    
    bool increase_next = (flow_cost_i == flow_cost_1 * inflow);
    
    // termination checks
    if (step_size == 1) {
      if (previously_increased && !increase_next) {
          inflow = inflow-1; 
          break;
      }
      
      if (inflow == max) {
        break;
      }
      
      if (!previously_increased && increase_next) {
        break;
      }
    }
    
    step_size = std::max(step_size / 2, (long) 1);
    prev_flow_cost = flow_cost_i;
    prev_inflow = inflow;
  }
  
  std::cout << inflow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}