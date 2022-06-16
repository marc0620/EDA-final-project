#include "../lib/Objects.h"

void showcostreduction(vector<Inst> instances)
{
	for(int i=0;i<instances.size();i++)
	cout<<"cost reduction of Inst "<<i+1<<" = "<<instances[i].cr<<endl;
}

int getcurrentcost(vector<Inst> instances)
{
	float totalcost=0;
	for(int i=0;i<instances.size();i++){
		for(list<pair<Inst*, float> >::iterator itr = instances[i].adjlist.begin(); itr != instances[i].adjlist.end(); itr++){
			if(instances[i].atdie != (*itr).first->atdie)
			totalcost += (*itr).second;
		}
	}
	return (int)totalcost/2;
}

void showcurrentcost(vector<Inst> instances)
{
	float totalcost=0;
	for(int i=0;i<instances.size();i++){
		for(list<pair<Inst*, float> >::iterator itr = instances[i].adjlist.begin(); itr != instances[i].adjlist.end(); itr++){
			if(instances[i].atdie != (*itr).first->atdie)
			totalcost += (*itr).second;
		}
	}
	cout<<"current total cost =  "<<(int)totalcost/2<<endl;
}

void showadjlist(vector<Inst> instances)
{
	for(int i=0;i<instances.size();i++){
		cout<<"neighbors of Inst "<<i+1<<":"<<endl;
		for(list<pair<Inst*, float> >::iterator itr = instances[i].adjlist.begin(); itr != instances[i].adjlist.end(); itr++){
			cout<<(*itr).first->name+1<<" "<<setprecision(2)<<(*itr).second<<endl;
		}
	}
}

void showtwodie(vector<Inst*> D0,vector<Inst*> D1){
	cout<<"Instances on D0: "<<endl;
	for(int i=0;i<D0.size();i++)
	cout<<"Inst "<<D0[i]->name+1<<endl;
	
	cout<<"Instances on D1: "<<endl;
	for(int i=0;i<D1.size();i++)
	cout<<"Inst "<<D1[i]->name+1<<endl;
}

