#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__
#include <SFML/Graphics.hpp>
#include "stack.h"
using namespace std;

// Class containing methods to render Mandelbrot Set using various algorithms, and at different zooms and precisions
class MandelbrotSet {

private:
    // Width of the SFML window
    int width;
    // Height of the SFML window
    int height;
    // Random variable used to make a color palette
    int p;
    // Max number of iterations for different algorithms (acts as a measure of precision)
    int max_iteration;
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
    // Color palette used for the coloring of the Set (uses p as a random input)
    vector<sf::Color> palette;
    // Array of points in the window, having information about their color
    sf::VertexArray points;
    // Stack containing rendering of the Mandelbrot Set at different zoom levels
    stack_pixels st;

public:
    // Constructor (takes member variables p, width, height, max_iteration as input)
    MandelbrotSet(int p, int width, int height, int q, int r, int s, int max_iteration);
    // Function to change color palette
    void change_colors(int p, int q, int r, int s);
    // Function to return array points
    sf::VertexArray get_points();
    // Function to return zoom
    long long get_zoom();
    // Function to return precision (i.e. max_iteration)
    int get_precision();
    // Functions to increase and decrease precision (value changed by 100)
    void inc_precision();
    void dec_precision();
    // Function to call different rendering Algorithms based on precision involved
    // If addToStack is true, then the new rendering is pushed to the stack
    void Algorithm(bool addToStack);
    // Function to find the new values of limits of the Set on zooming in at point (x,y)
    void zoom_in(double x, double y);
    // Function to show the last zoomed out rendering
    void zoom_out();
    // Plotting Mandelbrot Set with Escape Time Algorithm (Unoptimized)
    void unoptimizedEscapeTime(bool addToStack);
    // Plotting Mandelbrot Set with Escape Time Algorithm (Optimized)
    void optimizedEscapeTime(bool addToStack);
    // Plotting Mandelbrot Set with Escape Time Algorithm, and reducing time by checking periodicity (Optimized)
    void periodicallyCheckedEscapeTime(bool addToStack);
    // Plotting Mandelbrot Set using Histogram Coloring (involves optimized escape time algorithm as a subtask)
    void histogramColoring(bool addToStack);
    // Plotting Mandelbrot Set using Continuous (smooth) Coloring
    void continuousColoring(bool addToStack);
};
#endif // __MANDELBROT_H__
