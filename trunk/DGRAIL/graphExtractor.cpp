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
// Name        : graphExtractor.cpp
// Author      : Hilmi Yildirim
// Version     :
// Copyright   : 
// Description : Extract an initial graph from test file and print it in gra format.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"
#include "Grail.h"
#include "DFSIndex.h"

using namespace std;

int size = 100000;

void process(Graph &g, istream& in){
	vector<pair<int,int> > nodes;
	vector<char> info;
	int cnt;
	int i=0,x,y,iR,tR;
	char c;
	string line;
	while(getline(in,line) && ++cnt< size){
		istringstream is(line);	
		is >> skipws >> c;
		if(c == '#')
			continue;
		
		switch(c){
			case 'q' : 
					is >> skipws >> x >> y;
				   break; 
			case 'd' : 	is >> skipws >> x >> y;
					g.deleteEdge(x,y); break;
			case 'i' : is >> skipws >> x >> y;
					g.insertEdge(x,y); break;
			case 'r' : is >> skipws >> x ;
					g.deleteNode(x); break;
			case 'a' :
						int noofincoming,noofoutgoing,next;
						is >> skipws >> x >> noofincoming ;
						vector<int> inedges;
						for(int i=0;i<noofincoming; i++){
							is >> skipws >> next;
							inedges.push_back(next);
						}
						is >> skipws >> noofoutgoing ;
						vector<int> outedges;
						for(int i=0;i<noofoutgoing; i++){
							is >> skipws >> next;
							outedges.push_back(next);
						}
						g.insertNode(x,inedges,outedges);	break;
		}	
//		g.writeGraph(cout);
//		index.writeIndex();
	}
//	index.writeIndex();
}

void printUsage(){
	cout << " TODO : Print Usage " << endl;
}

int main(int argc, char **argv) {
	Index * index;
	Index * dfsIndex;
	int graphFileArg=0;
	int testFileArg=0;
	int typeFileArg=0;
	int dim = 2;
	
	// Parse Args:
	for(int i=0;i<argc; i++){
		if(strcmp(argv[i],"-g")==0){
			graphFileArg = ++i;
		}else if(strcmp(argv[i],"-t")==0){
			testFileArg = ++i;
		}else if(strcmp(argv[i],"-type")==0){
			typeFileArg = ++i;
		}else if(strcmp(argv[i],"-n")==0){
			size = atoi(argv[++i]);
		}
	}

	if(!testFileArg){
		cout << " Incomplete args " << endl;
		printUsage();
	}
	
	ifstream ib(argv[graphFileArg],ios::in);
	Graph g;
	
	ifstream ob(argv[testFileArg],ios::in);
	process(g,ob);
	g.writeGra(cout);
	
//	cout << "Exiting!..." << endl;
	return 0;
}
