#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
class Pin {
   public:
    int name, posX, posY, net, relativePosX, relativePosY;
    Pin() {
        net = -1;
        name = -1;
    };
    Pin(int name, int relativePosX, int relativePosY);
};

class Terminal {
   public:
    static int width, height, spacing;
    int posX, posY;
    static void setWidth(int i);
    static void setHeight(int i);
    static void setSpacing(int i);
    Terminal(){posX=-1; posY=-1;}

    static int eqwidth();
    static int eqheight();
    static int eqarea();
};

class Inst {
   public:
    int posX, posY, sizeY, sizeX, pinNum, name, type, atdie, cr, gposX, gposY;
    vector<Pin> pins;
    list<pair<Inst*, float>> adjlist;

    Inst();
    Inst(int posX, int posY, int sizeY, int sizeX, int pinNum);
};

class LibCell {
   private:
    int type, sizeX, sizeY, pinNum, tech;

   public:
    vector<Pin>* pins;
    int getsizeX();
    int getsizeY();
    int getarea();
    int getpinNum();
    int gettech();
    int gettype();
    void setsizeX(int i);
    void setsizeY(int i);
    void setpinNum(int i);
    void settech(int i);
    void settype(int i);
    LibCell(){};
    LibCell(int type, int tech, int sizeX, int sizeY, int pinNum);
};
class Die {
   public:
    int lowerLeftX, lowerLeftY, higherRightX, higherRightY, gridWidth, gridHeight, tech, maxUtil, rowNum, colNum, instNum, area;
    vector<vector<Inst*>> grid;
    vector<Inst*> instances;
    int gridStartX;
    Die(){};
    Die(int lowerLeftX, int lowerLeftY, int higherRightX, int higherRightY);
};

class Net {
   public:
    vector<Pin*> pins;
    int name;
    int num;
};

// int HPWL() {
// }
