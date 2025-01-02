#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_handle Face_handle;

typedef std::map<Triangulation::Face_handle, long> Face_Info;

const long MAX_LONG = std::numeric_limits<long>::max();

void solve(int n) {
  // 1) read information
  std::vector<K::Point_2> infected;
  infected.reserve(n);
  
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    infected.push_back(K::Point_2(x, y));
  }
  
  int m;
  std::cin >> m;
  
  std::vector<K::Point_2> healthy;
  std::vector<long> min_d;
  healthy.reserve(m);
  min_d.reserve(m);
  
  for (int i = 0; i < m; i++) {
    int x, y;
    long d;
    std::cin >> x >> y;
    std::cin >> d;
    
    healthy.push_back(K::Point_2(x, y));
    min_d.push_back(d);
  }
  
  // 2) create delauny triangulation
  Triangulation t;
  t.insert(infected.begin(), infected.end());
  
  Face_Info max_d;
  
  std::priority_queue<std::pair<long, Face_handle>> queue;
  
  for (Face_handle face = t.all_faces_begin(); face != t.all_faces_end(); ++face) {
    if (t.is_infinite(face)) {
      max_d[face] = MAX_LONG;
      
      queue.push(std::make_pair(MAX_LONG, face));
    } else {
      max_d[face] = 0;
    }
  }
  
  while (!queue.empty()) {
    Face_handle face = queue.top().second;
    queue.pop();
    
    for (int i = 0; i < 3; i++) {
      // look at all neighbouring faces
      
      int ccw = Triangulation::ccw(i);
      int cw = Triangulation::cw(i);
      
      Face_handle neighbour = face->neighbor(i);
      
      if (!t.is_infinite(neighbour)) {
        // add the neighbour to the priority queue with the edge connecting face to neighbour
        
        auto v1 = face->vertex(ccw);
        auto v2 = face->vertex(cw);
        
        long squared_distance = CGAL::squared_distance(v1->point(), v2->point());
        
        long max_distance = std::min(squared_distance, max_d[face]); 
        
        if (max_distance > max_d[neighbour]) {
          max_d[neighbour] = max_distance;
          
          queue.push(std::make_pair(max_distance, neighbour));
        }
      }
    }
  }
  
  for (int i = 0; i < m; i++) {
    K::Point_2 point = healthy[i];
    long min_distance = min_d[i];
    
    // 1) check if closest vertex is far away
    auto nearest_vertex = t.nearest_vertex(point);
    
    long squared_distance = CGAL::squared_distance(point, nearest_vertex->point());
    
    // 2) check if this point can escape the current face
    auto face = t.locate(point);
    
    bool safe_distance_to_closest_vertex = squared_distance >= min_distance;
    bool can_escape_triangulation = (max_d[face] / 4) >= min_distance;
    
    bool safe = safe_distance_to_closest_vertex && can_escape_triangulation;
    
    std::cout << (safe ? "y" : "n");
  }
  
  std::cout << std::endl;
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