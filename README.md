# PandemicExpansion
This is a simulation of the evolution of a pandemic using celular automaton. SFML is used for printing the simulation.

## How it works

A *WHITE* cell is considered as a frontier. This could be a road, a river, or just a isolated person. A *BLACK* cell is a person that is not infected by the virus. *BLACK* cells are capable of transmitting the virus, while *WHITE* cells cannot let the virus spread out. *RED* cells are people that are already infected and they can, therefore, transmit the virus. *GREEN* cells are people that were infected and who have recovered from it. These people cannot transmit the virus again. They act like *WHITE* cells.

## Celular automaton rules

A cellular automaton consists of a regular grid of cells, each in one of a finite number of states, in this case _frontier_, _infected_, _not infected_ and _recovered_. For each cell, a set of cells called its neighborhood is defined relative to the specified cell. An initial state (time t = 0) is selected by assigning a state for each cell. A new generation is created (advancing t by 1), according to some fixed rule (generally, a mathematical function) that determines the new state of each cell in terms of the current state of the cell and the states of the cells in its neighborhood.

To define the next iteration of the matrix, for each cell, it will change its state according to the following rules:

* A *WHITE* cell will not change state.
* A *BLACK* cell will become a *RED* cell according to the number of *RED* cells of its neighborhood. If it has eight *RED* neighbours, the probabilities of becoming a *RED* cell are 100% (the maximum number of neighbours are eight); if it has four *RED* neighbours, the probabilities are 50%.. and so on.
* A *RED* cell will become a *GREEN* cell if the time since if became a *RED* cell if greater than a random time. This is a random number to simulate different recover times.
* A *GREEN* cell will not change state.

## Simulation

![Alt Text](/Users/joaquin/Code/Coronavirus/expansion.gif)