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
 * Graph.cpp
 *
 *  Created on: Mar 11, 2011
 *      Author: yildih2
 */
#include <istream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "Graph.h"

	Graph::Graph(){
		vsize = 0;
		MAX_ID = -1;
		MIN_ID = 2000000000;
	}
	Graph::Graph(int size):vsize(size){
		edgeCount = 0;
		for(int i=0;i<size;i++){
			vertices.insert(VertexList::value_type(i,Vertex(i)));
		}
	}

	Graph::Graph(VertexList& vlist){
		vertices = vlist;
	}

	Graph::Graph(istream& in){
		MAX_ID = -1;
		MIN_ID = 2000000000;
		readGraph(in);
	}

	Graph::~Graph(){
	}

	int Graph::size(){
		return vsize;
	}

	Vertex& Graph::operator [](int index){
		return vertices[index];
	}

	Vertex& Graph::getVertex(int index){
		return vertices[index];
	}

	void Graph::insertEdge(int sid, int tid){
		vertices[sid].addOutEdge(tid);
		vertices[tid].addInEdge(sid);
		edgeCount++;
		NotifyEdgeInsertion(sid,tid);
	}

	void Graph::deleteEdge(int sid, int tid){
		vertices[sid].outEdges().erase(tid);
		vertices[tid].inEdges().erase(sid);
		edgeCount--;
		NotifyEdgeDeletion(sid,tid);
	}
	
	void Graph::insertNode(int nid, vector<int>& incoming, vector<int>& outgoing){
		vsize++;
		vertices.insert(VertexList::value_type(nid,Vertex(nid)));

		if(nid>MAX_ID) MAX_ID = nid;
		if(nid<MIN_ID) MIN_ID = nid;

		EdgeList& el = vertices[nid].outEdges();
		for(int i=0;i<outgoing.size();i++){
			el[outgoing[i]]=1;
			vertices[outgoing[i]].addInEdge(nid);
		}
		NotifyNodeInsertion(nid,incoming,outgoing);
		edgeCount+=outgoing.size();
		
		for(int i=0;i<incoming.size();i++){
			insertEdge(incoming[i],nid);		
		}
	}

