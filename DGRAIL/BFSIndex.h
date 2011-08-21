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
 * BFSIndex.h
 *
 *  Created on: Mar 25, 2011
 *      Author: yildih2
 */

#ifndef BFSIndex_H_
#define BFSIndex_H_
#include <unordered_map>
#include "Index.h"
#include "Graph.h"

using namespace::std;

class BFSIndex : public Index {
protected:
	int size;
	Graph* og;
	unordered_map<int,int> visited;
	int operationCounter;
	int _opCnt;

public:
	BFSIndex();
	BFSIndex(Graph&);
	int query(int, int);
	virtual ~BFSIndex();
private :
	int pquery(int, int);

};

#endif /* BFSIndex_H_ */
