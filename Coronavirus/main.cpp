#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
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

void generateNext(Matrix& automata, int& infected, int& cured, int& dead) { //Generate Values from Column "from" to Column "to"
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
    sf::sleep(sf::milliseconds(30));
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
    Matrix automata(GRID_WIDTH, GRID_HEIGHT);
    
    sf::Thread thread(std::bind(&generateNext, automata, infected, cured, dead));
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::P) {
                        isPlaying = !isPlaying;
                    }
                    if(event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;
                    case sf::Event::MouseButtonPressed:     //Maybe Released?
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        int x = double(event.mouseButton.x)/CELL_SIZE;
                        int y = double(event.mouseButton.y)/CELL_SIZE;
                        automata.draw(x, y);
                    }
                default: break;
            }
        }
 
        //Draw Grid
        drawGrid(automata, window);
        if(isPlaying) {
            generateNext(automata, infected, cured, dead);
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