/* Old Delete Node : First delete in edges then remove
	void Graph::deleteNode(int nid){
		int next;
		vsize--;
		EdgeList& el = vertices[nid].inEdges();
//		edgeCount-=el.size();
		while(el.size()>0){
			deleteEdge(el.begin()->first,nid);
		}
		NotifyNodeDeletion(nid);
		EdgeList& el2 = vertices[nid].outEdges();
		edgeCount-=el2.size();
		EdgeList::iterator eit = el2.begin();
		while(eit != el2.end()){
			next = eit->first;
			vertices[next].inEdges().erase(nid);
			eit++;
		}
		vertices.erase(nid);
	}
*/

	// New Delete Node : First delete outedges then remove
	void Graph::deleteNode(int nid){
		int next;
		vsize--;
		EdgeList& el = vertices[nid].outEdges();
//		edgeCount-=el.size();
		while(el.size()>0){
			deleteEdge(nid,el.begin()->first);
		}
		NotifyNodeDeletion(nid);
		EdgeList& el2 = vertices[nid].inEdges();
		edgeCount-=el2.size();
		EdgeList::iterator eit = el2.begin();
		while(eit != el2.end()){
			next = eit->first;
			vertices[next].outEdges().erase(nid);
			eit++;
		}
		vertices.erase(nid);
	}

	int Graph::edgeSize(){
		return edgeCount;
	}

	void Graph::readGraph(istream& in) {
		string buf;
		getline(in, buf);

		strTrimRight(buf);
		if (buf != "graph_for_greach") {
			cout << "BAD FILE FORMAT!" << endl;
			exit(0);
		}

		int n;
		getline(in, buf);
		istringstream(buf) >> n;
		// initialize
		vsize = n;
		edgeCount = 0;
//		for (int i = 0; i < n; i++)
//			vertices.insert(VertexList::value_type(i,Vertex(i)));

		string sub;
		int idx;
		int sid = 0;
		int tid = 0;
		while (getline(in, buf)) {
			strTrimRight(buf);
			istringstream(buf) >> sid;
			idx = buf.find(":");
			buf.erase(0, idx+2);
			if(sid < 0){
				cout << "Wrong sid " << sid << endl;
				exit(0);
			}
			if(vertices.find(sid)==vertices.end()){
				vertices.insert(VertexList::value_type(sid,Vertex(sid)));
				if(sid>MAX_ID) MAX_ID = tid;
				if(sid<MIN_ID) MIN_ID = sid;
			}
			while (buf.find(" ") != string::npos) {
				sub = buf.substr(0, buf.find(" "));
				istringstream(sub) >> tid;
				buf.erase(0, buf.find(" ")+1);
				if(sid == tid){
					// cout << "Self-edge " << sid << " skipping" << endl;
					continue;
				}
				if(tid < 0){
					cout << "Wrong tid " << tid << endl;
					exit(0);
				}
				if(vertices.find(tid)==vertices.end()){
					vertices.insert(VertexList::value_type(tid,Vertex(tid)));
					if(tid>MAX_ID) MAX_ID = tid;
					if(tid<MIN_ID) MIN_ID = tid;
					
				}
				insertEdge(sid,tid);
			}
		}
	}

	void Graph::writeGraph(ostream& out) {
//		cout << "Graph size = " << vsize << endl;
		out << "graph_for_greach" << endl;
		out << vsize << endl;

		EdgeList::iterator eit;
		VertexList::iterator vit;
		for (vit=vertices.begin(); vit!= vertices.end(); vit++) {
			EdgeList& el = vit->second.outEdges();
			if(el.size()==0 && vit->second.inEdges().size()==0)
				continue;
			out << vit->first << ": ";
			for (eit = el.begin(); eit != el.end(); eit++)
				out << (eit->first) << "-"<<(eit->second) << " ";
			out << "#" << endl;
		}
		cout << "In edges " << endl;
		for (vit=vertices.begin(); vit!= vertices.end(); vit++) {
			EdgeList& el = vit->second.inEdges();
			if(el.size()==0 && vit->second.outEdges().size()==0)
				continue;
			out << vit->first << ": ";
			for (eit = el.begin(); eit != el.end(); eit++)
				out << (eit->first) << "-"<<(eit->second) << " ";
			out << "#" << endl;
		}


	}

	void Graph::writeGra(ostream& out){
		out << "graph_for_greach" << endl;
		out << vsize << endl;

		EdgeList::iterator eit;
		VertexList::iterator vit;
		int i = 0;
		for (vit=vertices.begin(); vit!= vertices.end(); vit++) {
			EdgeList& el = vit->second.outEdges();
			out << vit->first << ": ";
			for (eit = el.begin(); eit != el.end(); eit++)
				out << (eit->first) << " ";
			out << "#" << endl;
		}
	

	}
	void Graph::strTrimRight(string& str) {
		string whitespaces(" \t");
		int index = str.find_last_not_of(whitespaces);
		if (index != string::npos)
			str.erase(index+1);
		else
			str.clear();
	}


	int Graph::randomNodeSelection2(){
		int x,range = MAX_ID - MIN_ID + 1;
		do{
			x = (rand()%range) + MIN_ID;
		}while(vertices.find(x)==vertices.end());
		return x;
	}

	int Graph::randomNodeSelection(){
		VertexList& u = vertices;
    		size_t x = rand() % u.size();
    		size_t b;
    		for(b=0; b<u.bucket_count(); b++) {
      			if(x < u.bucket_size(b)) {
        			break;
     		 	} 
			else
       		 		x -= u.bucket_size(b);
    	    	}
   		VertexList::const_local_iterator l = u.begin(b);
    		while(x>0) {
      			l++;
     // 			assert(l!=u.end(b));
      			x--;
    		}
		return l->first;
	}
