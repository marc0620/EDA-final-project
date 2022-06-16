#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifndef OBJ
#define OBJ
#include "../lib/Objects.h"
#endif
#ifndef SA
#define SA
#include "../lib/SimulatedAnnealing.h"
#endif  // !SA
using namespace std;

void Partition(vector<Inst>* , vector<vector<LibCell> >, Die*, Die*, vector<list<Inst*> >, vector<Inst*>*, vector<Inst*>*);
void showtwodie(vector<Inst*>,vector<Inst*>);

/*
void showterminalneed(vector<bool> needterminal)
{
    for(int i=0;i<needterminal.size();i++)
    {
        cout<<"Net "<<i+1;
        if(needterminal[i])
        cout<<" need a terminal."<<endl;
        else
        cout<<" do not need a terminal."<<endl;
    }
}

const int sentinel = 2147483647 ;

class SquareofNet
{
    public:
     int llx,lly,hrx,hry;
     SquareofNet(){llx = lly = sentinel; hrx = hry = -1*sentinel;}
     int getmidx(){return (llx+hrx)/2;}
     int getmidy(){return (lly+hry)/2;}
};

void Terminal_Placing(vector<Inst*> D0inst, vector<list<Inst*>> nets, vector<vector<LibCell> > Lib, Die* die0)
{
    int terminalcount = 0;
    vector<bool> needterminal(nets.size());
    for(int i=0;i<nets.size();i++)
    {
        needterminal[i] = false;
        int sameornot = nets[i].front()->atdie;
        list<Inst*>::iterator itr = nets[i].begin();
        while(itr != nets[i].end())
        {
            if(sameornot != (*itr)->atdie)
            {
                needterminal[i] = true;
                terminalcount ++;
                break;
            }
            itr++;
        }
    }
    //showterminalneed(needterminal);


    vector<SquareofNet> SQN(nets.size()); 
    for(int i=0;i<D0inst.size();i++)
    {

        for(int j=0;j<D0inst[i]->pinNumused;j++)
        {
            if(needterminal[D0inst[i]->pins[j].net])
            {
                int pinrealposx = D0inst[i]->posX + (*Lib[0][D0inst[i]->type].pins)[D0inst[i]->pins[j].name].posX;
                int pinrealposy = D0inst[i]->posY + (*Lib[0][D0inst[i]->type].pins)[D0inst[i]->pins[j].name].posY;
                if(SQN[D0inst[i]->pins[j].net].llx > pinrealposx)
                SQN[D0inst[i]->pins[j].net].llx = pinrealposx;
                if(SQN[D0inst[i]->pins[j].net].lly > pinrealposy)
                SQN[D0inst[i]->pins[j].net].lly = pinrealposy;
                if(SQN[D0inst[i]->pins[j].net].hrx < pinrealposx)
                SQN[D0inst[i]->pins[j].net].hrx = pinrealposx;
                if(SQN[D0inst[i]->pins[j].net].hry < pinrealposy)
                SQN[D0inst[i]->pins[j].net].hry = pinrealposy;
            }
        }
    }

    for(int i=0;i<nets.size();i++)
    {
        if(needterminal[i])
        {   
            cout<<"square of net "<<i+1<<":"<<endl;
            cout<<"llx: "<<SQN[i].llx<<"  lly: "<<SQN[i].lly<<endl;
            cout<<"hrx: "<<SQN[i].hrx<<"  hry: "<<SQN[i].hry<<endl;
        }
        else
        cout<<"net "<<i+1<<" do not need a terminal"<<endl;
    }

    vector<Terminal> terminals(nets.size());    /////too  large for large case

    int diemidx = (die0->lowerLeftX + die0->higherRightX)/2;
    int diemidy = (die0->lowerLeftY + die0->higherRightY)/2;
    int gridside = sqrt(terminalcount)+1;
    cout<<"gridside="<<gridside<<endl;
    int minposx = diemidx - gridside*Terminal::eqwidth()/2 + Terminal::spacing;
    cout<<"minposx="<<minposx<<endl;
    int minspotx;    
    int minposy = diemidy - gridside*Terminal::eqheight()/2 + Terminal::spacing;
    cout<<"minposy="<<minposy<<endl;
    int minspoty;
    int maxposx = diemidx + gridside*Terminal::eqwidth()/2 - Terminal::spacing/2 - Terminal::width;
    cout<<"maxposx="<<maxposx<<endl;
    int maxspotx;
    int maxposy = diemidy + gridside*Terminal::eqheight()/2 - Terminal::spacing/2 - Terminal::height;
    cout<<"maxposy="<<maxposy<<endl;
    int maxspoty;
    
    int degree = 1;
    for(int i=0;i<terminalcount;i++)
    {
        if(i == degree*degree - degree)
        {
            minspotx = minposx + (degree - 1)*Terminal::eqwidth();
            minspoty = minposy;
        }
        else if(i == degree*degree - degree + 1)
        {
            maxspotx = maxposx - (degree - 1)*Terminal::eqwidth();
            maxspoty = maxposy;
            degree++;
        }
        else if(i%2==0)
        {
            minspotx -= Terminal::eqwidth();
            minspoty += Terminal::eqheight();
        }
        else
        {
            maxspotx += Terminal::eqwidth();
            maxspoty -= Terminal::eqheight();
        }

        int selected;
        if(i%2 == 0)
        {
            int min = sentinel;
            for(int j=0;j<nets.size();j++)
            {
                if(needterminal[j])
                {
                    if(SQN[j].getmidx() + SQN[j].getmidy() < min && terminals[j].posX == -1)
                    {
                        min = SQN[j].getmidx() + SQN[j].getmidy();
                        selected = j;
                    }
                }
            }
            terminals[selected].posX = minspotx;
            terminals[selected].posY = minspoty;
        }
       else
       {
            int max = -1*sentinel;
            for(int j=0;j<nets.size();j++)
            {
                if(needterminal[j])
                {
                    if(SQN[j].getmidx() + SQN[j].getmidy() > max && terminals[j].posX == -1)
                    max = SQN[j].getmidx() + SQN[j].getmidy();
                    selected = j;
                }
            }
            terminals[selected].posX = maxspotx;
            terminals[selected].posY = maxspoty;
        }
    }
    //terminal initialized

    cout<<terminalcount<<endl;
    for(int i=0;i<nets.size();i++)
    {
        if(needterminal[i])
        {
            cout<<"terminal for net "<<i+1<<" is placed at ("<<terminals[i].posX<<","<<terminals[i].posY<<")"<<endl;
        }
    }

    int llxlimit = die0->lowerLeftX + ceil((double)(spacing/2));
    int llylimit = die0->lowerLeftY + ceil((double)(spacing/2));
    int hrxlimit = die0->higherRightX - ceil((double)(spacing/2));
    int hrylimit = die0->higherRightY - ceil((double)(spacing/2));

    vector<int> end(nets.size());

    for(int i=0;i<nets.size();i++)
    {
        end[i] = 1;
        if(needterminal[i])
        {
            end[i] = 0;
        }
    }
    while(true)
    {
        int signal=1;
        for(int i=0;i<nets.size();i++)
        {
            signal *= end[i];
        }
        if(signal == 1)
        break;

        for(int i=0;i<nets.size();i++)
        {
            if(needterminal[i])
            {
                if((terminals[i].posX + Terminal::width/2 - diemidx)*(SQN[i].getmidx - terminals[i].posX) > 0)
                ;
            }
        }

    }
    
    /* 報完再想
    int min = sentinel;
    for(int j=0;j<nets.size();j++)
    {
        if(needterminal[j])
        {
            if(SQN[j].getmidx + SQN[j].getmidy < min)
            {
                min = SQN[j].getmidx + SQN[j].getmidy;
                selected = j;
            }
        }
    }
    terminals[selected].posX = minspotx;
    terminals[selected].posY = minspoty;
    minspotx += Terminal::eqwidth();
    minspoty += Terminal::eqheight();


    
    int max = -1*sentinel;
    for(int j=0;j<nets.size();j++)
    {
        if(needterminal[j])
        {
            if(SQN[j].getmidx + SQN[j].getmidy > min)
            max = SQN[j].getmidx + SQN[j].getmidy;
            selected = j;
        }
    }
    terminals[selected].posX = maxspotx;
    terminals[selected].posY = maxspoty;
    minspotx += Terminal::eqwidth();
    minspoty += Terminal::eqheight();

    for(int i=0;i<terminalcount-2;i++)
    {
        int selected;
        if(i%2 == 0)
        {
            int min = sentinel;
            for(int j=0;j<nets.size();j++)
            {
                if(needterminal[j])
                {
                    if(SQN[j].getmidx + SQN[j].getmidy < min)
                    {
                        min = SQN[j].getmidx + SQN[j].getmidy;
                        selected = j;
                    }
                }
            }
            terminals[selected].posX = minspotx;
            terminals[selected].posY = minspoty;
            while(terminals[selected].posX > minposx + spacing && terminal[selected].posY > minposy +spacing )
            {
                if((SQN[selected].getmidx-minspotx))
            }
            minspotx += Terminal::eqwidth();
            minspoty += Terminal::eqheight();
        }
       else
       {
            int max = -1*sentinel;
            for(int j=0;j<nets.size();j++)
            {
                if(needterminal[j])
                {
                    if(SQN[j].getmidx + SQN[j].getmidy > min)
                    max = SQN[j].getmidx + SQN[j].getmidy;
                    selected = j;
                }
            }
            terminals[selected].posX = maxspotx;
            terminals[selected].posY = maxspoty;
            minspotx += Terminal::eqwidth();
            minspoty += Terminal::eqheight();
        }
    }
    */



}
*/

