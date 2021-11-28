# CS293 Project - Mandelbrot Set Zoom

This project was done as part of the course CS293 Data Structures and Algorithms Lab. To run this project the host machine must have SFML installed.

All points in the Design document were completed, along with some more.
All details regarding the code are present as comments in the code files themselves.

The project consists of the following files:
1. stack.h
2. stack.cpp
3. mandelbrot.h
4. mandelbrot.cpp
5. run.h
6. run.cpp
7. plot.py
8. main.cpp
9. runtimes.txt
10. runtime.png

It also consists of the Design document (as submitted during the pre-submission), Recording.txt and this README.

The project can be run only if the system has SFML (I compiled the project on CodeBlocks).

After comparing the runtime of the 5 different algorithms, I came up with the following observations:
1. Histogram coloring was slowest, and checking periodicity made algorithms super-fast
2. As we go from slowest to fastest algorithms, we tend to lose precision and beauty to some extent

So for making Sets with larger max iterations, it is preferable to use the faster algorithms.