pair<vector<Inst*>,vector<Inst*> > Partition(vector<Inst> instances, vector<vector<LibCell> > Lib, Die* die0, Die* die1, vector<list<Inst> > nets){
	//pair<Inst*,list<pair<Inst*,int> > > Adjlist[instances.size()];
	//adjacency list define
	vector<double> nk;
	nk.resize(nets.size());
	for(int i=0;i<nk.size();i++){
		nk[i]=4/(float)nets[i].size()/(float)nets[i].size(); 
	}
	
	const int c1=1,c2=10;	//調參 
	
	for(int i=0;i<instances.size()-1;i++){
		for(int j=i+1;j<instances.size();j++){
			float tefunc = 1;	//調參 
			float costforij = 0;
			for(int k=0;k<instances[i].pins.size();k++){
				for(int l=0;l<instances[j].pins.size();l++){
					if(instances[i].pins[k].net == instances[j].pins[l].net){
						costforij += nk[instances[i].pins[k].net];
					} 
				}
			}
			if(costforij != 0){
				instances[i].adjlist.push_back(make_pair(&instances[j],(c1*tefunc*c2*costforij)));
				instances[j].adjlist.push_back(make_pair(&instances[i],(c1*tefunc*c2*costforij)));
			}
		} 
	} 
	//adjacency list doned
	showadjlist(instances);
	
	//partition start
	int D0area=0;
	int D1area=0;
	vector<Inst*> D0inst,D1inst;
	for(int i=0;i<instances.size();i++){
		if(D0area*100/die0->maxUtil <= D1area*100/die1->maxUtil){
			instances[i].atdie = 0;
			//D0inst.push_back(&instances[i]);
			//(Adjlist[i]).first = &instances[i];
			D0area += Lib[instances[i].atdie][instances[i].type].getarea();
		}
		else{
			instances[i].atdie = 1;
			//D1inst.push_back(&instances[i]);
			//(Adjlist[i]).first = &instances[i];
			D1area += Lib[instances[i].atdie][instances[i].type].getarea();
		} 
	}
	//showcurrentcost(instances);
	//showtwodie(D0inst,D1inst);
	
	//cost reduction calculation
	for(int i=0;i<instances.size();i++){
		for(list<pair<Inst*, float> >::iterator itr = instances[i].adjlist.begin(); itr != instances[i].adjlist.end(); itr++){
			float temp=0;
			if(instances[i].atdie == (*itr).first->atdie)
			temp -= (*itr).second;
			else
			temp += (*itr).second;
			instances[i].cr = (int)temp;
		}
	}
	///////
	//showcostreduction(instances);
	int count = 0;
	int idealterminalnum = (int)((double)die0->area/((double)Terminal::eqarea())*(0.5));	//調參 
	int idealtotalcost = idealterminalnum*10*0.8;	//調參 
	int currentcost = getcurrentcost(instances);
	
	const int CRsize = 100;
	list<Inst*> CRD0[2*CRsize+1],CRD1[2*CRsize+1];
	for(int i=0;i<instances.size();i++)	//store data to apply FM partition
	{
		if(instances[i].atdie == 0)
		CRD0[instances[i].cr + CRsize].push_back(&instances[i]);
		else
		CRD1[instances[i].cr + CRsize].push_back(&instances[i]);
	}
	
	while(count < instances.size())
	{
		cout<<"test4   index= "<<currentcost<<" - "<<idealtotalcost<<endl;
		int outward = -1;
		int index = (currentcost-idealtotalcost) >= 0 ? min(2*CRsize, (currentcost - idealtotalcost) + CRsize) : max(0, (currentcost - idealtotalcost) + CRsize);
		cout<<index<<endl;
		if(D0area*100/die0->maxUtil >= D1area*100/die1->maxUtil)	//move a inst from d0 to d1
		{	
			while(CRD0[index].size() == 0 && index != 2*CRsize && index != 0 && index != CRsize && index != (currentcost - idealtotalcost)*2 + CRsize)	//search the specific cr
			{	//from index,index-1,index+1,index-2,...
				index += outward;
				outward = -1*(outward+abs(outward)/outward);
			}
			if(index != 2*CRsize && index != 0 && index != CRsize)	//search done
			{
				Inst *movedinst = CRD0[index].front();
				for(list<pair<Inst*, float> >::iterator itr = movedinst->adjlist.begin(); itr != movedinst->adjlist.end(); itr++)
				{
					if((*itr).first->atdie == movedinst->atdie)	//the neighbor is at the same side before moving
					{
						list<Inst*>::iterator itr2 = CRD0[(*itr).first->cr + CRsize].begin();
						while(*itr2 != (*itr).first && itr2 != CRD0[(*itr).first->cr + CRsize].end())
						itr2++;
						if(itr2 == CRD0[(*itr).first->cr + CRsize].end())
						continue;
						CRD0[(*itr).first->cr + CRsize].erase(itr2);
						(*itr).first->cr += 2*ceil((*itr).second);
						CRD0[(*itr).first->cr + CRsize].push_front((*itr).first);
					}
					else										//the neighbor is at the other side before moving
					{
						list<Inst*>::iterator itr2 = CRD1[(*itr).first->cr + CRsize].begin();
						while(*itr2 != (*itr).first && itr2 != CRD1[(*itr).first->cr + CRsize].end())
						itr2++;
						if(itr2 == CRD1[(*itr).first->cr + CRsize].end())
						continue;
						CRD1[(*itr).first->cr + CRsize].erase(itr2);
						(*itr).first->cr -= 2*ceil((*itr).second);
						CRD1[(*itr).first->cr + CRsize].push_front((*itr).first);
					}
				}
				D0area -= Lib[0][CRD0[index].front()->type].getarea();
				D1area += Lib[1][CRD0[index].front()->type].getarea();
				//currentcost -= CRD0[index].front()->cr;
				CRD0[index].front()->atdie ^= 1; 
				currentcost = getcurrentcost(instances);
				CRD0[index].pop_front();
			}
			else break;
		}
		else													//move a inst from d1 to d0
		{
			while(CRD1[index].size() == 0 && index != 2*CRsize && index != 0 && index != CRsize && index != (currentcost - idealtotalcost)*2 + CRsize)	//search the specific cr
			{
				index += outward;
				outward = -1*(outward+abs(outward)/outward);
			}
			if(index != 2*CRsize && index != 0 && index != CRsize && index != (currentcost - idealtotalcost)*2 + CRsize)	//search done
			{
				Inst *movedinst = CRD1[index].front();
				for(list<pair<Inst*, float> >::iterator itr = movedinst->adjlist.begin(); itr != movedinst->adjlist.end(); itr++)
				{
					if((*itr).first->atdie == movedinst->atdie)	//the neighbor is at the same side before moving
					{
						list<Inst*>::iterator itr2 = CRD1[(*itr).first->cr + CRsize].begin();
						while(*itr2 != (*itr).first && itr2 != CRD1[(*itr).first->cr + CRsize].end())
						itr2++;
						if(itr2 == CRD1[(*itr).first->cr + CRsize].end())
						continue;
						CRD1[(*itr).first->cr + CRsize].erase(itr2);
						(*itr).first->cr += 2*ceil((*itr).second) ;
						CRD1[(*itr).first->cr + CRsize].push_front((*itr).first);
					}
					else										//the neighbor is at the other side before moving
					{
						list<Inst*>::iterator itr2 = CRD0[(*itr).first->cr + CRsize].begin();
						while(*itr2 != (*itr).first && itr2 != CRD0[(*itr).first->cr + CRsize].end())
						itr2++;
						if(itr2 == CRD0[(*itr).first->cr + CRsize].end())
						continue;
						
						CRD0[(*itr).first->cr + CRsize].erase(itr2);
						(*itr).first->cr -= 2*ceil((*itr).second);//(int)2*(*itr).second ;
						CRD0[(*itr).first->cr + CRsize].push_front((*itr).first);
					}
				}
				D0area += Lib[0][CRD1[index].front()->type].getarea();
				D1area -= Lib[1][CRD1[index].front()->type].getarea();
				//currentcost -= CRD1[index].front()->cr;
				CRD1[index].front()->atdie ^= 1; 
				currentcost = getcurrentcost(instances);
				CRD1[index].pop_front();
			}
			else break;
			
		}
		//for(int i=0;i<instances.size();i++)
		//cout<<"inst "<<i+1<<" is on d"<<instances[i].atdie<<endl;
		//showcurrentcost(instances);
		//showcostreduction(instances);
		count++;
	}
	
	//for(int i=0;i<instances.size();i++)
	//cout<<"inst "<<i+1<<" is on d"<<instances[i].atdie<<endl;
	//showcurrentcost(instances);
	//showcostreduction(instances);
	
	for(int i=0;i<instances.size();i++){
		if(instances[i].atdie == 0)
		D0inst.push_back(&instances[i]);
		else
		D1inst.push_back(&instances[i]);
	}
	//showtwodie(D0inst,D1inst); 
	return make_pair(D0inst,D1inst);
}