///
#include <iostream>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

typedef CGAL::Gmpq Fraction;

typedef std::pair<long, int> Pair_L_I;

void solve() {
  int n;
  std::cin >> n;
  
  std::vector<Fraction> gradients(n);
  std::vector<Pair_L_I> start_ids(n);
  std::vector<Point> starts(n);
  std::vector<Segment> segments(n);
  std::vector<int> direction(n);
  
  for (int i = 0; i < n; i++) {
    long y0, x, y;
    std::cin >> y0 >> x >> y;
    
    // (0, y0) - (x, y)
    long delta_x = x; 
    long delta_y = y - y0;
    
    Fraction gradient = CGAL::Gmpq(delta_x, delta_y);
    gradients[i] = gradient;
    
    if (delta_y == 0) {
      direction[i] = 0; // straight
    } else if (gradient < 0) {
      direction[i] = -1; // down
    } else {
      direction[i] = 1; // up
    }
    
    // map start location to id
    Pair_L_I start_id = Pair_L_I(y0, i);
    start_ids[i] = start_id;
    
    // add segment
    Point p0 = Point(0, y0);
    Point p1 = Point(x, y);
    
    Segment segment = Segment(p0, p1);
    segments[i] = segment;
    
    starts[i] = p0;
  }
  
  std::sort(start_ids.begin(), start_ids.end(),
    [](const Pair_L_I& e1, const Pair_L_I& e2) -> bool {
        return e1.first > e2.first;
  });

  std::vector<bool> riding(n, true);

  // check bikers going up
  bool found = false;
  int top_index = 0;
  for (int i = 0; i < n; i++) {
    if (direction[start_ids[i].second] > -1) {
      top_index = i;
      found = true;
      break;
    }
  }
  
  if (found) {
    int top_biker_index = start_ids[top_index].second;
    Fraction max = gradients[top_biker_index];
    
    int max_fraction_index = top_index;
    
    for (int i = top_index+1; i < n; i++) {
      int index = start_ids[i].second;
    
      
      if (direction[start_ids[i].second] > -1) {
        // check if this slope is the same or less
        Fraction current_gradient = gradients[index];
        
        if (current_gradient >= max) {
          max = current_gradient;
          max_fraction_index = index;
        } else {
          riding[index] = false;
        }
      } 
    }
  }
  
  for (int i = 0; i < n; i++) {
    if (riding[i]) {
      std::cout << i << " ";
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