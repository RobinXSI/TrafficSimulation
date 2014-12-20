#include <iostream>
#include <thread>
#include <random>


const int QUEUE_LENGTH = 10; // LL
const int STREET_COUNT = 4; // NN
const int PHASE_DURATION = 4; // PP
const int NUMBER_OF_PHASES = 4; // NPHASES

const double NEW_CAR_PROBABILITY = 0.4; // Wahrscheinlichkeit dass neues Auto hinzugefügt wird

const int TIME_ITERATIONS = 1000;

const int EMPTY = -1;

int carPositions[STREET_COUNT][QUEUE_LENGTH]; // cells[LL][NN] : Achtung andersherum in dieser Implementation
int tempCarPositions[STREET_COUNT][QUEUE_LENGTH]; //tmpCells[LL][NN]

void createTestData() {

    for(int streetNumber = 0; streetNumber < STREET_COUNT; streetNumber++) {
        for (int queuePosition = 0; queuePosition < QUEUE_LENGTH; queuePosition++) {
            carPositions[streetNumber][queuePosition] = EMPTY;
            tempCarPositions[streetNumber][queuePosition] = EMPTY;
        }
    }
}

void outputData(int actualTimeIteration) {
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << actualTimeIteration << ". Iteration\n";

    for(int streetNumber = 0; streetNumber < STREET_COUNT; streetNumber++) {
        for (int queuePosition = 0; queuePosition < QUEUE_LENGTH; queuePosition++) {
            if (carPositions[streetNumber][queuePosition] != EMPTY) {
                // if there is a vehicle, output its destination:
                std::cout << "O ";
            }
            else {
                // else output an empty space:
                std::cout << "_ ";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

bool allowed(int from, int to, int phase) {
    switch(phase) {
        case 0 : {
            if(from == 0 && (to == 2 || to == 3)) {
                return true;
            }
            if(from == 2 && (to == 0 || to == 1)) {
                return true;
            }
            return false;
        }
        case 1 : {
            if(from == 1 && (to == 0 || to == 3)) {
                return true;
            }
            if(from == 3 && (to == 1 || to == 2)) {
                return true;
            }
            return false;
        }
        case 2 : {
            if(from == 0 && to == 1) {
                return true;
            }
            if(from == 2 && to == 3) {
                return true;
            }
            return false;
        }
        case 3 : {
            if(from == 1 && to == 2) {
                return true;
            }
            if(from == 3 && to == 0) {
                return true;
            }
            return false;
        }
        default : return false;
    }
}

int main() {

    createTestData();
    // go through time:
    for(int actualTimeIteration = 0; actualTimeIteration < TIME_ITERATIONS; actualTimeIteration++) {

        outputData(actualTimeIteration);

        // traffic signal
        int trafficSignalPhase = (actualTimeIteration / PHASE_DURATION) % NUMBER_OF_PHASES;


        // go through all streets:
        for(int streetNumber = 0; streetNumber < STREET_COUNT; streetNumber++) {
            // enter a vehicle to the roundabout if this is possible
            // Enter im Sinn von Hinzufügen --> drand48() gibt bei jedem durchlauf die gleiche Zahl aus
            if(carPositions[streetNumber][0] == EMPTY && drand48() < NEW_CAR_PROBABILITY) {
                // Select a number between 0 and STREET_COUNT - 2:
                // STREET_COUNT - 1 wird gesetzt falls Zufallsstrasse = gewählte Strasse
                int destination = int((double)(STREET_COUNT - 1) * drand48());

                // if self is selected, use NN - 1:
                if(destination == streetNumber) {
                    destination = STREET_COUNT - 1;
                }

                tempCarPositions[streetNumber][0] = destination;
                std::cout << "Auto hinzugefuegt auf Strasse " << streetNumber << "\n";
            }

            // go through all cells EXCEPT cell closest to intersection:
            for(int queuePosition = 0; queuePosition < QUEUE_LENGTH - 1; queuePosition++) {
                if(carPositions[streetNumber][queuePosition] != EMPTY) {
                    if(carPositions[streetNumber][queuePosition + 1] == EMPTY) {
                        tempCarPositions[streetNumber][queuePosition + 1] = carPositions[streetNumber][queuePosition];
                    } else {
                        tempCarPositions[streetNumber][queuePosition] = carPositions[streetNumber][queuePosition];
                    }
                }
            }

            // special treatment for last cell:
            if(carPositions[streetNumber][QUEUE_LENGTH - 1] != EMPTY) {
                int destination = carPositions[streetNumber][QUEUE_LENGTH - 1];
                // if movement NOT allowed, keep vehicle:
                if(!allowed(streetNumber, destination, trafficSignalPhase)) {
                    tempCarPositions[streetNumber][9] = carPositions[streetNumber][9];
                }
            }
        }

        for(int streetNumber = 0; streetNumber < STREET_COUNT; streetNumber++) {
            for(int queuePosition = 0; queuePosition < QUEUE_LENGTH; queuePosition++) {
                carPositions[streetNumber][queuePosition] = tempCarPositions[streetNumber][queuePosition];
                tempCarPositions[streetNumber][queuePosition] = EMPTY;
            }
        }
    }

    return 0;
}





