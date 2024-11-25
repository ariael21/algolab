///

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

int round_up(double value) {
  double ceiled = std::ceil(value);
  
  while (ceiled > value) {
    ceiled -= 1;
  }
  
  while (ceiled < value) {
    ceiled += 1;
  }
  
  return static_cast<int>(ceiled);
}

Program type_1(int a, int b) {
  // max. b * y - a * x
  // -> min. -b * y + a * x
  // s.t.
  // x, y >= 0 (lower bound)
  // x + y <= 4
  // 4x + 2y <= a * b
  // -x + y <= 1
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  const int X = 0;
  const int Y = 1;
  
  lp.set_l(X, true, 0);
  lp.set_l(Y, true, 0);
  
  // set the coefficients of A and b:
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, (a * b)); // 4x + 2y <= a * b
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1
  
  // no upper bound
  // ...
  
  // objective function
  lp.set_c(X, a);
  lp.set_c(Y, -b);
  
  return lp;
}

Program type_2(int a, int b) {
  // min. a * x + b * y + z
  // s.t.
  // x,y,z <= 0 (upper bound)
  // x + y >= -4
  // -> -x -y <= 4
  // 4x + 2y + z >= -a * b
  // -> -4x -2y -z <= a * b
  // -x + y >= -1
  // -> x - y <= 1
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  
  lp.set_u(X, true, 0);
  lp.set_u(Y, true, 0);
  lp.set_u(Z, true, 0);
  
  // set the coefficients of A and b:
  lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_a(Z, 0, 0); lp.set_b(0, 4); // -x -y <= 4
  lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, (a * b)); // -4x -2y -z <= a * b
  lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_a(Z, 2, 0); lp.set_b(2, 1); // x - y <= 1
  
  // no upper bound
  // ...
  
  // objective function
  lp.set_c(X, a);
  lp.set_c(Y, b);
  lp.set_c(Z, 1);
  
  return lp;
}

void solve(int p) {
  int a, b;
  std::cin >> a >> b;
  
  Program lp;
  
  if (p == 1) {
    lp = type_1(a, b);
  } else if (p == 2) {
    lp = type_2(a, b);
  }
  
  Solution solution = CGAL::solve_linear_program(lp, ET());
  
  if (solution.is_unbounded() || solution.is_infeasible()) {
    std::cout << "no" << std::endl;
  } else {
    int value = round_up(CGAL::to_double(solution.objective_value()));
    
    if (p == 1) {
      value *= -1;
    }
    
    std::cout << value << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int p;
  std::cin >> p;
  
  while (p != 0) {
    solve(p);
    std::cin >> p;
  };
}