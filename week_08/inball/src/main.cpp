#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int round_down(double value) {
  int floored = std::floor(value);
    
  while (value > floored) {
    floored += 1;
  }
  
  while (floored > value) {
    floored -= 1;
  }
  
  return floored;
}

bool solve() {
  int n; 
  std::cin >> n;
  
  if (n == 0) { return false; }
  
  int d; 
  std::cin >> d;
  
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  for (int i = 0; i < n; i++) {
    int squared_norm = 0;
    
    for (int j = 0; j < d; j++) {
      int x_j = j;
      
      int a_i_j;
      std::cin >> a_i_j;
      
      squared_norm += a_i_j * a_i_j;
      
      // a_i_j * x_j
      lp.set_a(x_j, i, a_i_j);
    }
    
    int norm = std::sqrt(squared_norm); // guaranteed to be an integer
    
    // r * ||a||_2
    lp.set_a(d, i, norm);
    
    int b_i;
    std::cin >> b_i;
    
    // a_i_0 * x_0 + ... +  a_i_d-1 * x_d-1 + r * ||a||_2 <= b_i
    lp.set_b(i, b_i);
  }
  
  // minimize: -r
  lp.set_c(d, -1);
  
  // lower bound for r => solutions with negative r make no sense
  lp.set_l(d, true, 0);
  
  Solution solution = CGAL::solve_linear_program(lp, ET());
  
  if (solution.is_infeasible()) {
    std::cout << "none" << std::endl;
  } else if (solution.is_unbounded()) {
    std::cout << "inf" << std::endl;
  } else {
    double value = CGAL::to_double(solution.objective_value());
    
    // negate result because we maximized -r => we want r and not -r
    double negative_value = -value;
    
    std::cout << round_down(negative_value) << std::endl;
  }
  
  return true;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  while(solve()) { }
}
