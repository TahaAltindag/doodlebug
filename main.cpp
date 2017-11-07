// Walter Savitch, 2014 -- CH16 Question #9 Answer ("Doodlebug")

// The goal for this project is to create a simple two-dimensional
// predator-prey simulation. In this simulation the prey are ants
// and the predators are doodlebugs. These critters live in a world
// composed of a 20 × 20 grid of cells. Only one critter may occupy
// a cell at a time. The grid is enclosed, so a critter is not
// allowed to move off the edges of the world. Time is simulated in
// time steps. Each critter performs some action every time step.

#include <controller.hpp>
#include <grid.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

// ============================== Main ==============================

int main()
{
    std::string temp;
    int count = 0;

    std::srand(std::time(NULL));  // required for random function usage

    Grid* grid = new Grid(20, 20);
    GridController controller = GridController(grid);

    // Initialize the world with 5 doodlebugs and 100 ants.
    std::cout << "Creating organisms..." << std::endl;
    controller.createAnts(100);
    controller.createDoodlebugs(5);
    controller.display();

    // After each time step, prompt the user to press
    // Enter to move to the next time step.

    while (true) {
        std::cout << "Press enter to move to the next time step.\n";
        std::getline(std::cin, temp);

        controller.step();
        count++;

        std::cout << "iterations: " << count <<std::endl;
        controller.display();
    }

    // You should see a cyclical pattern between the population of
    // predators and prey, although random perturbations may lead
    // to the elimination of one or both species.

    return 0;
}
