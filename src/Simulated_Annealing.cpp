#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Pin {
   public:
    int name, posX, posY, net;
};
class Inst {
   public:
    int posX, posY, height, width, pinNum;
    Inst(){};
    Inst(int posX, int posY, int);
};
class LibCell {
   private:
    int width, height, pinNumname, tech;

   public:
    vector<Pin>* pins;
    LibCell(){};
    LibCell(string name, string tech, int width, int height, int pinNum);
};
LibCell::LibCell(string name, string tech, int width, int height, int pinNum) {
    name = name;
    tech = tech;
    pinNum = pinNum;
    width = width;
    height = height;
}
class Die {
    int height, length, rowHeight, tech;
    vector<vector<int>> grid;
    Die(int height, int length, int rowHeight, int tech);
};
Die::Die(int height, int length, int rowHeight, int tech) {
    height = height;
    length = length;
    rowHeight = rowHeight;
    tech = tech;
}
int main() {
}