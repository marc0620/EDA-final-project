#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;
string buf;
char bufc;
int bufi;
class TECH;




struct PIN
{
	int index;
	int x;
	int y;	
};

class CELL
{
	public:
	int index;
	int numpin;
	int width;
	int height;
	int area(){return width*height;}
	vector <PIN> P;
	void inputpin(int);
	
	CELL() { };
};

void CELL::inputpin(int ind)
{
	cin>>buf>>buf;
	PIN Ptemp;
	Ptemp.index=ind;
	cin>>Ptemp.x>>Ptemp.y;
	P.push_back(Ptemp);
}

class TECH
{
	public:
	int numcell;
	vector <CELL> MC;
	void inputcell(int);
	
	TECH() { };
};

void TECH::inputcell(int ind)
{
	cin>>buf>>buf;
	CELL C;
	C.index=ind;
	cin>>C.width>>C.height>>C.numpin;
	for(int j=1;j<=C.numpin;j++)
	C.inputpin(j);
	MC.push_back(C);

}

class TERMINAL
{
	public:
	int width;
	int height;
	int spacing;
	int eqwidth(){return width+spacing ;}
	int eqheight(){return height+spacing ;}
	int eqarea(){return eqwidth()*eqheight() ;}
	
	TERMINAL(){	}
	void inputdata();
};

void TERMINAL::inputdata()
{
	cin>>buf>>width>>height;
	cin>>buf>>spacing;
}

class DIE
{
	public:
	int initx;
	int inity;
	int endx;
	int endy;
	int area() { return (endx-initx)*(endy-inity);}
	
	int arealimit;
	int rowheight;
	int rowrepeat;
	char techtype;
	
	TECH TE;

	
	DIE(){ }
};

void inputdiedata(DIE D1,DIE D2)
{
	cin>>buf>>D1.initx>>D1.inity>>D1.endx>>D1.endy;
	D2.initx=D1.initx;	D2.inity=D1.inity; D2.endx=D1.endx; D2.endy=D1.endy;
	cin>>buf>>D1.arealimit>>buf>>D2.arealimit;
	cin>>buf>>bufi>>bufi>>bufi>>D1.rowheight>>D1.rowrepeat;
	cin>>buf>>bufi>>bufi>>bufi>>D2.rowheight>>D2.rowrepeat;
	cin>>buf>>bufc>>D1.techtype>>buf>>bufc>>D2.techtype;
	
}


int main()
{
	TECH TA,TB;
	cin>>buf>>buf>>TA.numcell;
	for(int i=1;i<=TA.numcell;i++)
	TA.inputcell(i);

	cin>>buf>>buf>>TB.numcell;
	for(int i=1;i<=TB.numcell;i++)
	TB.inputcell(i);
	
	DIE D1,D2;
	inputdiedata(D1,D2);
	if(D1.techtype=='A')
	D1.TE=TA;
	else
	D1.TE=TB;
	
	if(D2.techtype=='A')
	D2.TE=TA;
	else
	D2.TE=TB;
	 

	
	TERMINAL TER;
	TER.inputdata();
	
	int numinst;
	cin>>buf>>numinst;
	
	
	pair<CELL,list<pair<CELL,int> > > Adjlist[numinst+1];
	int D1area=0;
	int D2area=0;
	for(int i=1;i<=numinst;i++)
	{
		int celltype;
		cin>>buf>>buf>>bufc>>bufc>>celltype;
		if(D1area/D1.arealimit<=D2area/D2.arealimit)
		{
			(Adjlist[i]).first=D1.TE.MC[celltype];
			D1area+=((Adjlist[i]).first).area();
		}
		else
		{
			(Adjlist[i]).first=D2.TE.MC[celltype];
			D2area+=((Adjlist[i]).first).area();
		} 
	}

}
