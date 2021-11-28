#ifndef __STACK_H__
#define __STACK_H__
#include <SFML/Graphics.hpp>

// Structure containing crucial data for a rendering of the Mandelbrot Set
struct pixels_data {
    // Array of points, containing color of each point in the window
    sf::VertexArray points;
    // Left limit of the Mandelbrot Set (min x)
    double lim_l;
    // Right limit of the Mandelbrot Set (max x)
    double lim_r;
    // Top limit of the Mandelbrot Set (max y)
    double lim_t;
    // Bottom limit of the Mandelbrot Set (min y)
    double lim_b;
    // Zoom factor of the given rendering
    long long zoom;
    // Pointer to previous element of the stack
    pixels_data* prev;
    // Constructor
    pixels_data();
};

// Stack containing rendering of the Mandelbrot Set at different zoom levels
// Implemented as a linked list, with top-to-bottom pointers
class stack_pixels {

private:
    // Topmost element of stack
    pixels_data* top;
    // Number of renderings present in the stack, initially 0
    int size;

public:
    // Constructor
    stack_pixels();
    // Pushing a new element (of rendering) at the top of the stack
    void push(pixels_data* elem);
    // Removing and returning the topmost element of the stack
    pixels_data pop();
};
#endif // __STACK_H__
