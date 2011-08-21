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
 * Vertex.h
 *
 *  Created on: Mar 11, 2011
 *      Author: yildih2
 */

#ifndef VERTEX_H_
#define VERTEX_H_
#include <unordered_map>
#include <iostream>
using namespace std;
typedef unordered_map<int,int> EdgeList;

class Vertex {
private:
	int id;


public:
	EdgeList inList;
	EdgeList outList;
	EdgeList& outEdges();
	EdgeList& inEdges();
	int outDegree();
	int inDegree();
	int degree();
	int hasEdge(int end);
	void addOutEdge(int tid);
	void addInEdge(int sid);
	void eraseOutEdge(int x);

	Vertex(int);
	Vertex();
	virtual ~Vertex();
};

#endif /* VERTEX_H_ */
