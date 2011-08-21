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
 * Graph.h
 *
 *  Created on: Mar 11, 2011
 *      Author: yildih2
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include <unordered_map>
#include <iostream>
#include "Vertex.h"
#include "GraphUpdateObservable.h"
using namespace std;

typedef unordered_map<int, Vertex> VertexList;

class Graph : public GraphUpdateObservable {
private:

	
public:
	Graph();
	Graph(int);
	Graph(VertexList&);
	Graph(istream&);
	
	VertexList vertices;
	int vsize;
	int edgeCount;
	int MAX_ID;
	int MIN_ID;	


	void readGraph(istream&);
	void writeGraph(ostream&);
	void writeGra(ostream&);
	void insertEdge(int sid, int tid);
	void deleteEdge(int sid, int tid);
	void insertNode(int sid, vector<int>&, vector<int>&);
	void deleteNode(int sid);

	Vertex& operator[](int index);
	Vertex& getVertex(int index);
	
	int size();
	int randomNodeSelection();
	int randomNodeSelection2();
	int edgeSize();
	virtual ~Graph();

private:
	void strTrimRight(string& );
};

#endif /* GRAPH_H_ */
