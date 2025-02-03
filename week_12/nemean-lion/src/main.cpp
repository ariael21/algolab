///4
#include <iostream>
#include <map>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef CGAL::Circle_2<K>                                       Circle;
typedef K::Point_2 Point;

typedef Delaunay::Face_handle                                   Face_handle;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<int,int,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<Point,int> IPoint;

typedef std::pair<K::FT, K::FT> Interval;
typedef std::vector<Interval> IntervalV;
typedef std::set<K::FT> dSet;


typedef std::tuple<int,int,int, K::FT> Face;
typedef std::vector<Face> FaceV;

const double inf = std::numeric_limits<double>::max();

int compose_id(int id1, int id2, int n) {
  return std::max(id1, id2) * n + std::min(id1, id2);
}

void solve() {
  int n, h; long s;
  std::cin >> n >> s >> h;

  int a_2 = 0;
  int a_3 = 0;
  int a_s = 0;
  int a_max = h;
  
  std::vector<IPoint> points(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    
    points[i] = IPoint(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  FaceV faces;
  faces.reserve(t.number_of_faces());
  
  std::map<int, K::FT> min_d;
  std::map<int, K::FT> max_d;
  
  dSet distinct_d;
  
  IntervalV intervals;
  intervals.reserve(3 * n); 
  
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
    auto v1 = f->vertex(0);
    auto v2 = f->vertex(1);
    auto v3 = f->vertex(2);

    Point center = t.dual(f);
    
    // calculate the distances for a_3
    K::FT face_distance = CGAL::squared_distance(center, f->vertex(0)->point());
    faces.emplace_back(v1->info(), v2->info(), v3->info(), face_distance);
    
    for (int i = 0; i < 3; i++) {
      int ccw = Delaunay::ccw(i);
      int cw = Delaunay::cw(i);
      
      auto v1 = f->vertex(i);
      auto v2 = f->vertex(cw);
      
      int edge_id = compose_id(v1->info(), v2->info(), n);
      
      if (min_d.find(edge_id) == min_d.end()) {
        // did not calculate the interval for the current edge
        
        auto neighboring_face = f->neighbor(ccw);
        
        Point neighbor_center = t.dual(neighboring_face);
        
        K::FT distance_c1 = CGAL::squared_distance(center, v1->point());
        K::FT distance_c2 = t.is_infinite(neighboring_face) ? inf : CGAL::squared_distance(neighbor_center, v1->point());
        
        K::FT max = std::max(distance_c1, distance_c2);
        K::FT min = std::min(distance_c1, distance_c2);
        
        Point mid = CGAL::midpoint(v1->point(), v2->point());
        Point nearest_mid = t.nearest_vertex(mid)->point();
        
        if (nearest_mid == v1->point() || nearest_mid == v2->point()) {
          // smallest value is the midpoint
          min = CGAL::squared_distance(mid, v1->point());
        }
        
        intervals.push_back(Interval(min, max));
        
        distinct_d.insert(min);
        distinct_d.insert(max);

        min_d.insert({edge_id, min});
        max_d.insert({edge_id, max});
      }
    }
  }
  
  // over all distances: count maximum number of pairwise distinct admissible sets of size two
  std::map<K::FT, int> in_count;
  std::map<K::FT, int> out_count;
  
  for (Interval interval : intervals) {
    in_count[interval.first]++;     // for current min_d -> enter one interval
    out_count[interval.second]++;   // for current max_d -> leave one interval
  }
  
  int in = 0;
  for (K::FT d : distinct_d) {
    in += in_count[d];  // for current d => enter in_count[d] intervals
    a_max = std::max(a_max, in);  // are we currently in more than previously ?
    in -= out_count[d]; // leave this value and go to next one => leave out_count[d] intervals
  }
  
  std::sort(faces.begin(), faces.end(),
      [](const Face& e1, const Face& e2) -> bool {
        return std::get<3>(e1) < std::get<3>(e2);
  });
  
  Face first_face = faces[0];
  K::FT smallest_d_3 = std::get<3>(first_face);
  
  for (FaceV::const_iterator f = faces.begin(); f != faces.end(); ++f) {
    if (std::get<3>(*f) == smallest_d_3) {
      a_3++;  // count how many faces have the same (smallest) distance d
    } else {
      break;
    }
  }
  
  EdgeV edges;
  edges.reserve(3*n);
  
  K::FT s_exact(s);
  
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    int i1 = e->first->vertex((e->second+1)%3)->info();
    int i2 = e->first->vertex((e->second+2)%3)->info();
    
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
    
    int edge_id = compose_id(i1, i2, n);
    
    if (min_d.at(edge_id) <= s_exact && s_exact <= max_d.at(edge_id)) {
      a_s++; // how many admissible sets can have distance sqrt(s)
    }
  }
  
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
  });
  
  Edge first = edges[0];
  auto smallest_d = std::get<2>(first);
  
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    if (std::get<2>(*e) == smallest_d) {
      a_2++;  // count how many edges have the same (smallest) distance d
    } else {
      break;
    }
  }
  
  std::cout << a_2 << " " << a_3 << " " << a_s << " " << a_max << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}