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
// Name        : correctness.cpp
// Author      : Hilmi Yildirim
// Version     :
// Copyright   : 
// Description : This main class is used to compare the results of a given index with the simple dfs solution.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "Graph.h"
#include "Grail.h"
#include "BfsGrail.h"
#include "DFSIndex.h"
#include "BFSIndex.h"

using namespace std;

int QpU = 0;
int upCnt = 100000000;
void process(Graph &g, Index& index, Index& groundTruth, istream& in){
	vector<pair<int,int> > nodes;
	vector<char> info;
	string buf;	
	int i=0,x,y,iR,tR;
	char c;
	while(--upCnt && getline(in,buf)){

		istringstream is(buf);	
//		cout << line << endl;
		is >> skipws >> c;
		if(c == '#')
			continue;
		if(QpU){
			for(int i=0;i<QpU;i++){
				x = g.randomNodeSelection2();
				y = g.randomNodeSelection2();
				if( g.vertices.find(x) == g.vertices.end() ||  g.vertices.find(y) == g.vertices.end())
					continue;
				if(g[x].outDegree()==0)
					continue;
//				cout << "query " << x << " " << y << endl; 
				iR = index.query(x,y);
				tR = groundTruth.query(x,y);
				if(iR != tR){		
					g.writeGraph(cout);
					index.writeIndex();
					cout << buf << endl;
					cout << "Mismatch at query: " << i << ": ("<< x << "," << y << ") returns " << iR << " truth : " << tR<< endl;
					return;	
				}
			}
		}
	
		switch(c){
			case 'q' : 
					is >> skipws >> x >> y;
					if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - query skipped" << endl;
						break;
					}
					iR = index.query(x,y);
				   tR = groundTruth.query(x,y);
				   if(iR != tR){		
					//g.writeGraph(cout);
					//index.writeIndex();

					cout << "Mismatch at query:" << " ("<< x << "," << y << ") returns " << iR << " truth : " << tR<< endl;
//					g.writeGraph(cout);
//					index.writeIndex();
					return;	
				   }
				   break; 
			case 'd' : 	is >> skipws >> x >> y;
					if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - edge deletion aborted" << endl;
						break;
						//exit(0);
					}
					if(!g[x].hasEdge(y)){
						cout << "the edge (" << x << ","<<y <<") does not exist - edge deletion aborted " << endl;
						break;
					}
						g.deleteEdge(x,y); break;
			case 'i' : is >> skipws >> x >> y;							if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - edge insertion aborted" << endl;
						break;		
				}
				if(g[x].hasEdge(y)){
					cout << "the edge (" << x << ","<<y <<") already exists - edge insertion aborted " << endl;
					break;
				}

						g.insertEdge(x,y); break;
			case 'r' : is >> skipws >> x ;
				if(g.vertices.find(x)== g.vertices.end()){
					cout << x << " does not exist - node deletion aborted" << endl;
						break;
				}
	
						g.deleteNode(x); 
						break;
			case 'a' :
						int noofincoming,noofoutgoing,next;
						is >> skipws >> x >> noofincoming ;
						if(g.vertices.find(x) != g.vertices.end()){
							cout << x << " already exists - node addition aborted" << endl;
							break;
						}
	
						vector<int> inedges;
						for(int i=0;i<noofincoming; i++){
							is >> skipws >> next;
							if(g.vertices.find(next) == g.vertices.end()){
								cout << next << " does not exists - node addition aborted" << endl;
								break;
							}
							inedges.push_back(next);
						}
						is >> skipws >> noofoutgoing ;
						vector<int> outedges;
						for(int i=0;i<noofoutgoing; i++){
							is >> skipws >> next;
							if(g.vertices.find(next) == g.vertices.end()){
								cout << next << " does not exists - node addition aborted" << endl;
								break;
							}
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
		}else if(strcmp(argv[i],"-dim")==0){
			dim = atoi(argv[++i]);
		}else if(strcmp(argv[i],"-qpu")==0){
			QpU = atoi(argv[++i]);
		}

	}

	if(!graphFileArg || !testFileArg || !typeFileArg){
		cout << " Incomplete args " << endl;
		printUsage();
	}
	
	ifstream ib(argv[graphFileArg],ios::in);
	Graph g(ib);
//	g.writeGraph(cout);
	
	dfsIndex = new DFSIndex(g);
	
	if(strcmp(argv[typeFileArg],"mg")==0){
		index = new Grail(g,dim,0,10);
		cout << "finished indexing " << endl;
		// index->writeIndex();
		g.AddObserver((Grail&)*index);
	}else if(strcmp(argv[typeFileArg],"bmg")==0){
		index = new BfsGrail(g,dim,0,10);
		cout << "finished indexing " << endl;
		// index->writeIndex();
		g.AddObserver((BfsGrail&)*index);
	}else if(strcmp(argv[typeFileArg],"bfs")==0){
		index = new BFSIndex(g);
		cout << "finished indexing " << endl;
		// index->writeIndex();
	}
	
	ifstream ob(argv[testFileArg],ios::in);
	process(g,*index,*dfsIndex,ob);
	cout << "Exiting!..." << endl;
	return 0;
}
