//
//  AutoFill.hpp
//  Coronavirus
//
//  Created by José Joaquín Arias Gómez-Calcerrada on 20/03/2020.
//  Copyright © 2020 José Joaquín Arias Gómez-Calcerrada. All rights reserved.
//

#ifndef AutoFill_hpp
#define AutoFill_hpp
#include <list>
#include "matrix.hpp"

class AutoFill {
public:
    AutoFill(const Matrix& automata, int x, int y): matrix(automata), x_(x), y_(y) {}
    void run();
    void fillElement(int x, int y);
    Matrix getMatrix() const {return matrix;}
private:
    Matrix matrix;
    int x_;
    int y_;
};

#endif /* AutoFill_hpp */
