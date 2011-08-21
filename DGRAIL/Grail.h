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
/*
 * Grail.h
 *
 *  Created on: Mar 14, 2011
 *      Author: yildih2
 */

#ifndef GRAIL_H_
#define GRAIL_H_
#include <vector>
#include <map>
#include "Graph.h"
#include "Index.h"
#include "DagWrapper.h"

using namespace::std;
class DagWrapper;

struct label{
	int* pre;
	int* post;
	label(int dim){
		pre = new int[2*dim];
		post = pre + dim;
	}
	label(){}
	label(label& cp,int dim){
		pre = new int[2*dim];
		post = pre + dim;
		for(int i=0;i<2*dim;i++){
			pre[i] = cp.pre[i];
		}
	}
	~label(){
//		delete[] pre;
	};
};

class compare_pairs {
	public:
		bool operator()(const pair<int,int> x,const pair<int,int> y) const {
			if(x.first == y.first) return x.second - y.second;
			return (x.first-y.first)<0; 
		}
};

typedef unordered_map<int, label> LabelList;
typedef unordered_map<int, int > MyMap;
typedef map<pair<int,int>, int , compare_pairs> PQueue;

class Grail :public GraphUpdateObserver, public Index{
protected:
	DagWrapper dag;
	int size;
	int dim;
	unordered_map<int, label> labels;
	MyMap visited;

	int operationCounter;
	int inc;
	int _opCnt;


public:
	Grail();
	Grail(Graph&, int, int, int );
	virtual ~Grail();
	void writeIndex();
	void integrityCheck();
	void printStatistics();
	void debug(int);
	virtual void edgeAdded(int, int);
	virtual void edgeDeleted(int, int);
	virtual void nodeAdded(int, vector<int>&, vector<int>&);
	virtual void nodeDeleted(int);
	virtual int query(int,int);
	virtual int contains(int,int);
	void dagEdgeAdded(int,int);
	void dagEdgeRemoved(int,int); 
	void componentsMerged(int center, vector<int>& list);
	void componentSplit(int center, MySet& clist, int trg);


private :
	void randomLabeling(int);
	void reverseLabeling(int);
	void visit(int node, int tra, int& post);
	void propagateUp(int, int, int);
	int findPaths(int, int , vector<int>&);
	int rquery(int,int);
	void extractList(EdgeList::iterator first, EdgeList::iterator last, std::back_insert_iterator<vector<int> > result );
	void splitVisit(int s, MySet& clist, int stval, int endval, int d, int& counter, PQueue& pqueue);

	void propagateUpStart(int,int,int);
	void propagateUpEnd(int,int,int);
	void pQueueProcess(PQueue& pqueue, int d);
};

#endif /* GRAIL_H_ */
