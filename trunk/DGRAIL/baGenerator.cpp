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
// Name        : baGenerator.cpp
// Author      : Hilmi Yildirim
// Version     :
// Copyright   : 
// Description : This main class is used to generate a dag in Albert-Barabasi model.
//============================================================================

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"
#define STEP 1000
using namespace std;
int k= 4;
int N = 100000;
int prs[100000];
int bidir = 0;
int edgeC;
int XXX,YYY;
int gralimit = -1;
void printUsage(){
	cout << " TODO : Print Usage " << endl;
	cout << "./testGenerator -n <numberofnodes> -d <avg. degree> " << endl;
}

int findTarget(Graph& g,int trg){
	int size = g.size();
	int i=0,cmp =0;
	while(cmp + prs[i] < trg){
		cmp+=prs[i++];
	}
	i*=STEP;
	while((cmp+=g[i++].inEdges().size()+k) <= trg && i<size);
//	cout << trg << " " << i-1 << endl;
	return i-1;
}

void increase(int val, int from){
	int start = from/STEP;
	prs[start]+=val;
}

int main(int argc, char **argv) {
	int insertMode=0,deleteMode=0;
	
	// Parse Args:
	for(int i=0;i<argc; i++){
		if(strcmp(argv[i],"-n")==0){
			N = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-d")==0){
			k = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-bidir")==0){
			bidir=1;
		}else if(strcmp(argv[i],"-upfile")==0){
			gralimit = atoi(argv[++i]);
			cout << "this is gralimit " << gralimit <<endl;
		}
	}
	ofstream grafile;
	grafile.open("out.gra");
	ofstream updatefile;
	updatefile.open("out.up");
	Graph g(2*k);
//	g.writeGraph(cout);
	int val,trg;
	edgeC = k*k*4;
	increase(edgeC,2*k);

	vector<int> ti, to;
	if(gralimit!=-1 && gralimit <k){
		g.writeGra(grafile);
		grafile.close();
		cout << "wrote gra at the beggining, everything is in the update file" << endl;
	}
	for(int i=2*k; i < N; i++){
		if(gralimit == i){
			g.writeGra(grafile);
			grafile.close();
			cout << "grafile contains the first " << gralimit << " nodes, the remaining are in the update file" << endl;
		}
		g.insertNode(i,ti,to);
		if(i%1000==0)
			cerr << i << endl;
		int d = rand() % (2*k-1)+1;
		for(int j=0;j<d;j++){
//			g.writeGraph(cout);
//			cout << "EdgeC = " << edgeC << " rand= " << val << endl;
			val = rand()%edgeC;
			trg = findTarget(g,val);
			if(trg==-1 || trg==i){
				j--;
				continue;
			}
			if(bidir == 0 || (bidir==1 && rand()%2==0)){
				if(g[i].hasEdge(trg)){
					j--; continue;
				}
				g.insertEdge(i,trg);
				XXX++;
			}else{
				YYY++;
				g.insertEdge(trg,i);
			}
			edgeC++;
			increase(1,trg);
		}
		edgeC+=k;
		increase(k,i);
		if(gralimit && i>=gralimit){
			EdgeList::iterator eit;
			EdgeList& inl = g[i].inEdges();
			updatefile << "a "<< i << " " << inl.size();
			for(eit = inl.begin();eit!=inl.end();eit++){
				updatefile << " " << eit->first ;
			}
			EdgeList& oul = g[i].outEdges();
			updatefile << " " << oul.size();
			for(eit = oul.begin();eit!=oul.end();eit++){
				updatefile << " " << eit->first ;
			}
			updatefile << endl;
		}
	}
	updatefile.close();
	if(gralimit==-1){
		g.writeGra(grafile);
		grafile.close();
		cout << "wrote everything into grafile, there is no update file " << endl;
	}
	cout << XXX << " " << YYY << endl;	
	return 0;
}
