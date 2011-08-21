/* DAGGER - A Scalable Reachability Index For Dynamic Graphs
Copyright (C) 2011 Hilmi Yildirim.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/ */
//============================================================================
// Name        : testGenerator.cpp
// Author      : Hilmi Yildirim
// Version     :
// Copyright   : 
// Description : This main class is used to generate test file given a graph and update/test ratio.
//============================================================================

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"

using namespace std;

int IERATIO = 60;
int DERATIO = 75;
int DNRATIO = 80;
int MAXNODEID = 0;
int edgeC = 0;
int prs[100000];
int STEP=1000;
int QpU = 0;
int AVGD = 4;
int PIN = 0;

void printUsage(){
	cout << " TODO : Print Usage " << endl;
	cout << "./testGenerator -g <graphFile> -qpu <queryPerUpdate> [-imode insertmode] [-n <#updates>]" << endl;
	cout << "insertmode can be 0 for Erdos-Renyi model, 1 for Preferential Attachment model " << endl;
//	cout << "deletemode can be 0 for Erdos-Renyi model, 1 for Preferential Attachment model " << endl;
	cout << "default modes are 1"<<endl;
}

int findTargetOld(Graph& g,int rnd){
	int size = g.size();
	int i=0,cmp =0;
	while((cmp+=g[i++].inEdges().size()) < rnd && i<size){
	};
	return i-1;
}

int nextNode(Graph&g, int x){
	for(int i=x+1; i < MAXNODEID;i++)
		if(g.vertices.find(i)!=g.vertices.end())
			return i;
	return MAXNODEID;
}

int findTarget(Graph& g,int trg){
	int size = g.size();
	int i=0,cmp =0;
	while(cmp + prs[i] < trg){
		cmp+=prs[i++];
	}
	i*=STEP;
	if(g.vertices.find(i)==g.vertices.end())
		i = nextNode(g,i);
	while((cmp+=g[i].degree()) < trg && i<MAXNODEID)
		i = nextNode(g,i);
//	cout << trg << " " << i-1 << endl;
	return i;
}

void increase(int val, int from){
	int start = from/STEP;
//	cout << "from="<< from << " step=  " << STEP << " from/step = " << start << endl;
	prs[start]+=val;
//	if(prs[start]<0){
//		cout << "A " << prs[start] << " " << start << " " << val << " " << from << endl;
//	}
		
}


int getRandomNode(Graph&g){
	int t;
	while(1){
		t = rand() % MAXNODEID;	
		if(g.vertices.find(t) != g.vertices.end())
			return t;	
	}
}

int main(int argc, char **argv) {
	int graphFileArg=0;
	int testFileArg=0;
	double ratio= 0.9;
	int N = 100000;
	int insertMode=1,deleteMode=0;
	
	// Parse Args:
	for(int i=0;i<argc; i++){
		if(strcmp(argv[i],"-g")==0){
			graphFileArg = ++i;
		}else if(strcmp(argv[i],"-o")==0){
			testFileArg = ++i;
		}else if(strcmp(argv[i],"-r")==0){
			ratio = atof(argv[++i]);
		}else if(strcmp(argv[i],"-n")==0){
			N = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-imode")==0){
			insertMode = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-dmode")==0){
			deleteMode = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-qpu")==0){
			QpU = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-avgd")==0){
			AVGD = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-pie")==0){
			IERATIO = 100; DERATIO = 100; DNRATIO = 100;
		}
		else if(strcmp(argv[i],"-pdn")==0){
			IERATIO = 0; DERATIO = 0; DNRATIO = 100;
		}
		else if(strcmp(argv[i],"-pde")==0){
			IERATIO = 0; DERATIO = 100; DNRATIO = 0;
		}
		else if(strcmp(argv[i],"-pin")==0){
			IERATIO = 0; DERATIO = 0; DNRATIO = 0;
			PIN = 1;
		}
	}

	if(!graphFileArg){
		cout << " Incomplete args " << endl;
		printUsage();
	}
	
	ifstream ib(argv[graphFileArg],ios::in);
	Graph g(ib);
	VertexList::iterator vit;
	for(vit=g.vertices.begin();vit!=g.vertices.end();vit++){
		increase((vit->second).degree(),vit->first);
		edgeC+=  (vit->second).degree();
		if(vit->first > MAXNODEID)
			MAXNODEID = vit->first;
	}
	MAXNODEID++;
//	g.writeGraph(cout);

	int roll; 
	int f,s;

	for(int i=0;i< N ; i++){
		roll = rand() % 100;
		for(int j=0;j<QpU; j++){
			cout << "q " << getRandomNode(g) << " " << getRandomNode(g) << endl;	
		}
		if(roll < IERATIO){
//			cout << "is gonna add edge " << endl;
			do{
				f = getRandomNode(g);
				switch(insertMode){
					case 0 : s = getRandomNode(g); break;
					case 1 : s = findTarget(g,rand()%edgeC);
				}
			
			}while(g[f].hasEdge(s) || f==s);
			increase(1,f);
			increase(1,s);
			edgeC+=2;
			g.insertEdge(f,s);
			cout << "i " << f << " " << s << endl;
		}else if(roll < DERATIO){
//			cout << "is gonna delete edge " << endl;
			do{
				f = findTarget(g,rand()%edgeC);
			}while((s=g[f].outDegree())==0);
			EdgeList::iterator eit = g[f].outEdges().begin();
			s = rand()%s;
			for(int j=0;j<s;j++,eit++);
			s = eit->first;
			increase(-1,f);
			increase(-1,s);

			g.deleteEdge(f,s);
			edgeC-=2;
			cout << "d " << f << " " << s << endl;
		}
		else if(roll < DNRATIO){
//			cout << "is gonna delete node " << endl;
			// select 3 nodes and remove the node with the in degree
			int x = getRandomNode(g);
			int y = getRandomNode(g);
			int z = getRandomNode(g);
			if ( g[x].degree() < g[y].degree() && g[x].degree() < g[z].degree()){
				cout << "r " << x << endl;
				edgeC-=2*g[x].degree();
				g.deleteNode(x);
			}
			else if ( g[y].degree() < g[z].degree()){
				cout << "r " << y << endl;
				edgeC-=2*g[y].degree();
				g.deleteNode(y);
			}
			else{
				cout << "r " << z << endl;
				edgeC-=2*g[z].degree();
				g.deleteNode(z);
			}
		}else{
//			cout << "is gonna add node " << endl;
			set<int> inl,oul;
			int Degree = rand()%(AVGD/2);
			f = MAXNODEID++;
			for(int j=0;j<Degree;j++){
				switch(insertMode){
					case 0 : s = getRandomNode(g); break;
					case 1 : s = findTarget(g,rand()%edgeC);
				}
				if(rand()%2 && !PIN)
					inl.insert(s);
				else
					oul.insert(s);
			}
			cout << "a " << f << " " << inl.size();
			set<int>::iterator sit = inl.begin();
			while(sit!=inl.end()){
				cout << " " << *sit++;
			}
			cout << " " << oul.size();
			sit = oul.begin();
			while(sit!=oul.end()){
				cout << " " << *sit++;
			}
			cout << endl;
			vector<int> inv(inl.begin(),inl.end());
			vector<int> ouv(oul.begin(),oul.end());
			g.insertNode(f,inv,ouv);
		}
//			cout << " Balance " << balance << endl;
	}
	return 0;
}
