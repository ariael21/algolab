## Diet

- **n** = number of nutrients
    - **min_i** = minimum amount of nutrient i needed
    - **max_i** = maximum amount of nutrient i needed
- **m** = number of foods
    - **p_j** = price for food j
    - **C_j_i** = amount of nutrient i in food j

**Goal**: Find the cheapest meal using the available foods to cover the minimum requirements of all nutrients.



## Define the LP

We define: 

**x_j** = amount of food j

Thus we need to find:

    min. p_0 * x_0 + ... + p_m-1 * x_m-1

stuch that

    max_0 >= x_0 * C_0_0 + ... + x_m-1 * C_m-1_0 >= min_0

    ...

    max_n-1 >= x_0 * C_0_n-1 + ... + x_m-1 * C_m-1_n-1 >= min_n-1

    x_0, ... x_m-1 >= 0

which is equivalent to the LP:

    min. p_0 * x_0 + ... + p_m-1 * x_m-1

stuch that

    max_0 >= x_0 * C_0_0 + ... + x_m-1 * C_m-1_0

    ...

    max_n-1 >= x_0 * C_0_n-1 + ... + x_m-1 * C_m-1_n-1

    min_0 >= -x_0 * C_0_0 - ... - x_m-1 * C_m-1_0

    ...

    min_n-1 >= -x_0 * C_0_n-1 - ... - x_m-1 * C_m-1_n-1

    x_0, ... x_m-1 >= 0