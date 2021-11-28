#include <SFML/Graphics.hpp>
#include "stack.h"
#include "mandelbrot.h"
#include "run.h"
#include <time.h>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

int main()
{
    /*
    runtimeCalculator();
    */

    srand(time(0));
    // Making an SFML window of size 1000 x 1000
    int width = 1000;
    int height = 1000;

    // Making a color palette with p colors (where p is a random integer from 499 to 999)
    int p = 499 + rand() % 501;
    // q, r, s are random integers in range [2, 999], used to create RGB values in the color palette
    int q = 2 + rand() % 998;
    int r = 2 + rand() % 998;
    int s = 2 + rand() % 998;

    // Initial precision of Mandelbrot Set kept at 100 iterations
    int precision = 100;

    MandelbrotSet* m = new MandelbrotSet(p, width, height, q, r, s, precision);

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");
    // Creating an initial Mandelbrot Set and depicting it on the window
    m -> Algorithm(true);

    // Various text and button elements implemented below

    // Text to show current zoom level (restricted to lie in [1, 62])
    sf::Text text;
    // Font used is BRITANIC.ttf, which must be present in the parent directory
    sf::Font font;
    if(!font.loadFromFile("BRITANIC.ttf")) {
        return -1;
    }

    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    string zoom_string = to_string(m -> get_zoom()) + "x";
    text.setFont(font);
    text.setString(zoom_string);
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    text.setPosition(10.0f, 20.0f);

    // Text to show current precision level (depicted in percentage as precision/10 %)
    sf::Text precision_text;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    precision_text.setFont(font);
    string precision_string = to_string(m -> get_precision() / 10) + "%";
    precision_text.setString(precision_string);
    precision_text.setCharacterSize(60);
    precision_text.setFillColor(sf::Color::Red);
    precision_text.setStyle(sf::Text::Bold);
    precision_text.setPosition(10.0f, 90.0f);

    // Circular button for zooming out of the Mandelbrot Set
    // Uses the stack to find the previous version, and depicts it on the screen
    sf::CircleShape zoomout;
    // Radius of button, and its position on the screen
    double radius = 40;
    double centerX = 3 * radius;
    double centerY = height - 3 * radius;
    zoomout.setRadius(radius);
    // Original color of the button (changes on hover and click)
    sf::Color color(255, 220, 200);
    zoomout.setFillColor(color);
    zoomout.setOutlineColor(sf::Color::Red);
    zoomout.setOutlineThickness(5);
    zoomout.setPosition(centerX, centerY);

    // Text on the zoom out button (shows - sign)
    sf::Text zoomout_text;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    zoomout_text.setFont(font);
    zoomout_text.setString("-");
    zoomout_text.setCharacterSize(80);
    zoomout_text.setFillColor(sf::Color::Red);
    zoomout_text.setStyle(sf::Text::Bold);
    zoomout_text.setPosition(centerX + 3 * radius / 4.0, centerY - radius / 2.0);

    // Rectangular button to implement color changing
    // Changes palette and re-renders image on being selected
    sf::RectangleShape colorChange;
    // Width and height of the button, as well as its position
    double rect_width = 100;
    double rect_height = 50;
    double posX = width - 2 * rect_width;
    double posY = 2 * rect_height;
    colorChange.setSize(sf::Vector2f(rect_width, rect_height));
    // Original color of the button (changes on hover and click)
    sf::Color color1(255, 220, 200);
    colorChange.setFillColor(color1);
    colorChange.setOutlineColor(sf::Color::Red);
    colorChange.setOutlineThickness(5);
    colorChange.setPosition(posX, posY);

    // Texts on the Color change button (shows "Change Palette" written)
    sf::Text change_text1;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    change_text1.setFont(font);
    change_text1.setString("Change");
    change_text1.setCharacterSize(20);
    change_text1.setFillColor(sf::Color::Red);
    change_text1.setPosition(posX + rect_width / 8.0, posY);

    sf::Text change_text2;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    change_text2.setFont(font);
    change_text2.setString("Palette");
    change_text2.setCharacterSize(20);
    change_text2.setFillColor(sf::Color::Red);
    change_text2.setPosition(posX + rect_width / 8.0, posY + rect_height / 2.0);

    // Rectangular button to implement increasing precision
    // Increases max iterations and uses corresponding algorithm to re-render the image on being selected
    sf::RectangleShape incIterations;
    // Width and height of the button, as well as its position
    double rect_iter_width = 120;
    double rect_iter_height = 50;
    double iterX = width - 3 * rect_iter_width;
    double iterY = height - 2 * rect_iter_height;
    incIterations.setSize(sf::Vector2f(rect_iter_width, rect_iter_height));
    // Original color of the button (changes on hover and click)
    sf::Color color2(255, 220, 200);
    incIterations.setFillColor(color2);
    incIterations.setOutlineColor(sf::Color::Red);
    incIterations.setOutlineThickness(5);
    incIterations.setPosition(iterX, iterY);

    // Texts on the Increase precision button (shows "Increase Precision" written)
    sf::Text iter_text1;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    iter_text1.setFont(font);
    iter_text1.setString("Increase");
    iter_text1.setCharacterSize(20);
    iter_text1.setFillColor(sf::Color::Red);
    iter_text1.setPosition(iterX + rect_iter_width / 8.0, iterY);

    sf::Text iter_text2;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    iter_text2.setFont(font);
    iter_text2.setString("Precision");
    iter_text2.setCharacterSize(20);
    iter_text2.setFillColor(sf::Color::Red);
    iter_text2.setPosition(iterX + rect_iter_width / 8.0, iterY + rect_iter_height / 2.0);

    // Rectangular button to implement decreasing precision
    // Decreases max iterations and uses corresponding algorithm to re-render the image on being selected
    sf::RectangleShape decIterations;
    // Width and height of the button, as well as its position
    double rect_dec_width = 120;
    double rect_dec_height = 50;
    double dec_iterX = width - 3 * rect_dec_width / 2.0;
    double dec_iterY = height - 2 * rect_dec_height;
    decIterations.setSize(sf::Vector2f(rect_dec_width, rect_dec_height));
    // Original color of the button (changes on hover and click)
    sf::Color color3(255, 220, 200);
    decIterations.setFillColor(color3);
    decIterations.setOutlineColor(sf::Color::Red);
    decIterations.setOutlineThickness(5);
    decIterations.setPosition(dec_iterX, dec_iterY);

    // Texts on the Decrease precision button (shows "Decrease Precision" written)
    sf::Text dec_iter_text1;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    dec_iter_text1.setFont(font);
    dec_iter_text1.setString("Decrease");
    dec_iter_text1.setCharacterSize(20);
    dec_iter_text1.setFillColor(sf::Color::Red);
    dec_iter_text1.setPosition(dec_iterX + rect_dec_width / 8.0, dec_iterY);

    sf::Text dec_iter_text2;
    // Setting various features of the text (such as font, font size, color, bold and position on the screen)
    dec_iter_text2.setFont(font);
    dec_iter_text2.setString("Precision");
    dec_iter_text2.setCharacterSize(20);
    dec_iter_text2.setFillColor(sf::Color::Red);
    dec_iter_text2.setPosition(dec_iterX + rect_dec_width / 8.0, dec_iterY + rect_dec_height / 2.0);

    while (window.isOpen())
    {
        // Change all colors back to their initial green values
        // Used to implement color change on clicking button
        color.g = 220;
        zoomout.setFillColor(color);

        color1.g = 220;
        colorChange.setFillColor(color1);

        color2.g = 220;
        incIterations.setFillColor(color2);

        color3.g = 220;
        decIterations.setFillColor(color3);

        // Check if an event has happened (such as mouse click or mouse movement or closing the window)
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Checking mouse movement to change colors of buttons on hover
            if (event.type == sf::Event::MouseMoved) {
                // Current position of mouse pointer
                double mouseX = (double) event.mouseMove.x;
                double mouseY = (double) event.mouseMove.y;

                // Distance between mouse and center of circular zoom out button
                double distX = mouseX - (centerX + radius);
                double distY = mouseY - (centerY + radius);
                double distance = distX * distX + distY * distY;

                // If mouse points inside circular button then change color
                if(distance <= radius * radius) {
                    color.g = 220;
                    color.b = 255;
                    zoomout.setFillColor(color);
                }

                // If mouse is outside the button then restore its color to the original value
                else {
                    color.b = 200;
                    zoomout.setFillColor(color);
                }

                // Check if mouse is inside the rectangle for changing palette
                // If yes, then change color, else restore to original color
                double distX_rect = mouseX - posX;
                double distY_rect = mouseY - posY;
                bool in_rect_width = distX_rect >= 0 && distX_rect <= rect_width;
                bool in_rect_height = distY_rect >= 0 && distY_rect <= rect_height;
                bool in_rect = in_rect_width && in_rect_height;

                if(in_rect) {
                    color1.g = 220;
                    color1.b = 255;
                    colorChange.setFillColor(color1);
                }

                else {
                    color1.b = 200;
                    colorChange.setFillColor(color1);
                }

                // Check if mouse is inside the rectangle for increasing precision
                // If yes, then change color, else restore to original color
                double distX_iter = mouseX - iterX;
                double distY_iter = mouseY - iterY;
                bool in_iter_width = distX_iter >= 0 && distX_iter <= rect_iter_width;
                bool in_iter_height = distY_iter >= 0 && distY_iter <= rect_iter_height;
                bool in_iter = in_iter_width && in_iter_height;

                if(in_iter) {
                    color2.g = 220;
                    color2.b = 255;
                    incIterations.setFillColor(color2);
                }

                else {
                    color2.b = 200;
                    incIterations.setFillColor(color2);
                }

                // Check if mouse is inside the rectangle for decreasing precision
                // If yes, then change color, else restore to original color
                double distX_dec = mouseX - dec_iterX;
                double distY_dec = mouseY - dec_iterY;
                bool in_dec_width = distX_dec >= 0 && distX_dec <= rect_dec_width;
                bool in_dec_height = distY_dec >= 0 && distY_dec <= rect_dec_height;
                bool in_dec = in_dec_width && in_dec_height;

                if(in_dec) {
                    color3.g = 220;
                    color3.b = 255;
                    decIterations.setFillColor(color3);
                }

                else {
                    color3.b = 200;
                    decIterations.setFillColor(color3);
                }
            }

            // Checking for events of type mouse click
            if (event.type == sf::Event::MouseButtonPressed) {
                // Only working for left mouse click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Current position of mouse pointer
                    double mouseX = (double) event.mouseButton.x;
                    double mouseY = (double) event.mouseButton.y;

                    // Distance between mouse and center of circular zoom out button
                    double distX = mouseX - (centerX + radius);
                    double distY = mouseY - (centerY + radius);
                    double distance = distX * distX + distY * distY;

                    // Check if mouse is inside the rectangle for changing palette
                    double distX_rect = mouseX - posX;
                    double distY_rect = mouseY - posY;
                    bool in_rect_width = distX_rect >= 0 && distX_rect <= rect_width;
                    bool in_rect_height = distY_rect >= 0 && distY_rect <= rect_height;
                    bool in_rect = in_rect_width && in_rect_height;

                    // Check if mouse is inside the rectangle for increasing precision
                    double distX_iter = mouseX - iterX;
                    double distY_iter = mouseY - iterY;
                    bool in_iter_width = distX_iter >= 0 && distX_iter <= rect_iter_width;
                    bool in_iter_height = distY_iter >= 0 && distY_iter <= rect_iter_height;
                    bool in_iter = in_iter_width && in_iter_height;

                    // Check if mouse is inside the rectangle for decreasing precision
                    double distX_dec = mouseX - dec_iterX;
                    double distY_dec = mouseY - dec_iterY;
                    bool in_dec_width = distX_dec >= 0 && distX_dec <= rect_dec_width;
                    bool in_dec_height = distY_dec >= 0 && distY_dec <= rect_dec_height;
                    bool in_dec = in_dec_width && in_dec_height;

                    // If zoom out button was clicked
                    if(distance <= radius * radius) {
                        // Change color (creates blinking effect)
                        color.g = 170;
                        color.b = 255;
                        zoomout.setFillColor(color);

                        // Calling zoom out function from the MandelbrotSet class
                        m -> zoom_out();
                    }

                    // If change palette button was clicked
                    else if(in_rect) {
                        // Change color (creates blinking effect)
                        color1.g = 170;
                        color1.b = 255;
                        colorChange.setFillColor(color1);
                        window.draw(colorChange);
                        window.draw(change_text1);
                        window.draw(change_text2);

                        // Forming new random values for p, q, r, s
                        p = 499 + rand() % 501;
                        q = 2 + rand() % 998;
                        r = 2 + rand() % 998;
                        s = 2 + rand() % 998;

                        // Changing color of the palette and re-rendering the image
                        m -> change_colors(p, q, r, s);
                    }

                    // If increase precision button was clicked
                    else if(in_iter) {
                        // Change color (creates blinking effect)
                        color2.g = 170;
                        color2.b = 255;
                        incIterations.setFillColor(color2);
                        window.draw(incIterations);
                        window.draw(iter_text1);
                        window.draw(iter_text2);

                        // Calling inc precision function from the MandelbrotSet class
                        m -> inc_precision();
                    }

                    // If decrease precision button was clicked
                    else if(in_dec) {
                        // Change color (creates blinking effect)
                        color3.g = 170;
                        color3.b = 255;
                        decIterations.setFillColor(color3);
                        window.draw(decIterations);
                        window.draw(dec_iter_text1);
                        window.draw(dec_iter_text2);

                        // Calling dec precision function from the MandelbrotSet class
                        m -> dec_precision();
                    }

                    // Otherwise the mouse was clicked at some random point in the window
                    // Then we zoom in (centering at the clicked point) and re-render the image
                    else {
                        m -> zoom_in(mouseX, mouseY);
                        m -> Algorithm(true);
                    }

                    // Since click might change zoom or precision values, so re-writing those texts
                    zoom_string = to_string(m -> get_zoom()) + "x";
                    text.setString(zoom_string);

                    precision_string = to_string(m -> get_precision() / 10) + "%";
                    precision_text.setString(precision_string);
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Drawing all elements of the program (such as the Set itself and various texts and buttons)
        window.clear();
        window.draw(m -> get_points());
        window.draw(text);
        window.draw(precision_text);
        window.draw(zoomout);
        window.draw(zoomout_text);
        window.draw(colorChange);
        window.draw(change_text1);
        window.draw(change_text2);
        window.draw(incIterations);
        window.draw(iter_text1);
        window.draw(iter_text2);
        window.draw(decIterations);
        window.draw(dec_iter_text1);
        window.draw(dec_iter_text2);
        window.display();
    }

    return 0;
}
