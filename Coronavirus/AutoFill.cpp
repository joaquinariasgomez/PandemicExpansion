//
//  AutoFill.cpp
//  Coronavirus
//
//  Created by José Joaquín Arias Gómez-Calcerrada on 20/03/2020.
//  Copyright © 2020 José Joaquín Arias Gómez-Calcerrada. All rights reserved.
//

#include "AutoFill.hpp"
#include <iostream>

bool outsideMatrix(int x, int y) {
    return x < 0 || y < 0 || x >= 1920/20 || y >= 1080/20;
}

void AutoFill::run() {
    this->fillElement(x_, y_);
}

void AutoFill::fillElement(int x, int y) {
    if(!outsideMatrix(x, y)) {
        if(!matrix.isAPerson(x, y)) {
            matrix.setPerson(x, y, true);
            this->fillElement(x-1, y);
            this->fillElement(x+1, y);
            this->fillElement(x, y-1);
            this->fillElement(x, y+1);
        }
    }
}
