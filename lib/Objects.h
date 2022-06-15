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

class Terminal {
   public:
    static int width, height, spacing;
    int posX, posY;
    static void setWidth(int i);
    static void setHeight(int i);
    static void setSpacing(int i);
};

class Inst {
   public:
    int posX, posY, sizeY, sizeX, pinNum, type;
    vector<Pin> pins;
    Inst(){};
    Inst(int posX, int posY, int sizeY, int sizeX, int pinNum);
};

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
class Die {
   public:
    int lowerLeftX, lowerLeftY, higherRightX, higherRightY, gridWidth, gridHeight, tech, maxUtil, rowNum, colNum;
    vector<vector<int>> grid;
    Die(){};
    Die(int lowerLeftX, int lowerLeftY, int higherRightX, int higherRightY);
};
// int HPWL() {
// }