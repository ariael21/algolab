///

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<bool,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::pair<K::Point_2,bool> BPoint;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Coordinate {
    long x;
    long y; 

    Coordinate(long x_val, long y_val)
      : x(x_val), y(y_val) {}
};

struct Warehouse {
  Coordinate coordinate;
  int s;
  int a;
  
  Warehouse(long x_val, long y_val, int s_val, int a_val)
    : coordinate(x_val, y_val), s(s_val), a(a_val) {}
};

struct Stadium {
  Coordinate coordinate;
  int d;
  int u;
  
  Stadium(long x_val, long y_val, int d_val, int u_val)
    : coordinate(x_val, y_val), d(d_val), u(u_val) {}
};

long floor_to_long(const CGAL::Quotient<CGAL::Gmpz>& x)  // Change this
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void solve() {
  int n, m, c;
  std::cin >> n >> m >> c;
  
  std::vector<BPoint> points;
  points.reserve(n + m);
  
  std::vector<Warehouse> warehouses;
  warehouses.reserve(n);
  
  for (int i = 0; i < n; i++) {
    long x, y;
    int s, a;
    std::cin >> x >> y >> s >> a;
    
    warehouses.emplace_back(x, y, s, a);
    
    points.emplace_back(K::Point_2(x, y), true);
  }
  
  std::vector<Stadium> stadiums;
  stadiums.reserve(m);
  
  for (int j = 0; j < m; j++) {
    long x, y;
    int d, u;
    std::cin >> x >> y >> d >> u;
    
    stadiums.emplace_back(x, y, d, u);
    
    points.emplace_back(K::Point_2(x, y), false);
  }
  
  std::vector<std::vector<int>> r(n, std::vector<int>(m));
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int r_i_j;
      std::cin >> r_i_j;
      
      r[i][j] = r_i_j;
    }
  }
  
  std::vector<std::vector<int>> l(n, std::vector<int>(m, 0));
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  std::vector<std::vector<long>> relevant_contours;
  relevant_contours.reserve(c);
  for (auto relevant_contour : relevant_contours) {
    relevant_contour.reserve(3);
  }
  
  for (int i = 0; i < c; i++) {
    long x, y, r;
    std::cin >> x >> y >> r;

    K::Point_2 contour_center = K::Point_2(x, y);
    long squared_radius = r*r;
    
    auto nearest = t.nearest_vertex(contour_center);
    
    long distance = CGAL::squared_distance(nearest->point(), contour_center);
    
    if (distance < squared_radius) {
      // relevant contour line
      std::vector<long> contour_line(3);
      contour_line[0] = x;
      contour_line[1] = y;
      contour_line[2] = squared_radius;
      
      relevant_contours.push_back(contour_line);
    }
  }
  
  Program lp (CGAL::SMALLER, true, 0, false, 0); // LOWER BOUND: makes sense, as we cannot take away beer 
  
  int CONSTRAINT_COUNT = 0;
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int x_i_j = i * m + j;
      lp.set_a(x_i_j, CONSTRAINT_COUNT, 1);
    }
    
    lp.set_b(CONSTRAINT_COUNT, warehouses[i].s);
    CONSTRAINT_COUNT += 1;
  }
  

  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      int x_i_j = i * m + j;
      
      lp.set_a(x_i_j, CONSTRAINT_COUNT, 1);
      lp.set_a(x_i_j, CONSTRAINT_COUNT+1, -1);

      lp.set_a(x_i_j, CONSTRAINT_COUNT+2, warehouses[i].a);
    }
    
    lp.set_b(CONSTRAINT_COUNT, stadiums[j].d);
    lp.set_b(CONSTRAINT_COUNT+1, -stadiums[j].d);
    lp.set_b(CONSTRAINT_COUNT+2, stadiums[j].u * 100);

    CONSTRAINT_COUNT += 3;
  }
  
  for (int i = 0; i < n; i++) {
    K::Point_2 warehouse_point = K::Point_2(warehouses[i].coordinate.x, warehouses[i].coordinate.y);
    
    for (int j = 0; j < m; j++) {
      K::Point_2 stadium_point = K::Point_2(stadiums[j].coordinate.x, stadiums[j].coordinate.y);
      
      int l_i_j = 0;
      
      // for each combination check how many countour lines are crossed
      for (auto contour_line : relevant_contours) {
        K::Point_2 center = K::Point_2(contour_line[0], contour_line[1]);
        
        long distance_warehouse = CGAL::squared_distance(warehouse_point, center);
        long distance_stadium = CGAL::squared_distance(stadium_point, center);
        
        bool warehouse_inside = distance_warehouse < contour_line[2];
        bool stadium_inside = distance_stadium < contour_line[2];
        
        if ((warehouse_inside && !stadium_inside) || (!warehouse_inside && stadium_inside)) {
          l_i_j++;
        }
      }
      
      int x_i_j = i * m + j;
      lp.set_c(x_i_j, -(100 * r[i][j] - l_i_j));
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible() || s.is_unbounded()) {
    std::cout << "no" << std::endl;
  } else {
    std::cout << floor_to_long(-s.objective_value() / 100) << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}