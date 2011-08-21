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
 * BfsGrail.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: yildih2
 */

#include "BfsGrail.h"
#include <queue>

BfsGrail::BfsGrail(Graph& graph, int Dim, int labelingType, int increment)
		: Grail(graph,Dim,labelingType, increment) {
	cout << "finished indexing " << endl;
}

BfsGrail::~BfsGrail() {
	// TODO Auto-generated destructor stub
}

 int BfsGrail::query(int src, int trg){
	if(src>=0)
		src = dag.getScc(src);
	if(trg>=0)
		trg = dag.getScc(trg);
	if(src == trg){
		return 1;
	}
	_opCnt++;
	return rquery(src,trg);
}

 int BfsGrail::rquery(int src, int trg){
	visited[src] = _opCnt;
//	dag.writeGraph(cout);
	if(src == trg)
		return 1;
	queue<int> myq;
	int next,nextsrc;
	myq.push(src);
	EdgeList::iterator it;
	
	while(!myq.empty()){
		nextsrc = myq.front();
		myq.pop();	
		
		EdgeList &el = dag.getOutEdgesFast(nextsrc);	
		for(it=el.begin();it!=el.end();it++){
			next = (it->first>=0) ? dag.getScc(it->first) : it->first;
			if(next == trg)
				return 1;
			if(visited[next]!=_opCnt && contains(next,trg)){
				myq.push(next);
				visited[next]=_opCnt;
			}
		}
	}
	return 0;
}
