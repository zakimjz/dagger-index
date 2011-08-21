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
// Name        : DGRAIL.cpp
// Author      : Hilmi Yildirim
// Version     :
// Copyright   : 
// Description : This is the tester program for the DynamicGRAIL class.
//
//
// TODO : Keep a mapping from arbitrary node labels to internal labels
//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <sys/time.h>
#include "Graph.h"
#include "Grail.h"
#include "BfsGrail.h"
//#include "MinimalGrail.h"
#include "DFSIndex.h"
#include "BFSIndex.h"

using namespace std;

float qTime,iTime,dTime,rTime,aTime;
int qCnt,iCnt,dCnt,rCnt,aCnt;
struct timeval before_time,after_time;

int QpU = 0;
int upCnt = 100000000;

void process(Graph &g, Index& index, istream& in){
	string buf;
	vector<pair<int,int> > nodes;
	vector<char> info;

	int i=0,x,y;
	char c;
	
	while(--upCnt && getline(in,buf)){
		istringstream is(buf);	
		is >> skipws >> c;
		if(c == '#')
			continue;
//		cout << buf << endl;
		if(QpU){
			for(int i=0;i<QpU;i++){
				x = g.randomNodeSelection2();
				y = g.randomNodeSelection2();
				if( g.vertices.find(x) == g.vertices.end() ||  g.vertices.find(y) == g.vertices.end())
					continue;
				if(g[x].outDegree()==0)
					continue;
				gettimeofday(&before_time,NULL);
				index.query(x,y); 
				gettimeofday(&after_time,NULL);
				qTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
					(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
				qCnt++;
			}
		}	
//		index.integrityCheck();
		switch(c){
			case 'q' : 
					is >> skipws >> x >> y;
					if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - query skipped" << endl;
						break;
					}
					gettimeofday(&before_time,NULL);
					index.query(x,y); 
					gettimeofday(&after_time,NULL);
					qTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
						(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
					qCnt++;
					break;
			case 'd' : 
					is >> skipws >> x >> y;
					if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - edge deletion aborted" << endl;
						continue;
						//exit(0);
					}
					if(!g[x].hasEdge(y)){
						cout << "the edge (" << x << ","<<y <<") does not exist - edge deletion aborted " << endl;
						continue;
					}
					gettimeofday(&before_time,NULL);
					g.deleteEdge(x,y);
					gettimeofday(&after_time,NULL);
					dTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
						(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
//					cout << "Deleted " << x << " " << y  << " at " << dTime << endl;
					dCnt++;
					 break;
			case 'i' : 	
					is >> skipws >> x >> y;
					if(g.vertices.find(x)== g.vertices.end() || g.vertices.find(y) == g.vertices.end()){
						cout << x << " or " << y << " does not exist - edge insertion aborted" << endl;
						continue;				
	//	exit(0);
					}
					if(g[x].hasEdge(y)){
						cout << "the edge (" << x << ","<<y <<") already exists - edge insertion aborted " << endl;
						continue;
					}
					gettimeofday(&before_time,NULL);
					g.insertEdge(x,y);
					gettimeofday(&after_time,NULL);
					iTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
						(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
					iCnt++;
	 				break;
			case 'r' :
					is >> skipws >> x ;
					if(g.vertices.find(x)== g.vertices.end()){
						cout << x << " does not exist - node deletion aborted" << endl;
						continue;
						//exit(0);
					}
					gettimeofday(&before_time,NULL);
					g.deleteNode(x);
					gettimeofday(&after_time,NULL);
					rTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
						(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
					rCnt++;
					break;
			case 'a' :
						int noofincoming,noofoutgoing,next;
						is >> skipws >> x >> noofincoming ;
						if(g.vertices.find(x) != g.vertices.end()){
							cout << x << " already exists - node addition aborted" << endl;
						//	continue;
							exit(0);
						}
						vector<int> inedges;
						for(int i=0;i<noofincoming; i++){
							is >> skipws >> next;
							if(g.vertices.find(next) == g.vertices.end()){
								cout << next << " does not exists - node addition aborted" << endl;
								continue;
								//exit(0);
							}
							inedges.push_back(next);
						}
						is >> skipws >> noofoutgoing ;
						vector<int> outedges;
						for(int i=0;i<noofoutgoing; i++){
							is >> skipws >> next;
							if(g.vertices.find(next) == g.vertices.end()){
								cout << next << " does not exists - node addition aborted" << endl;
								continue;
								//exit(0);
							}
							outedges.push_back(next);
						}
						gettimeofday(&before_time,NULL);
						g.insertNode(x,inedges,outedges);
						gettimeofday(&after_time,NULL);
						aTime += (after_time.tv_sec - before_time.tv_sec)*1000.0 + 
							(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
						aCnt++;
						break;
		}
//		index.debug(0);
//		index.writeIndex();
//		g.writeGraph(cout);
	}

	cout << "Total Query Time = " << qTime << " Avereage Query Time " << (qTime/qCnt) << endl;
	cout << "Total Insert Time = " << iTime <<  " Avereage Insert Time " << (iTime/iCnt) <<endl;
	cout << "Total Delete Time = " << dTime <<  " Avereage Delete Time " << (dTime/dCnt) <<endl;
	cout << "Total Add Time = " << aTime <<  " Avereage Add Time " << (aTime/aCnt) <<endl;
	cout << "Total Remove Time = " << rTime <<  " Avereage Remove Time " << (rTime/rCnt) <<endl;
	cout << "Total Time = " << qTime + iTime + dTime + aTime + rTime << endl;

}


int main(int argc, char **argv) {
	Index * index;
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
		}else if(strcmp(argv[i],"-n")==0){
			upCnt = atoi(argv[++i]);
		}
	}

	if(!graphFileArg || !testFileArg || !typeFileArg){
		cout << " Incomplete args " << endl;
		// printUsage();
	}
	
	ifstream ib(argv[graphFileArg],ios::in);
	Graph g(ib);
//	DagWrapper dw(&g);
	
	gettimeofday(&before_time,NULL);
	if(strcmp(argv[typeFileArg],"mg")==0){
		index = new Grail(g,dim,0,10);
		//index = new MinimalGrail(g,dim,0,10);
		g.AddObserver((Grail &)*index);
	}else if(strcmp(argv[typeFileArg],"bmg")==0){
		index = new BfsGrail(g,dim,0,10);
		//index = new MinimalGrail(g,dim,0,10);
		g.AddObserver((BfsGrail &)*index);
	}else if(strcmp(argv[typeFileArg],"dfs")==0){
		index = new DFSIndex(g);
	}else if(strcmp(argv[typeFileArg],"bfs")==0){
		index = new BFSIndex(g);
	}

	gettimeofday(&after_time,NULL);
	cout  << "construction time " <<  ((after_time.tv_sec - before_time.tv_sec)*1000.0 + 
						(after_time.tv_usec - before_time.tv_usec)*1.0/1000.0 ) << endl;

	ifstream ob(argv[testFileArg],ios::in);
	cout << " Statistics at the beginning " << endl;
//	if(strcmp(argv[typeFileArg],"mg")==0)
	index->printStatistics();
	process(g,*index,ob);
//	if(strcmp(argv[typeFileArg],"mg")==0)
	cout << " Statistics at the end " << endl;
	index->printStatistics();
//	g.writeGra(cout);
//	index->writeIndex();
	//cout << "Dag Representation!..." << endl;
	//dw.writeGra(cout);
	return 0;
}
