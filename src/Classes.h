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
    int getarea();
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
int LibCell::getarea() {
    return sizeX*sizeY;
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
