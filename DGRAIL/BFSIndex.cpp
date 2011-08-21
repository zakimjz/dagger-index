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
 * BFSIndex.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: yildih2
 */

#include "BFSIndex.h"
#include <queue>
BFSIndex::BFSIndex(Graph& graph): size(graph.size()),
		og(&graph), visited(graph.size()) {
}

BFSIndex::~BFSIndex() {
	// TODO Auto-generated destructor stub
}

int BFSIndex::query(int src, int trg){
	_opCnt++;
	return pquery(src,trg);
}


int BFSIndex::pquery(int src, int trg){
	queue<int> myq;
	int next;
	myq.push(src);
	visited[src] = _opCnt;
	EdgeList::iterator it;
	while(!myq.empty()){
		next = myq.front();
		myq.pop();	
		if(next == trg)
			return 1;
		EdgeList &el = (*og)[next].outEdges();
		for(it=el.begin();it!=el.end();it++){
//		cout << it->first << endl;
			if(visited[it->first]!=_opCnt){
				myq.push(it->first);
				visited[it->first]=_opCnt;
			}
		}
	}
//	cout << "src= " << src << " trg= " << trg << endl;
	return 0;
}
