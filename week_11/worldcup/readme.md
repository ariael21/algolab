## Worldcup

It is an LP. I struggled with some details of the LP, but generally it is quite straight forward. 

Additionally to the LP, we have to calculate the influence of the contour lines.

### LP-Constraints

```
max. x(0,0) * (r(0,0) * l(0,0)) + ... + x(n,0) * (r(n,0) * l(n,0)) + x(n,1) * (r(n,1) * l(n,1)) + ... + x(n,m) * (r(n,m) * l(n,m))
```

Which is equivalent to:

```
min. - ( x(0,0) * (r(0,0) * l(0,0)) + ... + x(n,0) * (r(n,0) * l(n,0)) + x(n,1) * (r(n,1) * l(n,1)) + ... + x(n,m) * (r(n,m) * l(n,m)) )
```

subject to:

```

x(i,j) >= 0

s(i) >= x(i,0) + ... + x(i, m)

d(j) = x(0,j) + ... + d(n,j) 

100 * u(j) >= x(0,j) * a(0) + ... + x(n,j) * a(n)

```

For `i = 0..n` and `j = 0..m`, where

- `x(i,j)`: amount of beer moved from warehouse `i` to statdium `j`
- `r(i,j)`: revenue we get per beer from  warehouse `i` to statdium `j`
- `l(i,j)`: loss we make due to contour-lines from warehouse `i` to statdium `j`
- `s(i)`: capacity of beer of warehouse `i`
- `d(j)`: demand of stadium `j`
- `u(j)`: maximum amount of liter of pure alcohol of stadium `j`
- `a(i)`: amount of alcohol (ml) of one beer producesin warehouse `i`

### Things I learned

- Think about lower bounds: here we need a lower bound for `x_i_j >= 0`, as we cannot move negatives amounts of beer / move beer away from a stadium
