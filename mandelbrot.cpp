#include "mandelbrot.h"
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

// Constructor to allocate memory and initialize various member variables
MandelbrotSet::MandelbrotSet(int p, int width, int height, int q, int r, int s, int max_iteration) {
    // max_iteration, p, width, height taken as input
    this -> max_iteration = max_iteration;
    this -> p = p;
    this -> width = width;
    this -> height = height;
    // Initial Mandelbrot Set is plotted in the dimensions [-2: 0.47], [-1.12: 1.12]
    this -> lim_l = -2;
    this -> lim_r = 0.47;
    this -> lim_t = 1.12;
    this -> lim_b = -1.12;
    // Initial zoom level is 1x
    this -> zoom = 1;

    // VertexArray points consists elements of type sf::Points
    // Size of the array is same as area of window
    points.setPrimitiveType(sf::Points);
    points.resize(width * height);

    // Creating color palette (consisting of p colors)
    // Uses p, and random variables q, r, s
    // Take three integers cx, cy, cz (intially equal to 1)
    // At each step from 1 to p, cx = q^p, cy = r^p and cz = s^p
    // To be able to calculate such large powers, we take the remainder of these values with p
    // Then the color at each step has RGB value (cx, cy, cz), after reducing each number to the range [0, 255]
    int cx = 1;
    int cy = 1;
    int cz = 1;
    palette.push_back(sf::Color(cx, cy, cz));

    for(int i = 0; i < p ; i ++) {
        // Instead of taking power at each step, we simply multiply and take remainder with p
        cx = (cx * q) % p;
        cx = cx % 256;
        cy = (cy * r) % p;
        cy = cy % 256;
        cz = (cz * s) % p;
        cz = cz % 256;
        palette.push_back(sf::Color(cx, cy, cz));
    }
}

// Change color palette with respect to the new values of p, q, r, s
void MandelbrotSet::change_colors(int p, int q, int r, int s) {
    // Update p (number of colors in color palette)
    this -> p = p;
    // Clear palette, to be able to recalculate the colors and save them
    palette.clear();

    // At each step from 1 to p, cx = q^p, cy = r^p and cz = s^p
    int cx = 1;
    int cy = 1;
    int cz = 1;
    palette.push_back(sf::Color(cx, cy, cz));

    for(int i = 0; i < p ; i ++) {
        // Instead of taking power at each step, we simply multiply and take remainder with p
        cx = (cx * q) % p;
        cx = cx % 256;
        cy = (cy * r) % p;
        cy = cy % 256;
        cz = (cz * s) % p;
        cz = cz % 256;
        palette.push_back(sf::Color(cx, cy, cz));
    }

    // Remove the topmost plot from the stack (that was the plot before changing the colors)
    st.pop();
    // Re-render the Mandelbrot Set and add this new plot to the stack
    this -> Algorithm(true);
}

// Function to make points array publicly available
sf::VertexArray MandelbrotSet::get_points() {
    return points;
}

// Function to get current zoom of the Set
long long MandelbrotSet::get_zoom() {
    return zoom;
}

// Function to get current precision level of the set
int MandelbrotSet::get_precision() {
    return max_iteration;
}

// Increase precision (cf. max_iteration) by a value of 100
void MandelbrotSet::inc_precision() {
    // Precision is increased to a maximum of 1000
    if(max_iteration >= 1000) return;
    max_iteration += 100;
    // Re-render the set, but do not add this new rendering to the stack
    this -> Algorithm(false);
}

// Decrease precision (cf. max_iteration) by a value of 100
void MandelbrotSet::dec_precision() {
    // Precision is decreased to a minimum of 100
    if(max_iteration <= 100) return;
    max_iteration -= 100;
    // Re-render the set, but do not add this new rendering to the stack
    this -> Algorithm(false);
}

