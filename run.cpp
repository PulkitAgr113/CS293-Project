#include <SFML/Graphics.hpp>
#include "stack.h"
#include "mandelbrot.h"
#include "run.h"
#include <time.h>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

void runtimeCalculator()
{
    srand(time(0));
    // Making a Mandelbrot set on a window of size 1000 x 1000
    int width = 1000;
    int height = 1000;

    // Making a color palette with p colors (where p is a random integer from 499 to 999)
    int p = 499 + rand() % 501;
    // q, r, s are random integers in range [2, 999], used to create RGB values in the color palette
    int q = 2 + rand() % 998;
    int r = 2 + rand() % 998;
    int s = 2 + rand() % 998;

    // Precision of all algorithms set at 1000
    int precision = 1000;

    MandelbrotSet* m = new MandelbrotSet(p, width, height, q, r, s, precision);

    // Running all 5 algorithms N (=10) times and adding the times required to file runtimes.txt
    int N = 10;
    ofstream fout;
    fout.open("runtimes.txt", ios::out);

    for(int  i = 0; i < N; i ++) {
        auto start = chrono::system_clock::now();
        m -> unoptimizedEscapeTime(true);
        auto end = chrono::system_clock::now();
        chrono::duration<double> diff = end - start;
        fout << diff.count() << " ";

        start = chrono::system_clock::now();
        m -> optimizedEscapeTime(true);
        end = chrono::system_clock::now();
        diff = end - start;
        fout << diff.count() << " ";

        start = chrono::system_clock::now();
        m -> periodicallyCheckedEscapeTime(true);
        end = chrono::system_clock::now();
        diff = end - start;
        fout << diff.count() << " ";

        start = chrono::system_clock::now();
        m -> continuousColoring(true);
        end = chrono::system_clock::now();
        diff = end - start;
        fout << diff.count() << " ";

        start = chrono::system_clock::now();
        m -> histogramColoring(true);
        end = chrono::system_clock::now();
        diff = end - start;
        fout << diff.count() << "\n";
    }
}
