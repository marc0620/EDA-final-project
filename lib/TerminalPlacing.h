#ifndef OBJ
#define OBJ
#include "../lib/Objects.h"
#endif
const int sentinel = 2147483647 ;

class SquareofNet
{
    public:
     int llx,lly,hrx,hry;
     SquareofNet(){llx = lly = sentinel; hrx = hry = -1*sentinel;}
     int getmidx(){return (llx+hrx)/2;}
     int getmidy(){return (lly+hry)/2;}
};

class Terminalplacement
{
    public:
     Terminalplacement(){}
     void showterminalneed(vector<bool>);
     void Terminal_Placing(vector<Inst*>, vector<list<Inst*>>*, vector<vector<LibCell> >*, Die* );
};