// Choosing an algorithm based on the precision level required
// For higher precision, choosing a faster method to get calculation time similar to previous precision
void MandelbrotSet::Algorithm(bool addToStack) {
    // If precision is between 10% and 20%, then do histogram coloring (most time consuming)
    if(max_iteration <= 200) this -> histogramColoring(addToStack);
    // If precision is between 30% and 40%, then use continuous coloring
    else if(max_iteration <= 400) this -> continuousColoring(addToStack);
    // If precision is 50% or above, then use periodic escaping (not as accurate, but highly time-efficient)
    else this -> periodicallyCheckedEscapeTime(addToStack);
}

// Finding power (x^n) for large values of n
// Time complexity O(log n) algorithm
double power(double x, long long n) {
    double res = 1;
    // Find binary representation of n, and note that x^(2^k) = (x^(2^(k-1)))^2
    while(n > 0) {
        // If some bit is set in n, then result must contain the corresponding value of x
        if(n & 1) res *= x;
        x *= x;
        n >>= 1;
    }
    return res;
}

// Zooming in, taking (x,y) as the zoom origin
void MandelbrotSet::zoom_in(double x, double y) {
    // Max value of zoom is 62 (since we have to do 2^zoom, and we do not wish to overflow)
    if(zoom == 62) return;

    // x0 consists the position of the point x on the number line from lim_l to lim_r
    double x0 = lim_l + ((lim_r - lim_l) * x) / width;
    // y0 consists the position of the point y on the number line from lim_b to lim_t
    double y0 = lim_b + ((lim_t - lim_b) * y) / height;

    // zoom factor increased by 1
    zoom ++;
    // two_zoom = 2 ^ zoom, used to find change in limits
    long long two_zoom = power(2, zoom);
    // multiplier is the factor by which the limits are changed
    // multiplier is taken as 0.9 - (1/2)^(two_zoom), where 0.9 was found as an optimal value based on different trials
    double multiplier = 0.9 - power(0.5, two_zoom);

    lim_l *= multiplier;
    lim_r *= multiplier;
    lim_t *= multiplier;
    lim_b *= multiplier;

    // Centering the limits at (x0, y0) by shifting the origin to this point
    // Horizontal shift is distance between mid point of lim_l, lim_r and x0
    double horizontal_shift = x0 - (lim_l + lim_r) / 2;
    // Vertical shift is distance between mid point of lim_b, lim_t and y0
    double vertical_shift = y0 - (lim_t + lim_b) / 2;

    // Shifting horizontal limits by horizontal_shift
    lim_l += horizontal_shift;
    lim_r += horizontal_shift;
    // Shifting vertical limits by vertical_shift
    lim_t += vertical_shift;
    lim_b += vertical_shift;
}

// Zooming out, by taking the previous element on the stack of plots
void MandelbrotSet::zoom_out() {
    if(zoom != 1) {
        // Remove the topmost (current element) and take the previous element on the stack
        pixels_data data = st.pop();
        // points, zoom and limits changed to that of current top element of stack
        points = data.points;
        zoom = data.zoom;
        lim_l = data.lim_l;
        lim_r = data.lim_r;
        lim_t = data.lim_t;
        lim_b = data.lim_b;
    }
}

void MandelbrotSet::unoptimizedEscapeTime(bool addToStack) {

    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Find point (x0,y0) in the limits corresponding to (i,j)
            // Complex number c = x0 + iy0
            double x0 = lim_l + ((lim_r - lim_l) * i) / width;
            double y0 = lim_b + ((lim_t - lim_b) * j) / height;
            // Complex number z = x + iy
            double x = 0;
            double y = 0;

            // Number of iterations required for z to get outside circle centered at origin of radius 2
            int iteration = 0;
            while(x * x + y * y <= 4 && iteration < max_iteration) {
                // New values of x and y found as x = Re(z^2 + c) and y = Im(z^2 + c)
                double xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iteration ++;
            }

            // Point at (i,j) colored with iteration (mod p) element of the color palette
            points[i*width + j].position = sf::Vector2f(i, j);
            points[i*width + j].color = palette[iteration % p];
        }
    }

    // Add the plot created to the stack if addToStack variable is true
    if(addToStack) {
        pixels_data* new_elem = new pixels_data;
        new_elem -> points = points;
        new_elem -> lim_l = lim_l;
        new_elem -> lim_r = lim_r;
        new_elem -> lim_t = lim_t;
        new_elem -> lim_b = lim_b;
        new_elem -> zoom = zoom;
        st.push(new_elem);
    }
}

