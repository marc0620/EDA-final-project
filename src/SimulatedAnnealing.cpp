#ifndef SA
#define SA
#include "../lib/SimulatedAnnealing.h"
#endif  // !SA

using namespace std;

void SimulatedAnnealing::pinsLookUp(Inst* a, LibCell& b) {
    if (a->type == b.gettype()) {
        // cout << a->pins.size() << " " << b.getpinNum() << endl;
        for (int i = 0; i < b.getpinNum(); i++) {
            a->pins[i].relativePosX = (*b.pins)[i].relativePosX;
            a->pins[i].relativePosY = (*b.pins)[i].relativePosY;
            // cout << "relative pin" << (*b.pins)[i].relativePosX << " " << (*b.pins)[i].relativePosY << endl;
            a->pins[i].name = (*b.pins)[i].name;
            // cout << a->pins[i].name << " ";
        }
    } else {
        cout << "error: copy wrong cell" << '\n';
    }
}

SimulatedAnnealing::SimulatedAnnealing(int nn) {
    nets.resize(nn);
    srand(time(NULL));
}

void SimulatedAnnealing::randomLayer(Die& die, vector<vector<LibCell>>& lib) {
    int r = 0, c = 0;
    rowOccupied.resize(die.rowNum);
    fill(rowOccupied.begin(), rowOccupied.end(), 0);
    if (die.instances.size() > die.colNum * die.rowNum) {
        cout << "error insts too much";
        return;
    } else {
        currentBest.resize(die.colNum);
        for (int i = 0; i < die.instNum; i++) {
            currentBest[i].resize(die.rowNum);
        }
        for (int i = 0; i < die.instances.size(); i++) {
            pinsLookUp(die.instances[i], (lib[die.tech][die.instances[i]->type]));
            if (r >= die.rowNum) {
                r = r % die.rowNum;
                c += 1;
            }
            die.grid[c][r] = die.instances[i];
            currentBest[c][r] = die.instances[i];
            die.instances[i]->posX = die.gridStartX + die.gridWidth * c;
            die.instances[i]->posY = die.gridHeight * r;
            // cout << "instPos" << die.instances[i]->posX << " " << die.instances[i]->posY << endl;
            die.instances[i]->gposX = c;
            die.instances[i]->gposY = r;
            rowOccupied[r] += die.instances[i]->sizeX;
            pinPlacer(die.instances[i]);
            for (int j = 0; j < die.instances[i]->pinNum; j++) {
                // cout << j;
                //  cout << die.instances[i]->pins[j].name << " " << die.instances[i]->pins[j].net << endl;
                int n = die.instances[i]->pins[j].net;
                if (n != -1)
                    nets[die.instances[i]->pins[j].net].pins.push_back((&(die.instances[i]->pins[j])));
            }
            r += 1;
        }
    }
    previousCost = Cost(die);
    // cout << "Cost" << previousCost;
}

void SimulatedAnnealing::pinPlacer(Inst* inst) {
    for (int i = 0; i < inst->pinNum; i++) {
        inst->pins[i].posX = inst->posX + inst->pins[i].relativePosX;
        inst->pins[i].posY = inst->posY + inst->pins[i].relativePosY;
        // cout << "x:" << inst.pins[i].relativePosY << "y:" << inst.pins[i].relativePosX;
    }
}

int SimulatedAnnealing::hpwlCalculator(vector<Net>& nets) {
    int sum = 0;
    for (int i = 0; i < nets.size(); i++) {
        // cout << "NET size" << nets[i].pins.size();
        if (nets[i].pins.size() == 0)
            continue;
        int left, right, top, bottom;
        left = nets[i].pins[0]->posX;
        right = left;
        top = nets[i].pins[0]->posY;
        bottom = top;
        for (int j = 0; j < nets[i].pins.size(); j++) {
            // cout << "PIN" << nets[i].pins[j]->posX << " " << nets[i].pins[j]->posY << endl;
            left = ((nets[i].pins[j]->posX < left) ? nets[i].pins[j]->posX : left);
            right = ((nets[i].pins[j]->posX > right) ? nets[i].pins[j]->posX : right);
            top = ((nets[i].pins[j]->posY > top) ? nets[i].pins[j]->posY : top);
            bottom = ((nets[i].pins[j]->posX < bottom) ? nets[i].pins[j]->posY : bottom);
        }
        // cout << right << " " << left << " " << top << " " << bottom << endl;
        sum += (right - left + top - bottom);
        // cout << "SUM" << endl;
    }
    return sum;
}

int SimulatedAnnealing::Cost(Die& die) {
    int hpwl;
    // int ror;
    hpwl = hpwlCalculator(nets);

    return hpwl * 500;
}

