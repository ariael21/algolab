///4
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

#include <boost/pending/disjoint_sets.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                 Triangulation;
typedef Triangulation::Point                                        Point;
typedef Triangulation::Vertex_handle                                Vertex_handle;
typedef Triangulation::Edge_iterator                                Edge_iterator;
typedef Triangulation::Face_handle                                  Face_handle;

typedef std::tuple<int,int,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

int num_families_size_k(std::vector<int>& components_size, int k) {
  switch(k) {
    case 4: {
      const int m3w1 = std::min(components_size[2], components_size[0]);
      const int r3 = components_size[2] - m3w1;
      const int r1 = components_size[0] - m3w1;
      return components_size[3] + m3w1 + ((components_size[1] + r3))/2 + ((((components_size[1] + r3) & 1) * 2) + r1)/4;
    }
    case 3: {
      const int m2w1 = std::min(components_size[1], components_size[0]);
      const int two = (components_size[1]-m2w1)/2;
      const int one = (components_size[0]-m2w1)/3;
      return components_size[3] + components_size[2] + m2w1 + two + one;
    }
    case 2: {
      return components_size[3] + components_size[2] + components_size[1] + components_size[0]/2;
    }
    default: {
      return components_size[3] + components_size[2] + components_size[1] + components_size[0];
    }
  }
}

void solve() {
  // read input
  int n, k, f_0;
  long s_0;
  
  std::cin >> n >> k >> f_0 >> s_0;
  
  std::vector<std::pair<Point, int>> tents;
  tents.reserve(n);
  
  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    
    tents.push_back(std::make_pair(Point(x, y), i));
  }
  
  Triangulation triangulation;
  triangulation.insert(tents.begin(), tents.end());
  
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = triangulation.finite_edges_begin(); e != triangulation.finite_edges_end(); ++e) {
    int i1 = e->first->vertex((e->second+1)%3)->info();
    int i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller int comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, triangulation.segment(e).squared_length());
  }
  
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
  });
  
  
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  std::map<int, int> component_size_map;
  
  for (int i = 0; i < n; i++) {
    component_size_map.insert(std::make_pair(i, 1));
  }
  
  std::vector<int> components_size(4, 0);
  components_size[0] = n;
  
  int f = 1;
  long s = 0;
  
  s = std::get<2>(edges[0]);
  f = s_0 <= std::get<2>(edges[0]) ? n/k : 1;
  
  long squared_distance = std::get<2>(edges[0]);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    int c1 = uf.find_set(std::get<0>(*e));
    int c2 = uf.find_set(std::get<1>(*e));
    
    long current_squared_distance = std::get<2>(*e);
    if (c1 != c2) {
      int familys_size_k = num_families_size_k(components_size, k);
      
      if (f_0 == familys_size_k) {
        s = std::max(s, current_squared_distance);
      }
      
      if (s_0 <= current_squared_distance) {
        f = std::max(f, familys_size_k);
      }
      
      int component_size_c1 = component_size_map[c1];
      int component_size_c2 = component_size_map[c2];
      int combined_size = component_size_c1 + component_size_c2;
      
      component_size_map[c1] = 0;
      component_size_map[c2] = 0;
      
      components_size[std::min(component_size_c1, 4) - 1]--;
      components_size[std::min(component_size_c2, 4) - 1]--;
      components_size[std::min(combined_size, 4) - 1]++;
      
      uf.link(c1, c2);
      
      component_size_map[uf.find_set(c1)] = combined_size;
      
      n_components -= 1;

      if (n_components == 1) {
        break;
      }
    }
    
    squared_distance = current_squared_distance;
  }
  
  
  std::cout << s << " " << f << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}