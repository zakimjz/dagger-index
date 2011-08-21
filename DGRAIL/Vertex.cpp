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
 * Vertex.cpp
 *
 *  Created on: Mar 11, 2011
 *      Author: yildih2
 */

#include "Vertex.h"
#include <sstream>
using namespace std;

Vertex::Vertex(int Id): id(Id){
}

Vertex::Vertex() {
}

Vertex::~Vertex() {
}

void Vertex::eraseOutEdge(int x){
	outList.erase(x);
}
EdgeList& Vertex::outEdges(){
	return outList;
}
EdgeList& Vertex::inEdges(){
	return inList;
}
int Vertex::outDegree(){
	return outList.size();
}
int Vertex::inDegree(){
	return inList.size();
}
int Vertex::degree(){
	return inList.size()+outList.size();
}
int Vertex::hasEdge(int endNode){
	return outList.find(endNode) != outList.end();
}
void Vertex::addInEdge(int sid){
	inList[sid]++;
}
void Vertex::addOutEdge(int tid){
	outList[tid]++;
}
