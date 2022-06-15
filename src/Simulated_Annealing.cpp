#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Pin {
   public:
    int name, posX, posY, net;
    Pin(){};
    Pin(int name, int posX, int posY);
    Pin(int name, int posX, int posY, int net);
};
Pin::Pin(int name, int posX, int posY) {
    name = name;
    posX = posX;
    posY = posY;
}
Pin::Pin(int name, int posX, int posY, int net) {
    name = name;
    posX = posX;
    posY = posY;
    net = net;
}
class Terminal {
   public:
    static int width, height, spacing;
    int posX, posY;
    static void setWidth(int i);
    static void setHeight(int i);
    static void setSpacing(int i);
};
int Terminal::width;
int Terminal::height;
int Terminal::spacing;

class Inst {
   public:
    int posX, posY, sizeY, sizeX, pinNum, type;
    vector<Pin> pins;
    Inst(){};
    Inst(int posX, int posY, int sizeY, int sizeX, int pinNum);
};

Inst::Inst(int posX, int posY, int sizeY, int sizeX, int pinNum) {
    posX = posX;
    posY = posY;
    sizeY = sizeY;
    sizeX = sizeX;
    pinNum = pinNum;
}
class LibCell {
   private:
    int name, sizeX, sizeY, pinNum, tech;

   public:
    vector<Pin>* pins;
    int getsizeX();
    int getsizeY();
    int getpinNum();
    int gettech();
    int getname();
    void setsizeX(int i);
    void setsizeY(int i);
    void setpinNum(int i);
    void settech(int i);
    void setname(int i);
    LibCell(){};
    LibCell(int name, int tech, int sizeX, int sizeY, int pinNum);
};
int LibCell::getname() {
    return name;
}
int LibCell::getsizeX() {
    return sizeX;
}
int LibCell::getsizeY() {
    return sizeY;
}
int LibCell::getpinNum() {
    return pinNum;
}
int LibCell::gettech() {
    return tech;
}
void LibCell::setsizeX(int i) {
    sizeX = i;
}

void LibCell::setsizeY(int i) {
    sizeY = i;
}
void LibCell::setpinNum(int i) {
    pinNum = i;
}
void LibCell::settech(int i) {
    tech = i;
}
void LibCell::setname(int i) {
    name = i;
}
LibCell::LibCell(int n, int t, int X, int Y, int pn) {
    name = n;
    tech = t;
    pinNum = pn;
    sizeX = X;
    sizeY = Y;
}
class Die {
   public:
    int lowerLeftX, lowerLeftY, higherRightX, higherRightY, gridWidth, gridHeight, tech, maxUtil, rowNum, colNum;
    vector<vector<int>> grid;
    Die(){};
    Die(int lowerLeftX, int lowerLeftY, int higherRightX, int higherRightY);
};
Die::Die(int llX, int llY, int hrX, int hrY) {
    lowerLeftX = llX;
    lowerLeftY = llY;
    higherRightX = hrX;
    higherRightY = hrY;
}
// int HPWL() {
// }
void InstDeepCopy(Inst& i, LibCell& l) {
    i.pinNum = l.getpinNum();
    i.sizeX = l.getsizeX();
    i.sizeY = l.getsizeY();
}
int main(int argc, char* argv[]) {
    int techNum, instanceNum, netNum;
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    char JUNK[100], junk;
    int lowerLeftX, lowerLeftY, higherRightX, higherRightY;
    fin >> JUNK;
    fin >> techNum;
    vector<int> techDieNum(techNum);
    vector<vector<LibCell>> Lib(techNum);
    vector<Inst> instances;
    vector<int> netPinNum;
    int64_t avgWidth[2] = {0};
    // input Technology Library

    for (int i = 0; i < techNum; i++) {
        int tech;
        fin >> JUNK >> junk >> junk;
        tech = (int)junk - 65;
        fin >> techDieNum[i];

        Lib[i].resize(techDieNum[i]);
        for (int j = 0; j < techDieNum[i]; j++) {
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
    // test(pass)
    //  for (int i = 0; i < techNum; i++) {
    //      cout << "Tech" << i << ":";
    //      for (int j = 0; j < techDieNum[i]; j++) {
    //          cout << "Cell" << j << ":" << '\n';
    //          cout << "   sizeX:" << Lib[i][j].getsizeX() << '\n';
    //          cout << "   sizeY:" << Lib[i][j].getsizeY() << '\n';
    //          cout << "   firstpinX" << (*Lib[i][j].pins)[0].posX << '\n';
    //          cout << "   firstpinY" << (*Lib[i][j].pins)[0].posY << '\n';
    //      }
    //  }

    // Todo1: constraint input

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
    // for (int i = 0; i < 2; i++) {
    //     cout << "die" << i << ": " << '\n'
    //          << dies[i]->lowerLeftX << " " << dies[i]->lowerLeftY << " " << dies[i]->higherRightX << " " << dies[i]->higherRightY << " " << dies[i]->maxUtil << " " << dies[i]->tech << '\n'
    //          << dies[i]->gridHeight << " " << dies[i]->rowNum << '\n';
    // }
    fin >> JUNK >> Terminal::width >> Terminal::height >> JUNK >> Terminal::spacing;
    fin >>
        JUNK >> instanceNum;
    instances.resize(instanceNum);
    cout << instanceNum << '\n';
    for (int i = 0; i < instanceNum; i++) {
        int type;
        fin >> JUNK >> JUNK >> junk >> junk >> type;
        instances[i].type = type - 1;
        instances[i].pinNum = Lib[0][type - 1].getpinNum();
        instances[i].pins.resize(instances[i].pinNum);
        avgWidth[0] += Lib[0][type - 1].getsizeX();
        avgWidth[1] += Lib[1][type - 1].getsizeX();
    }
    // for (int i = 0; i < instanceNum; i++) {
    //     cout << instances[i].pinNum << " " << instances[i].type << "\n";
    // }
    avgWidth[0] /= instanceNum;
    avgWidth[1] /= instanceNum;
    dies[0]->gridWidth = avgWidth[0];
    dies[1]->gridWidth = avgWidth[1];
    // Nets Input
    fin >>
        JUNK >> netNum;
    cout << netNum << '\n';
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
    for (int i = 0; i < instanceNum; i++) {
        cout << "inst" << i << ":\n";
        for (int j = 0; j < instances[i].pinNum; j++) {
            cout << instances[i].pins[j].net << " " << instances[i].pins[j].name << '\n';
        }
    }
}