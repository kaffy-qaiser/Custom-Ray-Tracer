CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRCS = main.cpp RayTracer.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = RayTracer.h Vec3.h Ray.h Sphere.h Camera.h
TARGET = raytracer

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) output.ppm