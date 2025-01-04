#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle; 
typedef Triangulation::Vertex_circulator Vertex_circulator;

typedef std::map<Vertex_handle, bool> Vertex_Boolean;
typedef std::pair<int, int> Distant_Vertex_Pair;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef K::Point_2 Point;
typedef std::pair<Point, Index> IPoint;

const long MAX_LONG = std::numeric_limits<long>::max();

double min_distance_from_dish(int x, int y, int l, int b, int r, int t) {
  double dist_from_top = std::abs(y - t); 
  double dist_from_bottom = std::abs(y - b);
  double dist_from_left = std::abs(x - l); 
  double dist_from_right = std::abs(x - r);
  
  double min_x = std::min(dist_from_left, dist_from_right); 
  double min_y = std::min(dist_from_top, dist_from_bottom);
  
  return std::min(min_x, min_y);
}

double get_radius(int t) {
  return t * t + 0.5;
}

void solve(int n) {
  int l, b, r, t;
  std::cin >> l >> b >> r >> t;
  
  std::vector<IPoint> bacteria_coordinates;
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    
    bacteria_coordinates.emplace_back(Point(x,y), i);
  }
  
  Triangulation triangulation;
  triangulation.insert(bacteria_coordinates.begin(), bacteria_coordinates.end());
  
  std::vector<bool> alive(n);
  std::vector<Distant_Vertex_Pair> dish_distance(n);
  
  for (auto v = triangulation.finite_vertices_begin(); v != triangulation.finite_vertices_end(); ++v) {
    alive[v->info()] = true;
    
    double min = min_distance_from_dish(v->point().x(), v->point().y(), l, b, r, t);
    
    dish_distance[v->info()] = std::make_pair(min, v->info());
  }
  
  std::sort(dish_distance.begin(), dish_distance.end(), 
    [](const Distant_Vertex_Pair& a, const Distant_Vertex_Pair& b) -> bool {
      return a.first < b.first;
    }
  );
  
  std::vector<Edge> edges;
  edges.reserve(3 * n);
  
  for (auto e = triangulation.finite_edges_begin(); e != triangulation.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, triangulation.segment(e).squared_length());
  }
  
  std::sort(edges.begin(), edges.end(),
    [](const Edge& e1, const Edge& e2) -> bool {
      return std::get<2>(e1) < std::get<2>(e2);
    }
  );
  
  int first = -1;
  int mid = -1;
  int last = -1;
  
  int dead_count = 0;
  int current_time = 0;
  
  int edge_iterator = 0;
  int distance_iterator = 0;
  
  while (dead_count != n) {
    double radius = get_radius(current_time);
    double squared_radius = radius * radius;
    double four_times_squared_radius = 4 * squared_radius;
    
    int died = 0;
    
    // 1) if radius >= min_dist => dead
    while (radius >= dish_distance[distance_iterator].first && distance_iterator < n) {
      int v_index = dish_distance[distance_iterator].second;
      
      if (alive[v_index]) {
        alive[v_index] = false;
        died++;
      }
      
      distance_iterator++;
    }
    
    // 2) if 4 * squared_radius >= squared_distance of edge => dead
    while (four_times_squared_radius >= std::get<2>(edges[edge_iterator]) && edge_iterator < edges.size()) {
      int v0_index = std::get<0>(edges[edge_iterator]);
      int v1_index = std::get<1>(edges[edge_iterator]);
      
      if (alive[v0_index]) {
        alive[v0_index] = false;
        died++;
      }
      
      if (alive[v1_index]) {
        alive[v1_index] = false;
        died++;
      }
      
      edge_iterator++;
    }
    
    if (dead_count == 0 && died != 0) {
      first = current_time;
    }
    
    dead_count += died;
    
    if (dead_count > n / 2 && mid == -1) {
      mid = current_time;
    }
    
    current_time++;
  }
  
  last = current_time - 1;
  
  std::cout << first << " " << mid << " " << last << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  std::cin >> n;
  
  while (n != 0) {
    solve(n);
    std::cin >> n;  
  }
}