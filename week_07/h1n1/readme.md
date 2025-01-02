## H1N1

We have sick people spread across a 2D plane and healthy people who want to escape from the sick individuals without getting too close to them. The coordinates of all sick and healthy people are provided. For each healthy person, we are given a value `d`, which determines the minimum safe distance they must maintain from any sick person. Specifically, the healthy individuals must stay at least `sqrt(d)` units away from the sick individuals.

The problem is to determine whether a disc with radius `sqrt(d)` can move away from the sick individuals without violating this distance constraint.

To solve this, we construct a **Delaunay Triangulation** using the coordinates of the sick individuals. This triangulation allows us to calculate the maximum permissible `d` value for each face in the triangulation, enabling us to assess whether the healthy individuals can escape. The steps are as follows:

1. Start with the outermost faces (infinite faces) of the triangulation. For these, the `max_face_d` value is set to `infinity`, as they lie outside the cluster of sick people.  
2. Traverse inward through the triangulation, examining each face and its neighbors.  
   - For each edge `e` shared between two neighboring faces, the size of the edge determines the maximum `d` value for the neighboring face.  
   - The escaping disc must be small enough so that it doesn’t touch the vertices at the endpoints of `e`.  
3. The `max_face_d` value for each face is computed progressively, starting from the outermost faces and moving inward. This process is managed using a priority queue, where the priority is determined by `max_face_d`. The priority queue ensures that we correctly propagate the largest possible `max_face_d` values to all faces.

### Steps to solve the problem:
1. Create a **Delaunay Triangulation** using the coordinates of the sick individuals.  
2. Compute the maximum `d` (`max_face_d`) for which a person can escape for each face in the triangulation.  
3. For each healthy person, check whether they can escape with their given `d`:
   - Ensure that the closest vertex is sufficiently far away.  
   - Verify that the healthy person lies in a face where `d` is less than the corresponding `max_face_d`.
