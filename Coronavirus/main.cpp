#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "matrix.hpp"

#include "ResourcePath.hpp"

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define RED sf::Color::Red
#define GREEN sf::Color::Green
#define GRAY sf::Color(153, 153, 153)

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int CELL_SIZE = 20;
const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);
int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

void drawGrid(Matrix& automata, sf::RenderWindow& window) {
    // Clear screen
    window.clear();
    
    for(int x = 0; x<GRID_WIDTH; ++x) {
        for(int y = 0; y<GRID_HEIGHT; ++y) {
            sf::RectangleShape cell;
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setSize(CELL_VECTOR);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(GRAY);
            
            if(automata.isAPerson(x, y)) {
                if(automata.isNotInfected(x, y)) {
                    if(automata.isCured(x, y)) {
                        cell.setFillColor(GREEN);
                    }
                    else {
                        cell.setFillColor(BLACK);
                    }
                }
                else {
                    cell.setFillColor(RED);
                }
            }
            else {
                cell.setFillColor(WHITE);
            }
            
            window.draw(cell);
        }
    }
    
    // Update the window
    window.display();
}

void generateNext(Matrix& automata, int& infected, int& cured, int& dead, bool isPlaying) { //Generate Values from Column "from" to Column "to"
    Matrix next(GRID_WIDTH, GRID_HEIGHT);
    int infectedIncrement = 0;
    int curedIncrement = 0;
    int deadIncrement = 0;

    for(int x=0; x<GRID_WIDTH; ++x) {
        for(int y=0; y<GRID_HEIGHT; ++y) {
            Person person = automata.getSucessorLife(x, y, infectedIncrement, curedIncrement, deadIncrement);
            next.set(x, y, person);
        }
    }
    infected += infectedIncrement;
    cured += curedIncrement;
    dead += deadIncrement;
    
    automata = next;
    if(isPlaying) {
        sf::sleep(sf::milliseconds(30));
    }
}

int distance(std::pair<int, int> border, std::pair<int, int> lastBorder) {
    return std::max(abs(border.first - lastBorder.first), abs(border.second - lastBorder.second));
}

void drawFromTo(Matrix& automata, std::pair<int, int> from, std::pair<int, int> to) {
    for(int xpos=from.first+1; xpos<to.first; ++xpos) {
        automata.draw(xpos, from.second);
    }
}

void drawBorder(Matrix& automata, std::pair<int, int> border, std::pair<int, int> lastBorder) {
    int x = border.first;
    int y = border.second;
    if(x<0 || x>=(WINDOW_WIDTH/CELL_SIZE) || y<0 || y>=(WINDOW_HEIGHT/CELL_SIZE)) return;
    automata.draw(x, y);
    if(distance(border, lastBorder) > 1 && lastBorder != std::make_pair(-1000, -1000)) {
        //std::cout << "Distance: " << distance(border, lastBorder) << std::endl;
        // Draw border from "lastBorder" to "border"
        //std::cout << "Value: " << lastBorder.first << ": ";
        if(border.first > lastBorder.first) {
            drawFromTo(automata, lastBorder, border);
        }
        else {
            drawFromTo(automata, border, lastBorder);
        }
        
        //std::cout << std::endl;
    }
}

int main()
{
    int infected = 0;
    int cured = 0;
    int dead = 0;
    
    std::ofstream infectedFile("/Users/joaquin/Code/Coronavirus/Coronavirus/infectedFile");
    std::ofstream curedFile("/Users/joaquin/Code/Coronavirus/Coronavirus/curedFile");
    std::ofstream deadFile("/Users/joaquin/Code/Coronavirus/Coronavirus/deadFile");
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Pandemic expansion");

    bool isPlaying = true;
    bool mousePressed = false;
    bool drawMode = false;
    std::pair<int, int> lastBorder = std::make_pair(-1000, -1000);
    Matrix automata(GRID_WIDTH, GRID_HEIGHT);
    
    sf::Thread thread(std::bind(&generateNext, automata, infected, cured, dead, isPlaying));
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::P) {
                    isPlaying = !isPlaying;
                }
                if(event.key.code == sf::Keyboard::D) {
                    drawMode = !drawMode;
                }
                if(event.key.code == sf::Keyboard::C) {
                    automata.clean();
                }
                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                mousePressed = true;
                if(event.mouseButton.button == sf::Mouse::Left && !drawMode) {
                    int x = double(event.mouseButton.x)/CELL_SIZE;
                    int y = double(event.mouseButton.y)/CELL_SIZE;
                    automata.infect(x, y);
                }
            }
            else {
                if(event.type == sf::Event::MouseButtonReleased) {
                    mousePressed = false;
                    lastBorder = std::make_pair(-1000, -1000);
                }
            }
        }
        
        if(drawMode && mousePressed) {  // Draw point
            int x = sf::Mouse::getPosition(window).x/CELL_SIZE;
            int y = sf::Mouse::getPosition(window).y/CELL_SIZE;
            drawBorder(automata, std::make_pair(x, y), lastBorder);
            lastBorder = std::make_pair(x, y);
        }
 
        //Draw Grid
        drawGrid(automata, window);
        if(isPlaying) {
            generateNext(automata, infected, cured, dead, isPlaying);
            //std::cout << "Dead: " << dead << " Cured: " << cured << std::endl;
            infectedFile << infected << ", ";
            curedFile << cured << ", ";
            deadFile << dead << ", ";
        }
    }
    infectedFile.close();
    curedFile.close();
    deadFile.close();

    return EXIT_SUCCESS;
}
