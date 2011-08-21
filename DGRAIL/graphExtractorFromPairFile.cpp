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

void process(Graph &g, istream& in, ostream& os,ostream& upd){
	vector<pair<int,int> > nodes;
	vector<char> info;
	int cnt=0;
	int i=0,x,y,iR,tR;
	char c;
	string line;
	vector<int> list1;
	vector<int> list2;
	int src = -1;	
	while(++cnt<size && getline(in,line)){
		if(line.find('#')!=string::npos)
			continue;
//		cout << line << endl;
		istringstream is(line);	
		is >> skipws >> x >> y;
		if(x==y) continue;
		if(g.vertices.find(x)==g.vertices.end())
			g.insertNode(x,list1,list2);
		if(g.vertices.find(y)==g.vertices.end())
			g.insertNode(y,list1,list2);
		g.insertEdge(x,y);
//		g.writeGraph(cout);
//		index.writeIndex();
		src = x;
	}

	// complete the last node
	while(getline(in,line)){
		if(line.find('#')!=string::npos)
			continue;
//		cout << line << endl;
		istringstream is(line);	
		is >> skipws >> x >> y;
		if(src!=x)
			break;
		if(x==y) continue;
		if(g.vertices.find(x)==g.vertices.end())
			g.insertNode(x,list1,list2);
		if(g.vertices.find(y)==g.vertices.end())
			g.insertNode(y,list1,list2);
		g.insertEdge(x,y);
	}
	
	g.writeGra(os);

	vector<int> empty;
	do{
		if(line.find('#')!=string::npos)
			continue;
//		cout << line << endl;
		istringstream is(line);	
		is >> skipws >> x >> y;
		if(x==y) continue;
		if(src!=x){
			if(list1.size()!=0 || list2.size()!=0){
				upd << "a " << src << " 0 " << list1.size() ;
				for(int i =0; i<list1.size();i++)
					upd << " " << list1[i];
				upd << endl;
				g.insertNode(src, empty, list1);
				list1.clear();
				list1.push_back(src);
				for(int i =0; i<list2.size();i++){
					upd << "a " << list2[i] << " 1 " << src << " 0" << endl;
					g.insertNode(list2[i],list1,empty);
				}
			}
			src = x;
			list1.clear();
			list2.clear();	
		}
		if(g.vertices.find(y) != g.vertices.end()){
			list1.push_back(y);
		}else{
			list2.push_back(y);
		}
	}while(getline(in,line));

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

//	if(!testFileArg){
//		cout << " Incomplete args " << endl;
//		printUsage();
//	}
	
	ifstream ib(argv[graphFileArg],ios::in);
	Graph g;
	
	ofstream ob(strcat(argv[graphFileArg],".gra"),ios::out);
	ofstream ob2(strcat(argv[graphFileArg],".up"),ios::out);
//	ifstream ob(argv[testFileArg],ios::in);
	process(g,ib,ob, ob2);
	ob.close();
	ob2.close();
//	cout << "Exiting!..." << endl;
	return 0;
}
