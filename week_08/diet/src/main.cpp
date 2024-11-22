#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int round_down(double value) {
  double floored = std::floor(value);
  
  while(floored > value) {
    floored -= 1;
  }
  
  while(floored + 1 < value) {
    floored += 1;
  }
  
  return static_cast<int>(floored);
}

int solve() {
  int n, m;
  std::cin >> n;
  
  if (n == 0) { return 0; }
  
  std::cin >> m;
  
  // store nutrients min and max value
  std::vector<std::vector<int>> nutrients(n, std::vector<int>(2, -1));
  
  for (int i = 0; i < n; i++) {
    std::cin >> nutrients.at(i).at(0) >> nutrients.at(i).at(1);
  }
  
  // store nutrients per food
  std::vector<int> price(m, -1);
  std::vector<std::vector<int>> food(m, std::vector<int>(n, -1));
  
  for (int j = 0; j < m; j++) {
    std::cin >> price.at(j);
    
    for (int i = 0; i < n; i++) {
      std::cin >> food.at(j).at(i);
    }
  }
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  for (int i = 0; i < n; i++) {
    int min = nutrients.at(i).at(0);
    int max = nutrients.at(i).at(1);
    
    for (int j = 0; j < m; j++) {
      int x_j = j;
      int C_j_i = food.at(j).at(i);
      
      // for upper bound
      // x_j * C_j_i
      lp.set_a(x_j, i, C_j_i);
      
      // for lower bound
      // -x_j * C_j_i
      lp.set_a(x_j, n + i, -C_j_i);
    }
    
    // max_i >= x_i * C_0_i + ... + x_i * C_m-1_i 
    lp.set_b(i, max);
    
    // -min_i >= -x_i * C_0_i - ... - x_i * C_m-1_i
    lp.set_b(n + i, -min);
  }
  
  // set objective function
  for (int j = 0; j < m; j++) {
    int x_j = j;
    
    // x_j * p_j
    lp.set_c(x_j, price.at(j));
    
    // lower bound => cannot have negative amounts of foods
    // x_j >= 0
    lp.set_l(x_j, true, 0);
  }
  
  Solution lp_solution = CGAL::solve_linear_program(lp, ET());
  
  if (lp_solution.is_unbounded() || lp_solution.is_infeasible()) {
    std::cout << "No such diet." << std::endl;
  } else {
    double solution = CGAL::to_double(lp_solution.objective_value());
    int rounded = round_down(solution);
    
    std::cout << rounded << std::endl; 
  }
  
  return 1;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  while(solve() != 0) { }
}