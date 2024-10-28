#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel epic;

void solve(int n) {
  bool intersected = false;
  
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;
  
  // create ray
  epic::Point_2 origin(x, y), ray_point(a, b);
  epic::Ray_2 ray(origin, ray_point);
  
  for (int i = 0; i < n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;
    
    if (!intersected) { 
      // cannot escape the for loop, as we have to read the input values
      
      // create current segment
      epic::Point_2 p_1(r, s), p_2(t, u);
      epic::Segment_2 segment(p_1, p_2);
      
      // check if they intersect
      if (CGAL::do_intersect(ray, segment)) {
        std::cout << "yes" << std::endl;
        intersected = true;
      } 
    }
  }
  
  if (!intersected) {
     std::cout << "no" << std::endl; 
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  
  // solve while not done
  while(n != 0) {
    solve(n);
    std::cin >> n;
  }
}