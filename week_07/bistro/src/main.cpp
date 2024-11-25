#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Delaunay_Triangulation;
typedef Delaunay_Triangulation::Edge_iterator  Edge_iterator;

void solve(int n) {
  std::vector<K::Point_2> restaurants;
  restaurants.reserve(n);
  
  // 1) put all existing restaurant locations in delaunay triangulation
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    
    K::Point_2 p(x,y);
    
    restaurants.push_back(p);
  }
  
  // 2) create delaunay triangulation
  Delaunay_Triangulation triangulation;
  triangulation.insert(restaurants.begin(), restaurants.end());
  
  int m;
  std::cin >> m;
  
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    
    K::Point_2 p(x,y);
    
    // get the nearest vertex
    Delaunay_Triangulation::Vertex_handle closest_vertex = triangulation.nearest_vertex(p);
    
    // calculate the squared distance
    K::FT distance = CGAL::squared_distance(p, closest_vertex->point());
    
    // watch out: right formatting of the output
    std::cout << std::fixed << std::setprecision(0) << distance << std::endl;
  }
}

int main() {
  int n;
  std::cin >> n;
  
  while (n != 0) {
    solve(n);
    std::cin >> n;
  }
}