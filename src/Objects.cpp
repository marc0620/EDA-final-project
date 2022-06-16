#ifndef OBJ
#define OBJ
#include "../lib/Objects.h"
#endif
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
Pin::Pin(int name, int relativePosX, int relativePosY) {
    name = name;
    relativePosX = relativePosX;
    relativePosY = relativePosY;
}
int Terminal::width;
int Terminal::height;
int Terminal::spacing;
int Terminal::eqarea(){
    return (Terminal::width + Terminal::spacing)*(Terminal::height + Terminal::spacing);
}

Inst::Inst()
{
    pinNumused = 0;
    cr = 0;
}

Inst::Inst(int posX, int posY, int sizeY, int sizeX, int pinNum) {
    posX = posX;
    posY = posY;
    sizeY = sizeY;
    sizeX = sizeX;
    pinNum = pinNum;
    pinNumused = 0;
    cr = 0;
}

int LibCell::gettype() {
    return type;
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
void LibCell::settype(int i) {
    type = i;
}
LibCell::LibCell(int n, int t, int X, int Y, int pn) {
    type = n;
    tech = t;
    pinNum = pn;
    sizeX = X;
    sizeY = Y;
}
Die::Die(int llX, int llY, int hrX, int hrY) {
    lowerLeftX = llX;
    lowerLeftY = llY;
    higherRightX = hrX;
    higherRightY = hrY;
    area = (hrX-llX)*(hrY-llY);
}
void InstDeepCopy(Inst& i, LibCell& l) {
    i.pinNum = l.getpinNum();
    i.sizeX = l.getsizeX();
    i.sizeY = l.getsizeY();
}