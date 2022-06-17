#ifndef TP
#define TP
#include "../lib/TerminalPlacing.h"
#endif

void Terminalplacement::showterminalneed(vector<bool> needterminal)
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

void Terminalplacement::Terminal_Placing(vector<Terminal>* terminals, vector<bool>* needterminal, vector<Inst*> D0inst, vector<list<Inst*>> *nets, vector<vector<LibCell> > *Lib, Die* die0)
{
    int terminalcount = 0;
    //vector<bool> needterminal((*nets).size());
    for(int i=0;i<(*nets).size();i++)
    {
        (*needterminal)[i] = false;
        int sameornot = (*nets)[i].front()->atdie;
        list<Inst*>::iterator itr = (*nets)[i].begin();
        while(itr != (*nets)[i].end())
        {
            if(sameornot != (*itr)->atdie)
            {
                (*needterminal)[i] = true;
                terminalcount ++;
                break;
            }
            itr++;
        }
    }
    //showterminalneed(needterminal);

    vector<SquareofNet> SQN((*nets).size()); 
    for(int i=0;i<D0inst.size();i++)
    {

        for(int j=0;j<D0inst[i]->pins.size();j++)
        {
            if(D0inst[i]->pins[j].net != -1)
            if((*needterminal)[D0inst[i]->pins[j].net])
            {
                int pinrealposx = D0inst[i]->posX + (*(*Lib)[0][D0inst[i]->type].pins)[j].relativePosX;
                int pinrealposy = D0inst[i]->posY + (*(*Lib)[0][D0inst[i]->type].pins)[j].relativePosY;
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

    for(int i=0;i<(*nets).size();i++)
    {
        if((*needterminal)[i])
        {   
            cout<<"square of net "<<i+1<<":"<<endl;
            cout<<"llx: "<<SQN[i].llx<<"  lly: "<<SQN[i].lly<<endl;
            cout<<"hrx: "<<SQN[i].hrx<<"  hry: "<<SQN[i].hry<<endl;
        }
        else
        cout<<"net "<<i+1<<" do not need a terminal"<<endl;
    }

    //vector<Terminal> terminals((*nets).size());    /////too  large for large case

    int diemidx = (die0->lowerLeftX + die0->higherRightX)/2;
    int diemidy = (die0->lowerLeftY + die0->higherRightY)/2;
    int gridside = sqrt(terminalcount)+1;
    //cout<<"gridside="<<gridside<<endl;
    int minposx = diemidx - ceil((double)gridside*Terminal::eqwidth()/2) + Terminal::spacing/2;
    //cout<<"minposx="<<minposx<<endl;
    int minspotx;    
    int minposy = diemidy - ceil((double)gridside*Terminal::eqheight()/2) + Terminal::spacing/2;
    //cout<<"minposy="<<minposy<<endl;
    int minspoty;
    int maxposx = diemidx + ceil((double)gridside*Terminal::eqwidth()/2) - Terminal::spacing/2 - Terminal::width;
    //cout<<"maxposx="<<maxposx<<endl;
    int maxspotx;
    int maxposy = diemidy + ceil((double)gridside*Terminal::eqheight()/2) - Terminal::spacing/2 - Terminal::height;
    //cout<<"maxposy="<<maxposy<<endl;
    int maxspoty;
    
    int degree = 1;
    //cout<<terminalcount;
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
            for(int j=0;j<(*nets).size();j++)
            {
                if((*needterminal)[j])
                {
                    if(SQN[j].getmidx() + SQN[j].getmidy() < min && (*terminals)[j].posX == -1)
                    {
                        min = SQN[j].getmidx() + SQN[j].getmidy();
                        selected = j;
                    }
                }
            }
            (*terminals)[selected].posX = minspotx;
            (*terminals)[selected].posY = minspoty;
        }
       else
       {
            int max = -1*sentinel;
            for(int j=0;j<(*nets).size();j++)
            {
                if((*needterminal)[j])
                {
                    if(SQN[j].getmidx() + SQN[j].getmidy() > max && (*terminals)[j].posX == -1)
                    {
                        max = SQN[j].getmidx() + SQN[j].getmidy();
                        selected = j;
                    }
                }
            }
            (*terminals)[selected].posX = maxspotx;
            (*terminals)[selected].posY = maxspoty;
        }
    }
    
    //terminal initialized
    
    //for(int i=0;i<(*nets).size();i++)
    //{
    //    if(needterminal[i])
    //    {
    //        cout<<"terminal for net "<<i+1<<" is placed at ("<<terminals[i].posX<<","<<terminals[i].posY<<")"<<endl;
    //    }
    //}

    int llxlimit = die0->lowerLeftX + Terminal::spacing;
    int llylimit = die0->lowerLeftY + Terminal::spacing;
    int hrxlimit = die0->higherRightX - Terminal::spacing;
    int hrylimit = die0->higherRightY - Terminal::spacing;

    vector<int> end((*nets).size()), endx((*nets).size()), endy((*nets).size());
    
    for(int i=0;i<(*nets).size();i++)
    {
        if((*needterminal)[i])
        end[i] = endx[i] = endy[i] = 0;
        else
        end[i] = endx[i] = endy[i] = 1;
    }
    
    
    while(true)
    {
        int signal=1;
        for(int i=0;i<(*nets).size();i++)
        {
            signal *= end[i];
        }
        if(signal == 1)
        break;

        for(int i=0;i<(*nets).size();i++)
        {
            if((*needterminal)[i] && end[i] == 0)
            {
                if(endx[i] == 0 && ((*terminals)[i].posX + Terminal::width/2 - diemidx) * (SQN[i].getmidx() - ((*terminals)[i].posX + Terminal::width/2)) > 0)
                {
                    //cout<<"terminal "<<i+1<<" x-moveable."<<endl;
                    int newx = (*terminals)[i].posX + ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    if(newx > llxlimit && newx + Terminal::width < hrxlimit)
                    (*terminals)[i].posX += ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    else
                    endx[i] = 1;
                }
                else
                endx[i] = 1;
                if((endy[i] == 0 && (*terminals)[i].posY + Terminal::height/2 - diemidy) * (SQN[i].getmidy() - ((*terminals)[i].posY + Terminal::height/2)) > 0)
                {
                    //cout<<"terminal "<<i+1<<" y-moveable."<<endl;
                    int newy = (*terminals)[i].posY + ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    if(newy > llylimit && newy + Terminal::height < hrylimit)
                    (*terminals)[i].posY += ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    else
                    endy[i] = 1;
                }
                else
                endy[i] = 1;

                if(endx[i] == endy[i] == 1)
                end[i] = 1;
            }
        }
    }

    
    
    
    /* 報完再想
    int min = sentinel;
    for(int j=0;j<(*nets).size();j++)
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

