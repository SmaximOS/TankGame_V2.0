#pragma once
#include "point.h"
#include <map>

using namespace std;

class Tank;
class Player {
private:
    int playerID;
    int numOfTanks = 0;
    int controlledTank = 1; // -1 means no tank is selected
    map<int, Tank*> tanksMap;

public:
    Player(int id) : playerID(id) {}

    int getPlayerID() const {
        return playerID;
    }

    int getNumOfTanks() const {
        return numOfTanks;
    }

    std::map<int, Tank*>& getTanksMap() {
        return tanksMap;
    }


    void incrementTanks() {
        ++numOfTanks;
    }

    void decrementTanks() {
        if (numOfTanks > 0)
            --numOfTanks;
    }

    void setControlledTank(int tankIndex) {
        controlledTank = tankIndex;
    }

    int getControlledTank() const {
        return controlledTank;
    }

    void changeControlledTank() {
        if (tanksMap.empty()) {
            controlledTank = -1;
            return;
        }

        auto it = tanksMap.find(controlledTank);
        bool foundNext = false;

        // Start from the next tank in map
        for (auto iter = std::next(it); iter != tanksMap.end(); ++iter) {
            if (iter->second && iter->second->isTankAlive()) {
                controlledTank = iter->first;
                foundNext = true;
                break;
            }
        }

        // If we didn't find a valid tank forward, loop back to the start
        if (!foundNext) {
            for (auto iter = tanksMap.begin(); iter != it; ++iter) {
                if (iter->second && iter->second->isTankAlive()) {
                    controlledTank = iter->first;
                    break;
                }
            }
        }
    }

};