void SimulatedAnnealing::instMove(Die& die) {
    int i = rand() % (die.instances.size());
    int x = rand() % (die.colNum);
    int y = rand() % (die.rowNum);
    bool swap = false;

    int originX = die.instances[i]->gposX, originY = die.instances[i]->gposY;
    while (x == originX && y == originY) {
        x = rand() % (die.colNum);
        y = rand() % (die.rowNum);
    }
    // cout << originX << " " << originY << " " << x << " " << y << '\n';
    if (die.grid[x][y] != nullptr) {
        die.grid[x][y]->gposX = originX;
        die.grid[x][y]->gposY = originY;
        die.grid[x][y]->posX = originX * die.gridWidth;
        die.grid[x][y]->posY = originY * die.gridHeight;
        rowOccupied[originY] += (die.grid[x][y]->sizeX);
        rowOccupied[y] -= (die.grid[x][y]->sizeX);
        pinPlacer(die.grid[x][y]);
        swap = true;
    }
    die.instances[i]->gposX = x;
    die.instances[i]->gposY = y;
    die.instances[i]->posX = x * die.gridWidth;
    die.instances[i]->posY = y * die.gridHeight;
    rowOccupied[originY] -= (die.instances[i]->sizeX);
    rowOccupied[y] += (die.instances[i]->sizeX);
    pinPlacer(die.instances[i]);
    for (int i = 0; i < die.instNum; i++) {
        for (int j = 0; j < die.instances[i]->pinNum; j++) {
            // cout << "PINS" << die.instances[i]->pins[j].posX << " " << die.instances[i]->pins[j].posY << endl;
        }
    }

    if (rowOccupied[originY] >= die.higherRightX || rowOccupied[y] >= die.higherRightX) {
        currentCost = temperature * 4;
    } else
        currentCost = Cost(die);
    // cout << "sum out" << endl;
    cout << " acc " << accept() << endl;
    if (accept() == 'a') {
        if (swap) {
            currentBest[originX][originY] = die.grid[x][y];
            die.grid[originX][originY] = currentBest[originX][originY];
        } else {
            currentBest[originX][originY] = nullptr;
            die.grid[originX][originY] = nullptr;
        }
        currentBest[x][y] = die.instances[i];
        die.grid[x][y] = currentBest[x][y];
        previousCost = currentCost;
    } else if (accept() == 'b') {
        if (swap) {
            currentBest[originX][originY] = die.grid[x][y];
            die.grid[originX][originY] = die.grid[x][y];

        } else {
            currentBest[originX][originY] = nullptr;
            die.grid[originX][originY] = nullptr;
        }
        currentBest[x][y] = die.instances[i];
        die.grid[x][y] = die.instances[i];
    } else {
        if (swap) {
            die.grid[x][y]->posX = x * die.gridWidth;
            die.grid[x][y]->posY = y * die.gridHeight;
            die.grid[x][y]->gposX = x;
            die.grid[x][y]->gposY = y;
            pinPlacer(die.grid[x][y]);
        }
        die.instances[i]->posX = originX * die.gridWidth;
        die.instances[i]->posY = originY * die.gridHeight;
        die.instances[i]->gposX = originX;
        die.instances[i]->gposY = originY;
        pinPlacer(die.instances[i]);
    }
}

char SimulatedAnnealing::accept() {
    if (currentCost - previousCost <= 0)
        return 'a';
    else {
        double prob = exp(-((currentCost - previousCost) / temperature));
        // cout << "PROB " << prob << endl;
        if (prob >= 0.0184 && rand() < prob * RAND_MAX)
            return 'b';
        else
            return 'c';
    }
}

void SimulatedAnnealing::entireProcedure(Die& die, vector<vector<LibCell>>& lib) {
    randomLayer(die, lib);

    int count = 0;
    while (temperature >= 100) {
        count++;
        temperature *= 0.9;
        instMove(die);
        cout << "grid: " << endl;
        for (int j = 0; j < die.rowNum; j++) {
            for (int i = 0; i < die.colNum; i++) {
                cout << ((die.grid[i][j] != nullptr) ? die.grid[i][j]->name : -1) << " ";
            }
            cout << endl;
        }
        cout << "current best:" << endl;
        for (int j = 0; j < die.rowNum; j++) {
            for (int i = 0; i < die.colNum; i++) {
                cout << ((currentBest[i][j] != nullptr) ? currentBest[i][j]->name : -1) << " ";
            }
            cout << endl;
        }
        // cout << count << endl;
    }
    for (int j = 0; j < die.rowNum; j++) {
        for (int i = 0; i < die.colNum; i++) {
            die.grid[i][j] = currentBest[i][j];
            cout << ((die.grid[i][j] != nullptr) ? die.grid[i][j]->name : -1) << " ";
        }
        cout << endl;
    }
}
