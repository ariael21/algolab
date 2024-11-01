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

int add_exit(int i, int used_exits, int N, int k, const vertex_desc &v_sink, edge_adder &adder) {
  int exit_vertex = N + used_exits;
  adder.add_edge(i, exit_vertex, 1);
  
  // add vertex to sink with capacity k (upper bound)
  // this way each exit could let all knights escape (eventho this would never be the case)
  adder.add_edge(exit_vertex, v_sink, k);
  
  return used_exits + 1;
}

void solve() {
  int m, n, k, C;
  std::cin >> m >> n >> k >> C;
  
  int num_intersections = n * m;    // number of intersections in the grid
  int N = num_intersections * 2;    // total number of vertices in the grid -> duplicate the vertices at each intersection
  int num_exits = 2 * n + 2 * m;    // the total amount of exits
  
  // total number of vertices in the graph
  int num_vertices = N + num_exits;
  
  // construct graph
  graph G(num_vertices);
  edge_adder adder(G);
  
  // add source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  // current coordinates within the grid
  int x = 0;
  int y = 0;
  
  // used to keep track of the already used exits
  int used_exits = 0;
  
  // go trough all intersections and add:
  // 1) in_vertex
  // 2) out_vertex
  // 3) connect in and out vertex with capacity C
  // 4) add all outgoing edges from out_vertex, which are either: (we look at left, top, right and bottom)
  //  4.1) exits
  //  4.2) in_vertex of neighbouring intersections
  for (int i = 0; i < num_intersections; i++) {
    // calculate current position
    x = i % m;
    y = (i - x) / m;
    
    // add connection between in and out vertex
    adder.add_edge(i + num_intersections, i, C);
    
    // handle all 4 sides of the grid
    // 1) left
    if (x == 0) {
      // add exit
      used_exits = add_exit(i, used_exits, N, k, v_sink, adder);
    } else {
      // add connection to left corner
      int left_in_vertex = (i - 1) + num_intersections;
      adder.add_edge(i, left_in_vertex, 1);
    }
    
    // 2) top
    if (y == 0) {
      // add exit
      used_exits = add_exit(i, used_exits, N, k, v_sink, adder);
    } else {
      // add connection to top corner
      int top_in_vertex = (i - m) + num_intersections;
      adder.add_edge(i, top_in_vertex, 1);
    }
    
    // 3) right
    if (x == m-1) {
      // add exit
      used_exits = add_exit(i, used_exits, N, k, v_sink, adder);
    } else {
      // add connection to left corner
      int right_in_vertex = (i + 1) + num_intersections;
      adder.add_edge(i, right_in_vertex, 1);
    }
    
    // 4) bottom
    if (y == n-1) {
      // add exit
      used_exits = add_exit(i, used_exits, N, k, v_sink, adder);
    } else {
      // add connection to top corner
      int bottom_in_vertex = (i + m) + num_intersections;
      adder.add_edge(i, bottom_in_vertex, 1);
    }
  }
  
  // add the starting locations of the knights:
  // - add an edge from the source to the starting location with capacity of 1
  for (int i = 0; i < k; i++) {
    // read starting points
    int x, y;
    std::cin >> x >> y;
    
    // calculate the index of the current in_vertex within the grid
    int in_vertex = (x + y * m) + num_intersections;
    
    adder.add_edge(v_source, in_vertex, 1);
  }
  
  // finally calculate the max flow of the constructed graph
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}