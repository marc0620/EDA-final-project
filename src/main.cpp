#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifndef OBJ
#define OBJ
#include "../lib/Objects.h"
#endif
using namespace std;
int main(int argc, char* argv[]) {
    int techNum, instanceNum, netNum;
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    char JUNK[100], junk;
    int lowerLeftX, lowerLeftY, higherRightX, higherRightY;
    fin >> JUNK;
    fin >> techNum;
    vector<int> techInstNum(techNum);
    vector<vector<LibCell>> Lib(techNum);
    vector<Inst> instances;
    vector<int> netPinNum;
    int64_t avgWidth[2] = {0};
    // input Technology Library

    for (int i = 0; i < techNum; i++) {
        int tech;
        fin >> JUNK >> junk >> junk;
        tech = (int)junk - 65;
        fin >> techInstNum[i];

        Lib[i].resize(techInstNum[i]);
        for (int j = 0; j < techInstNum[i]; j++) {
            int n, sizeX, sizeY, pinNum;
            fin >> JUNK >> junk >> junk >> n;
            if (n != j + 1) {
                cout << "error: input format fault" << '\n';
            }
            fin >> sizeX >> sizeY >> pinNum;
            Lib[i][j].setname(j);
            Lib[i][j].setsizeX(sizeX);
            Lib[i][j].setsizeY(sizeY);
            Lib[i][j].setpinNum(pinNum);
            Lib[i][j].settech(i);
            vector<Pin>* pins = new vector<Pin>(pinNum);
            (*pins).resize(pinNum);
            for (int k = 0; k < pinNum; k++) {
                int posX, posY;
                fin >> JUNK >> JUNK >> posX >> posY;
                (*pins)[k].name = k;
                (*pins)[k].posX = posX;
                (*pins)[k].posY = posY;
            }
            Lib[i][j].pins = pins;
        }
        // cout << junk << "tech:" << tech;
    }

    // // Todo1: constraint input

    fin >> JUNK >> lowerLeftX >> lowerLeftY >> higherRightX >> higherRightY;
    Die* dies[2];
    dies[0] = new Die(lowerLeftX, lowerLeftY, higherRightX, higherRightY);
    dies[1] = new Die(lowerLeftX, lowerLeftY, higherRightX, higherRightY);
    fin >> JUNK >> dies[0]->maxUtil >> JUNK >> dies[1]->maxUtil;
    fin >> JUNK >> JUNK >> JUNK >> JUNK >> dies[0]->gridHeight >> dies[0]->rowNum;
    fin >> JUNK >> JUNK >> JUNK >> JUNK >> dies[1]->gridHeight >> dies[1]->rowNum;
    fin >> JUNK >> junk >> junk;
    dies[0]
        ->tech = (int)junk - 65;
    fin >> JUNK >> junk >> junk;
    dies[1]->tech = (int)junk - 65;
    fin >> JUNK >> Terminal::width >> Terminal::height >> JUNK >> Terminal::spacing;
    fin >> JUNK >> instanceNum;
    instances.resize(instanceNum);
    for (int i = 0; i < instanceNum; i++) {
        int type;
        fin >> JUNK >> JUNK >> junk >> junk >> type;
        instances[i].type = type - 1;
        instances[i].pinNum = Lib[0][type - 1].getpinNum();
        instances[i].pins.resize(instances[i].pinNum);
        avgWidth[0] += Lib[0][type - 1].getsizeX();
        avgWidth[1] += Lib[1][type - 1].getsizeX();
    }
    avgWidth[0] /= instanceNum;
    avgWidth[1] /= instanceNum;
    dies[0]->gridWidth = avgWidth[0];
    dies[1]->gridWidth = avgWidth[1];
    // Nets Input
    fin >> JUNK >> netNum;
    netPinNum.resize(netNum);
    for (int i = 0; i < netNum; i++) {
        fin >> JUNK >> JUNK >> netPinNum[i];
        for (int j = 0; j < netPinNum[i]; j++) {
            int targetInst, targetPin;
            fin >> JUNK >> junk >> targetInst >> junk >> junk >> targetPin;
            instances[targetInst - 1].pins[targetPin - 1].net = i;
            instances[targetInst - 1].pins[targetPin - 1].name = targetPin - 1;
        }
    }
    // for (int i = 0; i < instanceNum; i++) {
    //     cout << "inst" << i << ":\n";
    //     for (int j = 0; j < instances[i].pinNum; j++) {
    //         cout << instances[i].pins[j].net << " " << instances[i].pins[j].name << '\n';
    //     }
    // }
}