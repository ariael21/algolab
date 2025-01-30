///1
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

typedef std::pair<Point, Point> Leg;

bool in(std::vector<Point>& triangle, Leg& leg) {
  bool start_in = (
    !CGAL::right_turn(triangle[0], triangle[1], leg.first) &&
    !CGAL::right_turn(triangle[2], triangle[3], leg.first) &&
    !CGAL::right_turn(triangle[4], triangle[5], leg.first) 
  );
  
  bool end_in = (
    !CGAL::right_turn(triangle[0], triangle[1], leg.second) &&
    !CGAL::right_turn(triangle[2], triangle[3], leg.second) &&
    !CGAL::right_turn(triangle[4], triangle[5], leg.second) 
  );

  return (start_in && end_in);
}

void solve() {
  int m, n;
  std::cin >> m >> n;
  
  std::vector<Leg> legs;
  legs.reserve(m-1);
  
  int x, y;
  std::cin >> x >> y;
  
  Point previous = Point(x, y);
  
  for (int i = 1; i < m; i++) {
    std::cin >> x >> y;
    Point current = Point(x, y);
    
    legs.push_back(std::make_pair(previous, current));
    
    previous = current;
  }
  
  std::vector<std::vector<Point>> maps(n, std::vector<Point>(6));
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 6; j++) {
      std::cin >> x >> y;
      maps[i][j] = Point(x, y);
    }
    
    // ensure correct ordering of the points of the triangle
    // needed for the check to determine if a leg is inside a triangle
    for (int j = 0; j < 6; j+=2) {
      int next_edge_index = (j+2) % 6; // index the first node of the next edge
      
      if (CGAL::right_turn(maps[i][j], maps[i][j+1], maps[i][next_edge_index])) {
        // order them, such that a point inside the triangle causes a left-turn
        std::swap(maps[i][j], maps[i][j+1]);
      }
    }
  }
  
  std::vector<std::vector<bool>> map_cover(n, std::vector<bool>(m-1, false));
  
  for (int i = 0; i < m-1; i++) {
    for (int j = 0; j < n; j++) {
      bool inside = in(maps[j], legs[i]);
      map_cover[j][i] = inside; 
    }
  }

  int left = 0;
  int right = -1;
  
  std::vector<int> covered(m-1, 0);
  
  int min_map = n; // default: we need all maps
  int uncovered = m-1;
  int current_maps = 0;
  
  while (true) {
    if (uncovered > 0 && right == n-1 || left == n) {
      break;
    }
    
    if (uncovered > 0) {
      // not all legs are covered: add a map
      
      right++;
      current_maps++;
      
      for (int i = 0; i < m-1; i++) {
        if (map_cover.at(right).at(i)) {
          if (covered.at(i) == 0) {
            uncovered--;
          }
          covered.at(i)++;
        }
      }
    }
    
    if (uncovered == 0) {
      // check if we found a better solution
      min_map = std::min(min_map, current_maps);
      
      // all legs are covered: we remove a map
      
      for (int i = 0; i < m-1; i++) {
        if (map_cover.at(left).at(i)) {
          
          covered.at(i)--;
          
          if (covered.at(i) == 0) {
            uncovered++;
          }
        }
      }
      
      left++;
      current_maps--;
    }
  }
  
  std::cout << min_map << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int c; std::cin >> c;
  
  for (int i = 0; i < c; i++) {
    solve();
  }
}