# ray-tracer

This project is a simple ray tracer implemented in C++. It includes the following components:

- **Vec3**: A class representing a 3D vector with various vector operations such as addition, subtraction, normalization, dot product, cross product, and reflection.
- **Ray**: A class representing a ray with an origin and direction, and a method to compute a point along the ray at a given parameter.
- **Material**: An abstract base class for different materials, with derived classes `Lambertian` and `Metal` implementing specific scattering behaviors for diffuse and metallic surfaces, respectively.

The ray tracer simulates the interaction of rays with surfaces to produce realistic images by tracing the path of light as pixels in an image plane and simulating the effects of its encounters with virtual objects.