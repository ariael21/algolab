#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

// load exact kernel
// we need to calculate exact intersection points between segments and rays
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;

// define 2D types
typedef Kernel::Point_2 Point;
typedef Kernel::Segment_2 Segment;
typedef Kernel::Ray_2 Ray;

// define rownd down method for FieldNumberType
long round_down(const Kernel::FT number) {
  double double_number = std::floor(CGAL::to_double(number));
  
  while (double_number > number) {
    double_number -= 1;
  }
  
  while (double_number + 1 < number) {
    double_number += 1;
  }
  
  return (long) double_number;
}

// CAUTION:
// we need the 'const' keyword 
Segment get_shortest_segment(const Segment &intersection_segment, const Point &origin) {
  Point source = intersection_segment.source();
  Point target = intersection_segment.target();
  
  Point closer_point;
  
  if (CGAL::has_smaller_distance_to_point(origin, source, target)) {
    closer_point = source;
  } else {
    closer_point = target;
  }
  
  return Segment(origin, closer_point);
}

void solve(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;
  
  Point origin(x, y), point(a, b);
  Ray ray(origin, point);
  
  std::vector<Segment> segments(n);
  
  // 1) load all segments
  for (int i = 0; i < n; i++) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;
    
    Point p1(r, s), p2(t, u);
    Segment segment(p1, p2);
    
    segments.at(i) = segment;
  }
  
  // 2) shuffle input segments
  // - to avoid worse time runtime
  std::random_shuffle(segments.begin(), segments.end());
  
  Segment shortest_segment;
  
  // 3) find first intersection
  // -> then transform ray into segment
  int i = 0;
  for (; i < n; i++) {
    if (CGAL::do_intersect(ray, segments.at(i))) {  // fist check if they intersect at all
      // only then calculate intersection
      auto intersection = CGAL::intersection(ray, segments.at(i));
      
      if (const Point* intersection_point = boost::get<Point>(&*intersection)) {
        Segment first(origin, *intersection_point);
        shortest_segment = first;
        
        break;
      } else if (const Segment* intersection_segment = boost::get<Segment>(&*intersection)) {
        shortest_segment = get_shortest_segment(*intersection_segment, origin);
        
        break;
      } else {
        throw std::runtime_error("strange segment intersection");
      }
    }
  } 
  
  if (i == n) {
    // check if any intersection occured
    // if not:
    std::cout << "no" << std::endl;
  } else {
    // 4) check for closer obstacles
    // transform ray into shorter segment
    // - if any object interesects which shorter segment -> obstacle is closer
    for (; i < n; i++) {
      if (CGAL::do_intersect(shortest_segment, segments.at(i))) {
        auto intersection = CGAL::intersection(ray, segments.at(i));
        
        if (const Point* intersection_point = boost::get<Point>(&*intersection)) {
          Segment shorter(origin, *intersection_point);
          shortest_segment = shorter;
          
        } else if (const Segment* intersection_segment = boost::get<Segment>(&*intersection)) {
          shortest_segment = get_shortest_segment(*intersection_segment, origin);
          
        } else {
          throw std::runtime_error("strange segment intersection");
        }
      }
    }
    
    Point end = shortest_segment.target();
    
    // 5) rownd down to next value
    // CAUTION: needs long
    long x = round_down(end.x());
    long y = round_down(end.y());
    
    std::cout << x << " " << y << std::endl;
  }
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