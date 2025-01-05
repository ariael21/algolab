///
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

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Vertex_handle                           Vertex_handle; 

typedef std::tuple<Index,Index, long> Edge;
typedef std::pair<long, int> Distance_Vertex_Pair;
typedef std::vector<Edge> EdgeV;
typedef K::Point_2 Point;
typedef std::pair<Point, Index> IPoint;

void solve() {
  int n, m, k;
  long s;
  
  std::cin >> n >> m >> s >> k;
  
  std::vector<IPoint> trees;
  trees.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    
    trees.emplace_back(K::Point_2(x, y), i);
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  EdgeV edges;
  edges.reserve(3*n);
  
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  
  std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
      return std::get<2>(e1) < std::get<2>(e2);
  });
  
  std::vector<Distance_Vertex_Pair> distances_bone_tree;
  distances_bone_tree.reserve(m);
  
  for (int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    Point p = Point(x, y);
    
    Vertex_handle nearest_tree = t.nearest_vertex(p);
    long squared_distance = CGAL::squared_distance(p, nearest_tree->point());
    
    distances_bone_tree.emplace_back(4 * squared_distance, nearest_tree->info());
  }
  
  std::sort(distances_bone_tree.begin(), distances_bone_tree.end(), [](const Distance_Vertex_Pair& p1, const Distance_Vertex_Pair& p2) -> bool {
      return p1.first < p2.first;
  });
  
  int edges_amount = edges.size();
  
  int distances_iterator = 0;
  int edges_iterator = 0;
  
  // initialize union find datastructure
  boost::disjoint_sets_with_storage<> uf(n);
  std::vector<int> component_bones(n);
  
  int max_bones = 0;
  long previous_radius = 0;
  
  bool set_a = false;
  bool set_b = false;
  
  int a = 0;
  long b = 0;
  
  while (distances_iterator < m || edges_iterator < edges_amount) {
    int current_max_bones = 0;
    long current_min_dist = distances_bone_tree[distances_iterator].first;
    long current_min_edge = std::get<2>(edges[edges_iterator]);
    long current_min_radius;
    
    if (edges_iterator == edges_amount) {
      current_min_radius = current_min_dist;
    } else if (distances_iterator == m) {
      current_min_radius = current_min_edge;
    } else {
      current_min_radius = std::min(current_min_dist, current_min_edge);
    }
    
    while (distances_iterator < m && current_min_radius == distances_bone_tree[distances_iterator].first) {
      // bone is within current_min_radius: add one bone to component
      int vertex = distances_bone_tree[distances_iterator].second;
      Index component = uf.find_set(vertex);
    
      component_bones[component] += 1;

      current_max_bones = component_bones[component] > current_max_bones ? component_bones[component] : current_max_bones;
      
      distances_iterator++;
    }
    
    while (edges_iterator < edges_amount && current_min_radius == std::get<2>(edges[edges_iterator])) {
      // two vertices are within reach of each other
      auto e = edges[edges_iterator];
      
      int v0 = std::get<0>(e);
      int v1 = std::get<1>(e);
      
      Index component1 = uf.find_set(v0);
      Index component2 = uf.find_set(v1);
      
      if (component1 != component2) {
        int bones_component1 = component_bones[component1];
        int bones_component2 = component_bones[component2];
        
        uf.link(component1, component2);
        
        Index combined = uf.find_set(v0);
        component_bones[combined] = bones_component1 + bones_component2;
        
        current_max_bones = component_bones[combined] > current_max_bones ? component_bones[combined] : current_max_bones;
      }
      
      edges_iterator++;
    }
    
    if (previous_radius <= s && s < current_min_radius) {
      a = max_bones > a ? max_bones : a;
      set_a = true;
    }
    
    max_bones = current_max_bones > max_bones ? current_max_bones : max_bones;

    if (max_bones >= k && !set_b) {
      b = current_min_radius;
      set_b = true;
    }
    
    if (set_b && set_a) {
      break;
    }
    
    previous_radius = current_min_radius;
  }
  
  std::cout << a << " " << b << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}