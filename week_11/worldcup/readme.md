## World Cup

It is an LP (Linear Program). I struggled with some details of the LP, but generally, it is quite straightforward.

In addition to the LP, we have to calculate the influence of the contour lines.

### LP Constraints

```
max. x(0,0) * (r(0,0) * l(0,0)) + ... + x(n,0) * (r(n,0) * l(n,0)) + x(n,1) * (r(n,1) * l(n,1)) + ... + x(n,m) * (r(n,m) * l(n,m))
```

Which is equivalent to:

```
min. - ( x(0,0) * (r(0,0) * l(0,0)) + ... + x(n,0) * (r(n,0) * l(n,0)) + x(n,1) * (r(n,1) * l(n,1)) + ... + x(n,m) * (r(n,m) * l(n,m)) )
```

As we subtract **0.01 * l(i, j)**, we have to **scale it all by a factor of 100**:

```
min. for i..n, for j..m: - x(i,j) * (100 * (r(i,j) - l(i,j)))
```

Thus, we **acquire the result by negating and dividing by 100.**

Subject to:

```
x(i,j) >= 0

s(i) >= x(i,0) + ... + x(i, m)

d(j) = x(0,j) + ... + x(n,j) 

100 * u(j) >= x(0,j) * a(0) + ... + x(n,j) * a(n)
```

For `i = 0..n` and `j = 0..m`, where:

- `x(i,j)`: amount of beer moved from warehouse `i` to stadium `j`
- `r(i,j)`: revenue we get per beer from warehouse `i` to stadium `j`
- `l(i,j)`: loss we incur due to contour lines from warehouse `i` to stadium `j`
- `s(i)`: capacity of beer in warehouse `i`
- `d(j)`: demand of stadium `j`
- `u(j)`: maximum amount of liters of pure alcohol allowed in stadium `j`
- `a(i)`: amount of alcohol (ml) in one beer produced in warehouse `i`

### How to Calculate `l(i,j)`

- `l(i,j)` is the number of contour lines we cross while traveling from warehouse `i` to stadium `j`.

Create a **Delaunay triangulation** using the points of the warehouses and stadiums. Using this, we can quickly determine the closest point to a contour center.

We start by filtering the relevant contour lines:
- Calculate the **squared distance from the center of the contour line to the closest vertex in the Delaunay triangulation**. If this distance is smaller than the squared radius of this contour line, we know that we have at least one point (warehouse or stadium) inside a contour line.

For each combination of warehouse `i` and stadium `j`, we check if only one of them is inside a relevant contour line. We do this for all relevant contour lines and count how many lines are crossed while traveling from `i` to `j`.

```c++
for (int i = 0; i < n; i++) {
    K::Point_2 warehouse_point = K::Point_2(warehouses[i].coordinate.x, warehouses[i].coordinate.y);

    for (int j = 0; j < m; j++) {
        K::Point_2 stadium_point = K::Point_2(stadiums[j].coordinate.x, stadiums[j].coordinate.y);

        int l_i_j = 0;
        
        // Check how many contour lines are crossed
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
```

### Notes

- **Think about lower bounds:** Here, we need a lower bound for `x_i_j >= 0`, as we cannot move negative amounts of beer or move beer away from a stadium.