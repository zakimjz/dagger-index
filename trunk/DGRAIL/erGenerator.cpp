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
	Graph g(N);
//	g.writeGraph(cout);
	int val,trg;

	for(int i=0; i < N; i++){
		if(i%1000==0)
			cerr << i << endl;
		int d = rand() % (2*k);
		for(int j=0;j<d;j++){
			trg = rand()%N;
			g.insertEdge(i,trg);
			edgeC++;
		}
	}
	if(gralimit==-1){
		g.writeGra(grafile);
		grafile.close();
		cout << "wrote everything into grafile, there is no update file " << endl;
	}
	cout << XXX << " " << YYY << endl;	
	return 0;
}