void MandelbrotSet::optimizedEscapeTime(bool addToStack) {

    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Find point (x0,y0) in the limits corresponding to (i,j)
            // Complex number c = x0 + iy0
            double x0 = lim_l + ((lim_r - lim_l) * i) / width;
            double y0 = lim_b + ((lim_t - lim_b) * j) / height;
            // Complex number z = x + iy
            double x = 0;
            double y = 0;
            // To reduce computations, maintaining variables x2 = x^2 and y2 = y^2
            double x2 = 0;
            double y2 = 0;

            // Number of iterations required for z to get outside circle centered at origin of radius 2
            int iteration = 0;
            while(x2 + y2 <= 4 && iteration < max_iteration) {
                // New values of x and y found as x = Re(z^2 + c) and y = Im(z^2 + c)
                // Uses only 3 multiplications inside while loop, unlike 5 for the unoptimized version
                y = (x + x) * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iteration ++;
            }

            // Point at (i,j) colored with iteration (mod p) element of the color palette
            points[i*width + j].position = sf::Vector2f(i, j);
            points[i*width + j].color = palette[iteration % p];
        }
    }

    // Add the plot created to the stack if addToStack variable is true
    if(addToStack) {
        pixels_data* new_elem = new pixels_data;
        new_elem -> points = points;
        new_elem -> lim_l = lim_l;
        new_elem -> lim_r = lim_r;
        new_elem -> lim_t = lim_t;
        new_elem -> lim_b = lim_b;
        new_elem -> zoom = zoom;
        st.push(new_elem);
    }
}

void MandelbrotSet::periodicallyCheckedEscapeTime(bool addToStack) {
    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Find point (x0,y0) in the limits corresponding to (i,j)
            // Complex number c = x0 + iy0
            double x0 = lim_l + ((lim_r - lim_l) * i) / width;
            double y0 = lim_b + ((lim_t - lim_b) * j) / height;
            // Complex number z = x + iy
            double x = 0;
            double y = 0;
            // To reduce computations, maintaining variables x2 = x^2 and y2 = y^2
            double x2 = 0;
            double y2 = 0;
            // To further reduce computations maintaining variables xold = x and yold = y after some period of iterations
            double xold = 0;
            double yold = 0;
            // Period after which xold and yold are changed to current x and y values
            int period = 0;
            // Number of iterations required for z to get outside circle centered at origin of radius 2
            int iteration = 0;

            while(x2 + y2 <= 4 && iteration < max_iteration) {
                // New values of x and y found as x = Re(z^2 + c) and y = Im(z^2 + c)
                y = (x + x) * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iteration ++;

                // Checking whether difference between x and y with xold and yold is small
                bool small_x = abs(x - xold) < 1e-4;
                bool small_y = abs(y - yold) < 1e-4;

                // If x and y haven't changed much, then not expected to change much in future also
                // Then (i,j) is part of the Set, and so we will need to run it for max number of iterations
                if(small_x && small_y) {
                    iteration = max_iteration;
                    break;
                }

                // Period to update xold and yold set as max_iteration / 10
                period ++;
                if(period > max_iteration / 10.0f) {
                    period = 0;
                    xold = x;
                    yold = y;
                }
            }

            // Point at (i,j) colored with iteration (mod p) element of the color palette
            points[i*width + j].position = sf::Vector2f(i, j);
            points[i*width + j].color = palette[iteration % p];
        }
    }

    // Add the plot created to the stack if addToStack variable is true
    if(addToStack) {
        pixels_data* new_elem = new pixels_data;
        new_elem -> points = points;
        new_elem -> lim_l = lim_l;
        new_elem -> lim_r = lim_r;
        new_elem -> lim_t = lim_t;
        new_elem -> lim_b = lim_b;
        new_elem -> zoom = zoom;
        st.push(new_elem);
    }
}

