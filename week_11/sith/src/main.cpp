///4
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int ,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef Delaunay::Edge_circulator  Edge_circulator;
typedef Delaunay::Vertex_handle  Vertex_handle;

typedef std::pair<long, long> Coordinate;
typedef std::pair<K::Point_2, int> IPoint;

void solve() {
  int n;
  long r;
  
  std::cin >> n >> r;
  
  long squared_radius = r * r;
  
  int mid = n / 2;
  
  std::vector<std::vector<long>> coordinates(n, std::vector<long>(2));
  
  std::vector<IPoint> points;
  points.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    
    if (i >= mid) {
      points.emplace_back(K::Point_2(x, y), i);
    }
    
    coordinates[i][0] = x;
    coordinates[i][1] = y;
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  std::vector<int> component_size(n, 1);
  boost::disjoint_sets_with_storage<> uf(n);
  
  int biggest_component = 1;
  
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    int i1 = e->first->vertex((e->second+1)%3)->info();
    int i2 = e->first->vertex((e->second+2)%3)->info();
    
    if (i1 >= mid && i2 >= mid) {
      int c1 = uf.find_set(i1);
      int c2 = uf.find_set(i2);

      if (c1 != c2 && t.segment(e).squared_length() <= squared_radius) {
        
        int size1 = component_size[c1];
        int size2 = component_size[c2];
        
        component_size[c1] = 0;
        component_size[c2] = 0;
          
        // connect components
        uf.link(c1, c2);
        
        c1 = uf.find_set(i1);
        component_size[c1] = size1 + size2;
        
        biggest_component = std::max(biggest_component, std::min(mid, size1 + size2));
      }
    }
  }
  
  for (int i = mid-1; i >= 0; i--) {
    Vertex_handle v = t.insert(K::Point_2(coordinates[i][0], coordinates[i][1]));
    v->info() = i;

    Edge_circulator edge = t.incident_edges(v);
    Edge_circulator start = edge;

    do {
      if (t.segment(edge).squared_length() <= squared_radius) {
        int i1 = edge->first->vertex((edge->second+1)%3)->info();
        int i2 = edge->first->vertex((edge->second+2)%3)->info();

        if (i1 >= i && i2 >= i) {
          int c1 = uf.find_set(i1);
          int c2 = uf.find_set(i2);
          
          if (c1 != c2) {
            int size1 = component_size[c1];
            int size2 = component_size[c2];
            
            component_size[c1] = 0;
            component_size[c2] = 0;
              
            // connect components
            uf.link(c1, c2);
            
            c1 = uf.find_set(i1);
            component_size[c1] = size1 + size2;

            biggest_component = std::max(biggest_component, std::min(i, size1 + size2));
          }
        }
      }
      
      edge++;
    } while (start != edge);
    
  }
  
  std::cout << biggest_component << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}