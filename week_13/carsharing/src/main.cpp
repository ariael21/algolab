#include <iostream>
#include <vector>
#include <map>
#include <utility>

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

const int cost_shift = 100;
const int max_cars = 100 * 10;
const int max_time = 100000;
const int min_time = 0;

void solve() {
  int N, S;
  std::cin >> N >> S;
  
  std::vector<int> l(S);
  
  for (int i = 0; i < S; i++) {
    int l_i;
    std::cin >> l_i;
    l[i] = l_i;
  }
  
  std::vector<std::map<int, int>> time_index_map(S);
  std::vector<std::vector<int>> booking(N, std::vector<int>(5));
  
  for (int i = 0; i < N; i++) {
    int s, t, d, a, p;
    std::cin >> s >> t >> d >> a >> p;
    
    time_index_map[s-1].emplace(std::make_pair(d, 0));
    time_index_map[s-1].emplace(std::make_pair(a, 0));
    
    time_index_map[t-1].emplace(std::make_pair(a, 0));
    time_index_map[t-1].emplace(std::make_pair(d, 0));
    
    booking[i][0] = s-1;
    booking[i][1] = t-1;
    booking[i][2] = d;
    booking[i][3] = a;
    booking[i][4] = p;
  }
  
  // add start and end node for all stations: (start at time 0, end at time 100'000)
  for (int i = 0; i < S; i++) {
    time_index_map[i].emplace(std::make_pair(min_time, 0));
    time_index_map[i].emplace(std::make_pair(max_time, 0));
  }
  
  // add ids to all the location-time-nodes
  int id = 0;
  for (int i = 0; i < S; i++) {
    for (auto it = time_index_map[i].begin(); it != time_index_map[i].end(); ++it) {
      time_index_map[i][it->first] = id;
      id++;
    }
  }
  
  int num_nodes = id;
  
  graph G(num_nodes);
  edge_adder adder(G);  
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int i = 0; i < N; i++) {
    int s_id = time_index_map[booking[i][0]][booking[i][2]];
    int t_id = time_index_map[booking[i][1]][booking[i][3]];
    int p = booking[i][4];
    
    int time_diff = booking[i][3] - booking[i][2];
    
    int cost = (cost_shift * time_diff) -p;
    
    adder.add_edge(s_id, t_id, 1, cost);
  }
  
  for (int i = 0; i < S; i++) {
    int size = time_index_map[i].size();
    
    int j = 0;
    int prev_node = -1;
    int prev_time = -1;
    
    for (auto it = time_index_map[i].begin(); it != time_index_map[i].end(); ++it) {
      int node = time_index_map[i][it->first];
      int current_time = it->first;
      
      if (j == 0) {
        // connect source to current node
        int initial_cars = l[i];
        
        adder.add_edge(v_source, node, initial_cars, 0); 
      } else {
        int time_diff = current_time - prev_time;
        int cost = time_diff * cost_shift;
        
        // connect this node to the next one
        adder.add_edge(prev_node, node, max_cars, cost);
      }
      
      if (j == size-1) {
        // connect current node to sink
        adder.add_edge(node, v_sink, max_cars, 0);  
      } 
      
      j++;
      prev_node = node;
      prev_time = current_time;
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost2 = boost::find_flow_cost(G);
  
  // Iterate over all edges leaving the source to sum up the flow values.
  int s_flow = 0;
  
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  int corrected_cost = -(cost2 - (s_flow * (max_time * cost_shift)));
  
  std::cout << corrected_cost << std::endl;
}

int main() {
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
    
    // // std::cout << "---" << std::endl;
  }
}