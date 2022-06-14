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
class Inst {
   public:
    int posX, posY, sizeY, sizeX, pinNum;
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
LibCell::LibCell(int name, int tech, int sizeX, int sizeY, int pinNum) {
    name = name;
    tech = tech;
    pinNum = pinNum;
    sizeX = sizeX;
    sizeY = sizeY;
}
class Die {
    int sizeY, length, rowsizeY, tech;
    vector<vector<int>> grid;
    Die(int sizeY, int length, int rowsizeY, int tech);
};
Die::Die(int sizeY, int length, int rowsizeY, int tech) {
    sizeY = sizeY;
    length = length;
    rowsizeY = rowsizeY;
    tech = tech;
}
// int HPWL() {
// }
int main(int argc, char* argv[]) {
    int techNum;
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    char JUNK[100], junk;

    fin >> JUNK;
    fin >> techNum;
    vector<int> techDieNum(techNum);
    vector<vector<LibCell>> Lib(techNum);

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
            cout << "read:" << JUNK << junk << " " << n << '\n';
            if (n != j + 1) {
                cout << "error: input format fault" << '\n';
            }
            fin >> sizeX >> sizeY >> pinNum;
            cout << sizeX << " " << sizeY << " " << pinNum << "\n";
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
                cout << JUNK << " " << posX << " " << posY << '\n';
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

    // Todo2: Instance input
}