#ifndef matrix_hpp
#define matrix_hpp
#include <time.h>
#include <stdlib.h>
#include <iostream>

class Person {
public:
    Person(): isPerson_(true), notInfected(true), cured(false), infectedIterations(0) {}
    Person(bool isPerson): isPerson_(isPerson), notInfected(true), cured(false), infectedIterations(0) {}
    Person(const Person& p) = default;
    bool isPerson() const {return isPerson_;}
    bool isNotInfected() const {return notInfected;}
    bool isCured() const {return cured;}
    int getInfectedIterations() const {return infectedIterations;}
    void setInfected(bool condition) {this->notInfected = condition;}
    void setCured(bool condition) {this->cured = condition;}
    void setPerson(bool condition) {this->isPerson_ = condition;}
    void incrementInfectedIterations() {this->infectedIterations++;}
private:
    bool isPerson_;
    bool notInfected;
    bool cured;
    bool dead;
    int infectedIterations;
};

class Matrix {
public:
    Matrix(int width, int height): width(width), height(height) {
        m = new Person*[width];
        for(int i=0;i<width;++i)
            m[i] = new Person[height];
        this->Initialize();
    }
    Matrix(const Matrix& m) = default;
    
    void Initialize() {
        //srand (time(NULL));
        for(int x=0; x<width; ++x) {
            for(int y=0; y<height; ++y) {
                if(rand()%3==0) {
                    m[x][y] = Person(false);  //There is not a person here
                }
                else {
                    m[x][y] = Person();  //There is a person here
                }
            }
        }
    }
    
    void clean() {
        for(int x=0; x<width; ++x) {
            for(int y=0; y<height; ++y) {
                m[x][y] = Person(false);  //There is not a person here
            }
        }
    }
    
    void setPerson(int x, int y, bool condition) {
        m[x][y].setPerson(condition);
    }
    
    bool isAPerson(int x, int y) const {
        return m[x][y].isPerson();
    }
    
    bool isNotInfected(int x, int y) const {
        return m[x][y].isNotInfected();
    }
    
    bool isCured(int x, int y) const {
        return m[x][y].isCured();
    }
    
    void infect(int x, int y) {
        if(m[x][y].isPerson()) {
            if(m[x][y].isNotInfected()) {
                m[x][y].setInfected(false);
            }
            else {
                m[x][y].setInfected(true);
            }
        }
    }
    
    void draw(int x, int y) {
        if(m[x][y].isPerson()) return;
        m[x][y].setPerson(true);
    }
    
    int getNeighbour(int x, int y, int addX, int addY) const {
        int addX2=addX;
        int addY2=addY;
        if(x==(width-1) && addX == 1) {
            //addX2 = -x;
            return 0;
        }
        if(x==0 && addX == -1) {
            //addX2 = width - 1;
            return 0;
        }
        if(y==(height-1) && addY == 1) {
            //addY2 = -y;
            return 0;
        }
        if(y==0 && addY == -1) {
            //addY2 = height - 1;
            return 0;
        }
        if(!m[x+addX2][y+addY2].isNotInfected()) {
            return 1;
        }
        else {
            return 0;
        }
    }
    
    void set(int x, int y, Person person) {
        m[x][y] = person;
    }
    
    Person getSucessorLife(int x, int y, int& infectedIncrement, int& curedIncrement, int& deadIncrement) {
        //srand(time(NULL));
        if(!m[x][y].isPerson()) {
            return m[x][y];
        }
        if(m[x][y].isCured()) {
            return m[x][y];
        }
        if(!m[x][y].isNotInfected()) {    //Already infected person
            m[x][y].incrementInfectedIterations();
            int randomTime = 100*(rand()%100) + 125;
            if(m[x][y].getInfectedIterations() > randomTime) {
                ++curedIncrement;
                //--infectedIncrement;
                m[x][y].setCured(true);
                m[x][y].setInfected(true);
            }
            return m[x][y];
        }
        
        int numInfectedNeighbour = 0;
        numInfectedNeighbour = this->getNeighbour(x, y, -1, 0) + this->getNeighbour(x, y, 1, 0) + this->getNeighbour(x, y, 0, -1) + this->getNeighbour(x, y, 0, 1) + this->getNeighbour(x, y, 1, 1) + this->getNeighbour(x, y, -1, -1) + this->getNeighbour(x, y, 1, -1) + this->getNeighbour(x, y, -1, 1);

        float probOfGettinInfected = numInfectedNeighbour * 12.5f;    //0 - 100 from 0 - 8
        float prob = rand()%100;
        if(x==0 && y==0) {
            std::cout << prob << std::endl;
        }
        bool isGettinInfected = prob < probOfGettinInfected;

        if(isGettinInfected) {
            Person person = new Person(true);
            person.setInfected(false);
            ++infectedIncrement;
            return person;   //Infected person
        }
        else {
            return m[x][y];     //Not infected person
        }
    }
private:
    int width;
    int height;
    Person** m;
};

#endif /* matrix_hpp */