void MandelbrotSet::histogramColoring(bool addToStack) {

    // Maintaining a 2D array consisting of iterations for all points
    vector<vector<int> > iterations(width, vector<int>(height));
    // Map of iterations (number of points having some iteration count)
    vector<int> map_iterations(max_iteration + 1, 0);

    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Find point (x0,y0) in the limits corresponding to (i,j)
            // Complex number c = x0 + iy0
            double x0 = lim_l + ((lim_r - lim_l) * i) / width;
            double y0 = lim_b + ((lim_t - lim_b) * j) / height;
            // Complex number z = x + iy
            double x = 0;
            double y = 0;
            // To reduce computations, maintaining variables x2 = x^2 and y2 = y^2
            double x2 = 0;
            double y2 = 0;

            // Number of iterations required for z to get outside circle centered at origin of radius 2
            int iteration = 0;
            while(x2 + y2 <= 4 && iteration < max_iteration) {
                // New values of x and y found as x = Re(z^2 + c) and y = Im(z^2 + c)
                y = (x + x) * y + y0;
                x = x2 - y2 + x0;
                x2 = x * x;
                y2 = y * y;
                iteration ++;
            }
            // Iteration count for (i,j) updated in 2D array and map
            iterations[i][j] = iteration;
            map_iterations[iteration] ++;
        }
    }


    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Finding number of vertices which have iteration count less than iteration count for (i,j)
            int iteration = iterations[i][j];
            int iter = 0;
            for(int k = 0; k <= iteration; k ++) {
                iter += map_iterations[k];
            }

            // Point at (i,j) colored with (no. of vertices found) (mod p) element of the color palette
            points[i*width + j].position = sf::Vector2f(i, j);
            points[i*width + j].color = palette[iter % p];
        }
    }

    // Add the plot created to the stack if addToStack variable is true
    if(addToStack) {
        pixels_data* new_elem = new pixels_data;
        new_elem -> points = points;
        new_elem -> lim_l = lim_l;
        new_elem -> lim_r = lim_r;
        new_elem -> lim_t = lim_t;
        new_elem -> lim_b = lim_b;
        new_elem -> zoom = zoom;
        st.push(new_elem);
    }
}

// Given c1 and c2 on the number line, find point dividing the line segment between them in (1-t):t ratio
double linear_interpolation(double c1, double c2, double t) {
    return (1-t)*c1 + t*c2;
}

void MandelbrotSet::continuousColoring(bool addToStack) {
    for(int i = 0; i < width; i ++) {
        for(int  j = 0; j < height; j ++) {
            // Find point (x0,y0) in the limits corresponding to (i,j)
            double x0 = lim_l + ((lim_r - lim_l) * i) / width;
            double y0 = lim_b + ((lim_t - lim_b) * j) / height;
            double x = 0;
            double y = 0;

            // Instead of the discrete version, using logarithms to perform escape time algorithms
            int iteration = 0;
            while(x * x + y * y <= (1 << 16) && iteration < max_iteration) {
                double xtemp = x * x - y * y + x0;
                y = (x + x) * y + y0;
                x = xtemp;
                iteration ++;
            }

            double new_it = 0;

            if(iteration < max_iteration) {
                double z = log(x * x + y * y) / 2;
                double mu = log(z / log(2)) / log(2);
                // New iterations are no longer an integer
                new_it = (double)iteration + 1 - mu;
                iteration = (int) new_it;
            }

            points[i*width + j].position = sf::Vector2f(i, j);
            // Finding color corresponding to some integer near new iterations
            int iter = (int)linear_interpolation(iteration, iteration + 1, new_it - iteration);
            points[i*width + j].color = palette[iter % p];
        }
    }

    // Add the plot created to the stack if addToStack variable is true
    if(addToStack) {
        pixels_data* new_elem = new pixels_data;
        new_elem -> points = points;
        new_elem -> lim_l = lim_l;
        new_elem -> lim_r = lim_r;
        new_elem -> lim_t = lim_t;
        new_elem -> lim_b = lim_b;
        new_elem -> zoom = zoom;
        st.push(new_elem);
    }
}
