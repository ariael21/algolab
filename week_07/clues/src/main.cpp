#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EPIC_K;

// add delaunay triangulation with extra information
typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned, EPIC_K>   Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                        Tds;
typedef CGAL::Delaunay_triangulation_2<EPIC_K, Tds>                     DelaunayTriangulation;
typedef DelaunayTriangulation::Vertex_handle                            Vertex_handle;

typedef boost::adjacency_list<
  boost::vecS, 
  boost::vecS, 
  boost::undirectedS
    > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
typedef boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map;

void solve() {
  int n, m, r;
  std::cin >> n >> m >> r;
  
  std::vector<std::vector<int>> stations(n, std::vector<int>(2, -1));
  
  std::vector<std::pair<EPIC_K::Point_2, unsigned>> station_points;
  station_points.reserve(n);
  
  double squared_radius = r * r;
  
  // create a graph with n vertices -> one for each station
  Graph connected_stations_graph(n);
  
  for (int i = 0; i < n; i++) {
    int x_i, y_i;
    std::cin >> x_i >> y_i;
    
    stations[i][0] = x_i;
    stations[i][1] = y_i;
    
    EPIC_K::Point_2 p_i(x_i, y_i);
    
    // add points for triangulation
    station_points.push_back(std::make_pair(p_i, i));
    
    for (int j = 0; j < i; j++) {
      // check for intersections between 
      // -> current position (x_i, y_i) 
      // -> previously stored positions (x_j, y_j)

      int x_j = stations[j][0];
      int y_j = stations[j][1];
      
      EPIC_K::Point_2 p_j(x_j, y_j);
    
      // check if squred distance of (x_i, y_i) to (x_j, y_j) is smaller equal to r^2 
      double squared_distance = CGAL::squared_distance(p_i, p_j);
      
      if (squared_distance <= squared_radius) {
        // the two points are within reach of eachother
        // they are connected
        boost::add_edge(i, j, connected_stations_graph);
      }
    }
  }
  
  // create triangulation
  DelaunayTriangulation triangulation;
  
  // calculate the graph coloring
  // TODO:

  bool two_coloring = false;
  
  // if (num_colors <= 2) {
    two_coloring = true;
  // }
  
  // connected components
  std::vector<int> component(num_vertices(connected_stations_graph));
  
  if (two_coloring) {
    // calculate the connected components
    int component_number = boost::connected_components(connected_stations_graph, &component[0]);
    
    // calculate the triangulation
    triangulation.insert(station_points.begin(), station_points.end());
  }
  
  // check if holmes and watson can communicate
  for (int i = 0; i < m; i++) {
    int h_x_i, h_y_i, w_x_i, w_y_i;
    
    std::cin >> h_x_i >> h_y_i >> w_x_i >> w_y_i;
    
    // check if they can directly communicate
    EPIC_K::Point_2 h(h_x_i, h_y_i);
    EPIC_K::Point_2 w(w_x_i, w_y_i);  

    double squared_distance = CGAL::squared_distance(h, w);
    
    if (squared_distance <= squared_radius) {
      std::cout << "y";
    } else if (two_coloring) {
      // check if they are connected to the same connected component
      bool connected_to_same_component = false;
      
      Vertex_handle nearest_holmes = triangulation.nearest_vertex(h);
      Vertex_handle nearest_watson = triangulation.nearest_vertex(w);
      
      if (
        CGAL::squared_distance(h, nearest_holmes->point()) <= squared_radius &&
        CGAL::squared_distance(w, nearest_watson->point()) <= squared_radius &&
        component[nearest_holmes->info()] == component[nearest_watson->info()]
      ) {
        connected_to_same_component = true;
      }
      
      std::cout << (connected_to_same_component ? "y" : "n");
    } else {
      std::cout << "n";
    }
  }
  
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}