#ifndef OBJ
#define OBJ
#include "../src/Objects.h"
#endif
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
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
int Terminal::width;
int Terminal::height;
int Terminal::spacing;

Inst::Inst(int posX, int posY, int sizeY, int sizeX, int pinNum) {
    posX = posX;
    posY = posY;
    sizeY = sizeY;
    sizeX = sizeX;
    pinNum = pinNum;
}

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
Die::Die(int llX, int llY, int hrX, int hrY) {
    lowerLeftX = llX;
    lowerLeftY = llY;
    higherRightX = hrX;
    higherRightY = hrY;
}
void InstDeepCopy(Inst& i, LibCell& l) {
    i.pinNum = l.getpinNum();
    i.sizeX = l.getsizeX();
    i.sizeY = l.getsizeY();
}