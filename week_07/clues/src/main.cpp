#include <iostream>
#include <vector>
#include <stack>

#include <boost/graph/bipartite.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel EPIC_K;

// add delaunay triangulation with extra information
typedef CGAL::Triangulation_vertex_base_with_info_2<int, EPIC_K>        Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                        Tds;
typedef CGAL::Delaunay_triangulation_2<EPIC_K, Tds>                     DelaunayTriangulation;
typedef DelaunayTriangulation::Vertex_handle                            Vertex_handle;
typedef DelaunayTriangulation::Vertex_circulator                        Vertex_circulator;

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
  
  double squared_radius = r * r;

  std::vector<std::pair<EPIC_K::Point_2, int>> station_points;
  station_points.reserve(n);
  
  for (int i = 0; i < n; i++) {
    int x_i, y_i; std::cin >> x_i >> y_i;
    
    EPIC_K::Point_2 p_i(x_i, y_i);
    station_points.push_back(std::make_pair(p_i, i));
  }
  
  // create triangulation
  DelaunayTriangulation triangulation;
  triangulation.insert(station_points.begin(), station_points.end());
  
  // create a graph which connects stations which are in reach of each other
  Graph connected_stations_graph(n);
  
  // iterate through the vertices in the triangulation
  std::stack<Vertex_handle> vertex_stack;
  
  for (Vertex_handle vertex : triangulation.finite_vertex_handles()) {
    std::vector<bool> visited(n, false);

    vertex_stack.push(vertex);
  
    while (!vertex_stack.empty()) {
      Vertex_handle current_vertex = vertex_stack.top(); vertex_stack.pop();
      
      if (!visited[current_vertex->info()]) {
        // check if an edge between the two vertices has to be added
        bool same_station = vertex->info() == current_vertex->info();
        bool stations_already_connected = boost::edge(vertex->info(), current_vertex->info(), connected_stations_graph).second;
        
        if (!same_station && !stations_already_connected) {
          boost::add_edge(vertex->info(), current_vertex->info(), connected_stations_graph);
        }
        
        // add the neighbours of the current vertex to the stack
        Vertex_circulator incident_vertices = triangulation.incident_vertices(current_vertex);
        Vertex_circulator start = incident_vertices;
        
        do {
          Vertex_handle neighbour = incident_vertices;
          
          long squared_distance = CGAL::squared_distance(vertex->point(), neighbour->point());
          
          // only add neighbours within reach
          if (!triangulation.is_infinite(neighbour) && squared_distance <= squared_radius) {
            vertex_stack.push(neighbour);
          }
        } while (++incident_vertices != start);

        visited[current_vertex->info()] = true;
      }
    }
  }
  
  // calculate the graph coloring
  bool bipartite = boost::is_bipartite(connected_stations_graph);
  
  // connected components
  std::vector<int> component(num_vertices(connected_stations_graph));
  
  if (bipartite) {
    boost::connected_components(connected_stations_graph, &component[0]);
  }
  
  // check if holmes and watson can communicate
  for (int i = 0; i < m; i++) {
    int h_x_i, h_y_i, w_x_i, w_y_i; std::cin >> h_x_i >> h_y_i >> w_x_i >> w_y_i;

    if (!bipartite) {
      // cannot configure stations properly
      std::cout << "n";
      
    } else {
      // check if they can directly communicate
      EPIC_K::Point_2 h(h_x_i, h_y_i);
      EPIC_K::Point_2 w(w_x_i, w_y_i);  
    
      double squared_distance = CGAL::squared_distance(h, w);
      
      if (squared_distance <= squared_radius) {
        // holmes and watson can directly communicate
        std::cout << "y";
      } else {
        // check if they are connected to the same connected component
        Vertex_handle nearest_holmes = triangulation.nearest_vertex(h);
        Vertex_handle nearest_watson = triangulation.nearest_vertex(w);
        
        bool reachable = CGAL::squared_distance(h, nearest_holmes->point()) <= squared_radius 
                      && CGAL::squared_distance(w, nearest_watson->point()) <= squared_radius;
        bool same_component = component[nearest_holmes->info()] == component[nearest_watson->info()];
        bool can_communicate = reachable && same_component;
        
        std::cout << (can_communicate ? "y" : "n");
      }
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