int main(int argc, char* argv[]) {
    // inputting
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
    vector<list<Inst*> > nets;
    int64_t avgWidth[2] = {0};
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
            Lib[i][j].settype(j);
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
    }
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
        instances[i].name = i;
        instances[i].type = type - 1;
        instances[i].pinNum = Lib[0][type - 1].getpinNum();
        instances[i].pins.resize(instances[i].pinNum);
        instances[i].name = i;
        avgWidth[0] += Lib[0][type - 1].getsizeX();
        avgWidth[1] += Lib[1][type - 1].getsizeX();
    }
    avgWidth[0] /= instanceNum;
    avgWidth[1] /= instanceNum;
    dies[0]->gridWidth = avgWidth[0] + 1;
    dies[1]->gridWidth = avgWidth[1] + 1;
    // Nets Input
    fin >> JUNK >> netNum;
    netPinNum.resize(netNum);
    nets.resize(netNum);
    for (int i = 0; i < netNum; i++) {
        fin >> JUNK >> JUNK >> netPinNum[i];
        for (int j = 0; j < netPinNum[i]; j++) {
            int targetInst, targetPin;
            fin >> JUNK >> junk >> targetInst >> junk >> junk >> targetPin;
            instances[targetInst - 1].pins[instances[targetInst - 1].pinNumused].net = i; /////
            instances[targetInst - 1].pins[instances[targetInst - 1].pinNumused].name = targetPin - 1;
            instances[targetInst - 1].pinNumused++;
        	nets[i].push_back(&instances[targetInst-1]);
        }
    }

    for (int i = 0; i < instanceNum; i++) {
    	instances[i].pins.resize(instances[i].pinNumused);
	}

    vector<Inst*> D0inst,D1inst;
    Partition(&instances, Lib, dies[0], dies[1], nets, &D0inst, &D1inst);
    
    //for(int i=0;i<instances.size();i++)
	//cout<<"inst "<<i+1<<" is on d"<<instances[i].atdie<<endl;
    
    //showtwodie(D0inst,D1inst);
    // end of input

    for (int i = 0; i < 2; i++) {
        dies[i]->colNum = dies[i]->higherRightX / dies[i]->gridWidth;
        dies[i]->grid.resize(dies[i]->colNum);
        for (int j = 0; j < dies[i]->colNum; j++) {
            dies[i]->grid[j].resize(dies[i]->rowNum);
            fill(dies[i]->grid[j].begin(), dies[i]->grid[j].end(), nullptr);
        }
        dies[i]->gridStartX = (dies[i]->higherRightX - dies[i]->gridWidth * dies[i]->colNum) / 2;
    }
    // remember to set die.instnum after gets the partition!!

    //D0 placemaent done
    //test data announcement
    
    //Terminal_Placing(D0inst, nets, Lib, dies[0]);
}