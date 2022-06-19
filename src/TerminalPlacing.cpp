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

bool Terminalplacement::overlap(int index, int newx, int newy ,vector<Terminal>* terminals, vector<bool>* needterminal,Die* die0)
{
    bool overlap = false;
    for(int i=0;i<index;i++)
    {
        if((*needterminal)[i])
        if(abs(newx - (*terminals)[i].posX) < Terminal::eqwidth() && abs(newy - (*terminals)[i].posY) < Terminal::eqwidth())
        overlap = true;
    }
    for(int i=index+1;i<terminals->size();i++)
    {
        if((*needterminal)[i])
        if(abs(newx - (*terminals)[i].posX) < Terminal::eqwidth() && abs(newy - (*terminals)[i].posY) < Terminal::eqheight())
        overlap = true;
    }
    if(abs(newx - die0->lowerLeftX) < Terminal::spacing)
    overlap = true;
    if(abs(newx - die0->higherRightX) < Terminal::spacing)
    overlap = true;
    if(abs(newy - die0->lowerLeftY) < Terminal::spacing)
    overlap = true;
    if(abs(newy - die0->higherRightY) < Terminal::spacing)
    overlap = true;

    return overlap;
}

bool Terminalplacement::occupy(bool* occupied, int num)
{
    bool a = true;
    for(int i=0;i<num+1;i++)
    if(!occupied[i])
    a = false;

    return a;
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
    //terminal needed checked
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

    //square of net computed
    
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
    cout<<"gridside="<<gridside<<endl;
    int minposx = diemidx - ceil((double)0.5*gridside*Terminal::eqwidth()) + Terminal::spacing/2;
    cout<<"minposx="<<minposx<<endl;
    //int minspotx;    
    int minposy = diemidy - ceil((double)0.5*gridside*Terminal::eqheight()) + Terminal::spacing/2;
    cout<<"minposy="<<minposy<<endl;
    //int minspoty;
    int maxposx = diemidx + ceil((double)0.5*gridside*Terminal::eqwidth()) - Terminal::spacing/2 - Terminal::width;
    cout<<"maxposx="<<maxposx<<endl;
    //int maxspotx;
    int maxposy = diemidy + ceil((double)0.5*gridside*Terminal::eqheight()) - Terminal::spacing/2 - Terminal::height;
    cout<<"maxposy="<<maxposy<<endl;
    //int maxspoty;
    
    
    bool** occupied;
    occupied = new bool* [2*gridside - 1];
    for(int i=0;i<gridside;i++)
    {
        occupied[i] = new bool [i+1];
        for(int j=0;j<i+1;j++)
        occupied[i][j] = false;
    }
    for(int i=gridside;i<2*gridside-1;i++)
    {
        occupied[i] = new bool [2*gridside - i - 1];
        for(int j=0;j<2*gridside - i - 1;j++)
        occupied[i][j] = false;
    }

    cout<<"terminals count = "<<terminalcount<<endl;
    
    int degreemin = 0;
    int degreemax = 0;
    
    
    for(int i=0;i<gridside*gridside - gridside;i++)
    {
        if(i%2 == 0)
        {
            if(i == degreemin*degreemin + 3*degreemin + 2)
            degreemin++;
        }
        else
        {
            if(i == degreemax*degreemax + 3*degreemax + 3)
            degreemax++;
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
 
            int gridselect;
            for(int j=0;j<degreemin+1;j++)
            {
                if(!occupied[degreemin][j])
                {
                    gridselect = j;
                    break;
                }
            }
            int minspotx = minposx + degreemin*Terminal::eqwidth() - gridselect*Terminal::eqwidth();
            int minspoty = minposy + gridselect*Terminal::eqheight();
            for(int j=gridselect+1;j<degreemin+1;j++)
            {
                if(!occupied[degreemin][j])
                {
                    if(abs(SQN[selected].getmidx() - (minspotx - (j-gridselect)*Terminal::eqwidth())) + abs(SQN[selected].getmidy() - (minspoty + (j-gridselect)*Terminal::eqheight() )) <
                    abs(SQN[selected].getmidx() - (minspotx)) + abs(SQN[selected].getmidy() - (minspoty)))
                    {
                        minspotx -= (j-gridselect)*Terminal::eqwidth();
                        minspoty += (j-gridselect)*Terminal::eqheight();
                        gridselect = j;
                    }
                }
            }
            occupied[degreemin][gridselect] = true;
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
            
            int gridselect;
            for(int j=0;j<degreemax+1;j++)
            {
                if(!occupied[2*gridside - 2 - degreemax][j])
                {
                    gridselect = j;
                    break;
                }
            }
            int maxspotx = maxposx - degreemax*Terminal::eqwidth() + gridselect*Terminal::eqwidth();
            int maxspoty = maxposy - gridselect*Terminal::eqheight();
            for(int j=gridselect+1;j<degreemax+1;j++)
            {
                if(!occupied[2*gridside - 2 - degreemax][j])
                {
                    if(abs(SQN[selected].getmidx() - (maxspotx + (j-gridselect)*Terminal::eqwidth())) + abs(SQN[selected].getmidy() - (maxspoty - (j-gridselect)*Terminal::eqheight() )) <
                    abs(SQN[selected].getmidx() - (maxspotx)) + abs(SQN[selected].getmidy() - (maxspoty)))
                    {
                        maxspotx += (j-gridselect)*Terminal::eqwidth();
                        maxspoty -= (j-gridselect)*Terminal::eqheight();
                        gridselect = j;
                    }
                }
            }
            occupied[2*gridside - 2 - degreemax][gridselect] = true;
            (*terminals)[selected].posX = maxspotx;
            (*terminals)[selected].posY = maxspoty;
            
        }
    }

    degreemin++;
    for(int i=gridside*gridside - gridside;i<terminalcount;i++)
    {
        int selected;
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
 
        int gridselect;
        for(int j=0;j<degreemin+1;j++)
        {
            if(!occupied[degreemin][j])
            {
                    gridselect = j;
                    break;
            }
        }
        int minspotx = minposx + degreemin*Terminal::eqwidth() - gridselect*Terminal::eqwidth();
        int minspoty = minposy + gridselect*Terminal::eqheight();
        for(int j=gridselect+1;j<degreemin+1;j++)
        {
            if(!occupied[degreemin][j])
            {
                if(abs(SQN[selected].getmidx() - (minspotx - (j-gridselect)*Terminal::eqwidth())) + abs(SQN[selected].getmidy() - (minspoty + (j-gridselect)*Terminal::eqheight() )) <
                abs(SQN[selected].getmidx() - (minspotx)) + abs(SQN[selected].getmidy() - (minspoty)))
                {
                    minspotx -= (j-gridselect)*Terminal::eqwidth();
                    minspoty += (j-gridselect)*Terminal::eqheight();
                    gridselect = j;
                }
            }
        }
        occupied[degreemin][gridselect] = true;
        (*terminals)[selected].posX = minspotx;
        (*terminals)[selected].posY = minspoty;
    }

    //////////////terminal initialized

    

    for(int i=0;i<(*nets).size();i++)
    {
        if((*needterminal)[i])
        if(overlap(i, (*terminals)[i].posX, (*terminals)[i].posY, terminals, needterminal,die0))
        cout<<"error"<<" for "<<i+1<<endl;
    }
    
    
    
    //terminal initialized
    
    for(int i=0;i<(*nets).size();i++)
    {
        if((*needterminal)[i])
        {
            cout<<"terminal for net "<<i+1<<" is placed at ("<<(*terminals)[i].posX<<","<<(*terminals)[i].posY<<")"<<endl;
        }
    }
    
    

    int llxlimit = die0->lowerLeftX + Terminal::spacing;
    int llylimit = die0->lowerLeftY + Terminal::spacing;
    int hrxlimit = die0->higherRightX - Terminal::spacing;
    int hrylimit = die0->higherRightY - Terminal::spacing;

    vector<int> end((*nets).size());
    vector<double> endx((*nets).size()), endy((*nets).size());
    
    for(int i=0;i<(*nets).size();i++)
    {
        if((*needterminal)[i])
        end[i] = endx[i] = endy[i] = 0;
        else
        end[i] = endx[i] = endy[i] = 1;
    }
    
    /*
    
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
                if(endx[i] < 1 && ((*terminals)[i].posX + Terminal::width/2 - diemidx) * (SQN[i].getmidx() - ((*terminals)[i].posX + Terminal::width/2)) > 0)
                {
                    cout<<"terminal "<<i+1<<" x-moveable."<<endl;
                    int newx = (*terminals)[i].posX + ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    if((*terminals)[i].left != NULL && (*terminals)[i].right != NULL)
                    if(newx > llxlimit && newx + Terminal::width < hrxlimit && (newx > (*terminals)[i].left->posX + Terminal::eqwidth()) && (newx < (*terminals)[i].right->posX - Terminal::eqwidth()))
                    (*terminals)[i].posX += ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    else
                    endx[i] += 0.01;
                    else if((*terminals)[i].left == NULL && (*terminals)[i].right != NULL)
                    if(newx > llxlimit && newx + Terminal::width < hrxlimit && (newx < (*terminals)[i].right->posX - Terminal::eqwidth()))
                    (*terminals)[i].posX += ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    else
                    endx[i] += 0.1;
                    else if((*terminals)[i].left != NULL && (*terminals)[i].right == NULL)
                    if(newx > llxlimit && newx + Terminal::width < hrxlimit && (newx > (*terminals)[i].left->posX + Terminal::eqwidth()))
                    (*terminals)[i].posX += ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    else
                    endx[i] += 0.1;
                    else
                    if(newx > llxlimit && newx + Terminal::width < hrxlimit)
                    (*terminals)[i].posX += ((*terminals)[i].posX + Terminal::width/2 - diemidx)/abs((*terminals)[i].posX + Terminal::width/2 - diemidx);
                    else
                    endx[i] += 1;
                }
                else
                endx[i] += 1;


                if(endy[i] < 1 && ((*terminals)[i].posY + Terminal::height/2 - diemidy) * (SQN[i].getmidy() - ((*terminals)[i].posY + Terminal::height/2)) > 0)
                {
                    cout<<"terminal "<<i+1<<" x-moveable."<<endl;
                    int newy = (*terminals)[i].posY + ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    if((*terminals)[i].down != NULL && (*terminals)[i].up != NULL)
                    if(newy > llylimit && newy + Terminal::height < hrylimit && (newy > (*terminals)[i].down->posY + Terminal::eqheight()) && (newy < (*terminals)[i].up->posY - Terminal::eqheight()))
                    (*terminals)[i].posY += ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    else
                    endy[i] += 0.01;
                    else if((*terminals)[i].down == NULL && (*terminals)[i].up != NULL)
                    if(newy > llylimit && newy + Terminal::height < hrylimit && (newy < (*terminals)[i].up->posY - Terminal::eqheight()))
                    (*terminals)[i].posY += ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    else
                    endy[i] += 0.1;
                    else if((*terminals)[i].down != NULL && (*terminals)[i].up == NULL)
                    if(newy > llylimit && newy + Terminal::height < hrylimit && (newy > (*terminals)[i].down->posY + Terminal::eqheight()))
                    (*terminals)[i].posY += ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    else
                    endy[i] += 0.1;
                    else
                    if(newy > llylimit && newy + Terminal::height < hrylimit)
                    (*terminals)[i].posY += ((*terminals)[i].posY + Terminal::height/2 - diemidy)/abs((*terminals)[i].posY + Terminal::height/2 - diemidy);
                    else
                    endy[i] += 1;
                }
                else
                endy[i] += 1;

                if(endx[i] >= 1 && endy[i] >= 1)
                end[i] = 1;
            }
        }
    }
    */


   delete occupied;
}

