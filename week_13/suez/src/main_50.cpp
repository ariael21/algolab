///1
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::pair<int, int> Coordinate;

void solve() {
  int n, m, h, w;
  std::cin >> n >> m >> h >> w;
  
  std::vector<Coordinate> free(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    
    free[i] = std::make_pair(x, y);
  }
  
  std::vector<Coordinate> fixed(m);
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    
    fixed[i] = std::make_pair(x, y);
  }
  
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  // lower bound: a_i >= 1
  
  int row = 0;
  
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      int x_i = free[i].first;
      int x_j = free[j].first;
      
      int y_i = free[i].second;
      int y_j = free[j].second;
      
      long delta_x = std::abs(x_i - x_j);
      long delta_y = std::abs(y_i - y_j);
      
      // at what axis are the two posters gonna meet soonest?
      // => add constraint there
      
      // scale both, such that they have the same scale factor (such that they are comparable)
      long scaled_delta_x = delta_x * h; // or delta_x / w
      long scaled_delta_y = delta_y * w; // or delta_y / h
      
      if (scaled_delta_x >= scaled_delta_y) { // note: "=" case => both work (so we can choose either)
        // only check x-boundary
        
        // for case that i.x > j.x (if we do delta_x = abs(i.x - j.y) => both cases are the same)
        
        // x_i + a_i * w/2 <= x_j + a_j * w/2
        // => w*a_i + w*a_j <= 2*(i.x - j.y)    NOTE: if we take abs(i.x - j.y) such that we dont have to distinguish between i.x > ? < j.x

        lp.set_a(i, row, w); // w*a_i
        lp.set_a(j, row, w); // w*a_j
        lp.set_b(row, 2 * delta_x); // 2*abs(i.x - j.y)
      } else {
        // only check y-boundary
        
        // analog for y-axis
        
        // y_i + a_i * h/2 <= y_j + a_j * h/2
        // => h*a_i + h*a_j <= 2*abs(i.y - j.y)    

        lp.set_a(i, row, h); // h*a_i
        lp.set_a(j, row, h); // h*a_j
        lp.set_b(row, 2 * delta_y); // 2*abs(i.y - j.y)
      }
      
      row++;
    }
  }
  
  for (int i = 0; i < n; i++) {
    lp.set_c(i, -(2 * (h + w)));
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // std::cout << s << std::endl;
  
  std::cout << long(std::ceil(CGAL::to_double(-s.objective_value()))) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; 
  std::cin >> t;
  
  for (int i = 0; i < t; i++) {
    solve();
  }
}