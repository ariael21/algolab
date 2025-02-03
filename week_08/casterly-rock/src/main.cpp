///4
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::pair<long, long> Coordinates;
typedef std::vector<Coordinates> CV;

void solve() {
  int n, m;
  long s;
  std::cin >> n >> m;
  std::cin >> s;
  
  int b_s = 0;
  int c_s = 1;
  int a_w = 2;
  int c_w = 3;
  
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  CV nobles(n);
  CV commoners(m);
  
  long x_n = 0;
  long x_m = 0;
  long y_n = 0;
  long y_m = 0;
  
  int row = 0;
  
  for (int i = 0; i < n; i++) {
    long x, y;
    std::cin >> x >> y;
    
    nobles[i] = std::make_pair(x, y);
    x_n += x;
    y_n += y;
    
    // b_s * y + c_s <= -x
    lp.set_a(b_s, row, y);  
    lp.set_a(c_s, row, 1);  
    lp.set_b(row, -x); 
    
    row++;
  }
  
  for (int i = 0; i < m; i++) {
    long x, y;
    std::cin >> x >> y;
    
    commoners[i] = std::make_pair(x, y);
    x_m += x;
    y_m += y;
    
    // -b_s * y - c_s <= x
    lp.set_a(b_s, row, -y);  
    lp.set_a(c_s, row, -1);  
    lp.set_b(row, x); 
    
    row++;
  }
  
  Solution solution = CGAL::solve_linear_program(lp, ET());
  if (solution.is_infeasible()) {
    std::cout << "Y" << std::endl;
    return;
  }
  
  // add constraint such that the lines are orthogonal to each other
  lp.set_a(b_s, row, 1);  
  lp.set_a(a_w, row, 1);  
  lp.set_b(row, 0);
  row++;
  
  lp.set_a(b_s, row, -1);  
  lp.set_a(a_w, row, -1);  
  lp.set_b(row, 0);
  row++;
  
  if (s != -1) {
    lp.set_a(b_s, row, - y_n + y_m);  
    lp.set_a(c_s, row, m - n);  
    lp.set_b(row, s - (- x_n + x_m)); 
    
    row++;
  }
  
  int l_max = 4;

  for (int i = 0; i < n+m; i++) {
    Coordinates coor;
  
    if (i < n) {
      coor = nobles[i]; 
    } else {
      coor = commoners[i-n]; 
    }
      
    long x = coor.first;
    long y = coor.second;
    
    lp.set_a(c_w, row, -1);  
    lp.set_a(a_w, row, -x);  
    lp.set_a(l_max, row, -1);  
    lp.set_b(row, y); 
    
    lp.set_a(c_w, row+1, 1);  
    lp.set_a(a_w, row+1, x);  
    lp.set_a(l_max, row+1, -1);  
    lp.set_b(row+1, -y); 
    
    row+=2;
  }
  
  lp.set_c(l_max, 1);
  
  solution = CGAL::solve_linear_program(lp, ET());
  if (solution.is_infeasible()) {
      std::cout << "B" << std::endl;
  } else {
     std::cout << (long)std::ceil(CGAL::to_double(solution.objective_value())) << std::endl;
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