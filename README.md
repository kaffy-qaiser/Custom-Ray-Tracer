# Advanced Ray Tracer with CUDA Acceleration and NLP Integration (In Progress 🚧)

## Project Overview

This project features a custom ray tracer designed and implemented in C++ with CUDA acceleration, enabling high-fidelity image generation. The system integrates NLP capabilities through Hugging Face, allowing users to input descriptive text prompts and generate corresponding images. A Next.js frontend provides an intuitive user interface, while OpenGL is used for real-time rendering and visualization, ensuring optimized performance across various hardware configurations.
## Key Features

1. **Custom Ray Tracer**: Implements a robust ray tracing engine in C++ with support for various materials, objects, and lighting models.
2. **CUDA Acceleration**: Utilizes CUDA to accelerate rendering processes, significantly improving performance on compatible hardware.
3. **NLP Integration**: Incorporates Hugging Face models through a Next.js frontend, enabling image generation from text prompts.
4. **OpenGL Rendering**: Leverages OpenGL for real-time visualization and rendering.
5. **Advanced Rendering Techniques**: Supports features such as:
    - Path tracing
    - Photon mapping
    - Importance sampling
    - Volumetric rendering
    - Subsurface scattering
6. **Flexible Scene Construction**: Allows for the creation of complex scenes with various objects, materials, and lighting conditions.


## Project Structure

The project is organized into several key components:

- `core/`: Contains fundamental classes like `Vector3`, `Ray`, `Scene`, and `Camera`.
- `objects/`: Defines various renderable objects like `Sphere`, `Plane`, and `SDFObject`.
- `materials/`: Implements different material types including `Phong`, `Reflective`, and `Transparent`.
- `lights/`: Contains light source implementations.
- `textures/`: Provides texture functionality with `CheckerTexture` and `ImageTexture`.
- `main.cpp`: The entry point of the application, demonstrating scene setup and rendering.

## Getting Started

1. Clone the repository
2. Ensure you have CUDA toolkit installed for GPU acceleration
3. Build the project using CMake:
   ```
   mkdir build && cd build
   cmake ..
   make
   ```
4. Run the executable:
   ```
   ./raytracer
   ```

## NLP Integration

The NLP functionality is integrated through a separate Next.js frontend. To use this feature:

1. Navigate to the frontend directory
2. Install dependencies: `npm install`
3. Start the Next.js server: `npm run dev`
4. Access the web interface and input text prompts to generate images

## Performance Optimization

The ray tracer is optimized for performance using several techniques:

- CUDA acceleration for parallel processing
- Importance sampling for efficient light transport simulation
- Photon mapping for global illumination effects
- Spatial data structures for faster intersection tests



## Contributing

Contributions to this project are welcome. Please fork the repository and submit a pull request with your proposed changes.

## License

This project is licensed under the MIT License.

---

