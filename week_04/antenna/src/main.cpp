#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Min_circle_2_traits_2<Kernel>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef Kernel::Point_2 Point;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt SQRT_Kernel;
typedef SQRT_Kernel::Point_2 SQRT_Point;
typedef SQRT_Kernel::Circle_2  SQRT_Circle;

void solve(int n) {
  std::vector<Point> points;
  points.reserve(n); // makes it faster
  
  // 1) read all input points
  for (int i = 0; i < n; i++) {
    long x, y; 
    std::cin >> y >> x;

    // emplace_back is faster than .at(i) = 
    // .at(i) adds bounds checks, which is an additional overhead
    points.emplace_back(x, y);
  }
  
  std::random_shuffle(points.begin(), points.end());
  
  if (n == 1) {
    std::cout << 0 << std::endl;
    
    return;
  } 
  // 2) calculate smallest enclosing circle
  Min_circle smallest_circle(points.begin(), points.end(), true);
  
  std::vector<SQRT_Point> sqrt_support_points;
  
  // 3) get support points of this circle and then calculate the exact radius
  for (auto it = smallest_circle.support_points_begin(); it != smallest_circle.support_points_end(); it++) {
    SQRT_Point point(it->x(), it->y());
    
    sqrt_support_points.push_back(point);
  }
  
  // construct exact circle
  SQRT_Circle sqrt_circle;
  
  if (sqrt_support_points.size() == 3) {
    sqrt_circle = SQRT_Circle(sqrt_support_points.at(0), sqrt_support_points.at(1), sqrt_support_points.at(2));
  } else if (sqrt_support_points.size() == 2) {
    sqrt_circle = SQRT_Circle(sqrt_support_points.at(0), sqrt_support_points.at(1));
  } else {
    std::cout << 0 << std::endl;
    return;
  }
  
  // 4) calculate radius
  auto sqrt = CGAL::sqrt(sqrt_circle.squared_radius());
  double ceiled = std::ceil(CGAL::to_double(sqrt));
  
  while (ceiled > sqrt) {
    ceiled -= 1;
  }
  
  while (ceiled < sqrt) {
    ceiled += 1;
  }
  
  int64_t result = (int64_t) ceiled;
  
  std::cout << result << std::endl;
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