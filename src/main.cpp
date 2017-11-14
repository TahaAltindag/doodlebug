// Walter Savitch, 2014 -- CH16 Question #9 Answer ("Doodlebug")

// The goal for this project is to create a simple two-dimensional
// predator-prey simulation. In this simulation the prey are ants
// and the predators are doodlebugs. These critters live in a world
// composed of a 2D grid of cells. Only one critter may occupy
// a cell at a time. The grid is enclosed, so a critter is not
// allowed to move off the edges of the world. Time is simulated in
// time steps. Each critter performs some action every time step.
// You should see a cyclical pattern between the population of
// predators and prey, although random perturbations may lead
// to the elimination of one or both species.

#include "controller.hpp"
#include "grid.hpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

// debugging only:
#include<mach/mach.h>
struct task_basic_info t_info;

void debug_info()
{
    // debugging for memory leaks only:
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)&t_info, 
                                  &t_info_count)) {
        exit(-1);
    }
    std::cout << "resident memory size is: " << t_info.resident_size << std::endl;
    std::cout << "virtual memory size is: " << t_info.virtual_size << std::endl;
}

int main()
{
    std::ofstream out_stream;
    std::string temp;
    int count = 0;

    out_stream.open("out/doodlebug.csv");
    if (out_stream.fail())
    {
        std::cout << "Output file in out/ failed to open.\n";
        exit(1);
    }
    out_stream << "iteration,doodlebugs,ants\n";  // csv header

    std::srand(std::time(NULL));  // required for random function usage

    Grid* grid = new Grid(20, 20);
    GridController controller = GridController(grid);

    // Initialize the world with 5 doodlebugs and 100 ants.
    std::cout << "Creating organisms..." << std::endl;
    controller.createAnts(100);
    controller.createDoodlebugs(5);
    controller.display(out_stream, count);

    // After each time step, prompt the user to press
    // Enter to move to the next time step.

    while (true) {
        std::cout << "Press enter to move to the next time step.\n";
        std::getline(std::cin, temp);

        controller.step();
        count++;

        std::cout << "iterations: " << count <<std::endl;
        controller.display(out_stream, count);
        debug_info();
    }

    out_stream.close();
}
