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
 * DFSIndex.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: yildih2
 */

#include "DFSIndex.h"

DFSIndex::DFSIndex(Graph& graph): size(graph.size()),
		og(&graph), visited(graph.size()) {
}

DFSIndex::~DFSIndex() {
	// TODO Auto-generated destructor stub
}

int DFSIndex::query(int src, int trg){
	_opCnt++;
	return pquery(src,trg);
}


int DFSIndex::pquery(int src, int trg){
	visited[src] = _opCnt;
	if(src == trg)
		return 1;
//	cout << "src= " << src << " trg= " << trg << endl;
//	if(og->vertices.find(src)==og->vertices.end()){
//		cout << "this shouldn be " << endl;
//	}
	EdgeList &el = (*og)[src].outEdges();
	EdgeList::iterator it;
	for(it=el.begin();it!=el.end();it++){
//		cout << it->first << endl;
		if(visited[it->first]!=_opCnt){
			if(pquery(it->first,trg))
				return 1;
		}
	}
//	cout << "src= " << src << " trg= " << trg << endl;
	return 0;
}
