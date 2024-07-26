#include "RayTracer.h"

int main() {
    int width = 800;
    int height = 400;

    RayTracer rayTracer(width, height);
    rayTracer.render();

    return 0;
}