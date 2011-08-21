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
 * DagWrapper.cpp
 *
 *  Created on: Mar 31, 2011
 *      Author: yildih2
 */

#include <istream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator> 
#include "DagWrapper.h"
#include "Grail.h"

/******************************************************************************
 * CONSTRUCTION & DESTRUCTION
 */


DagWrapper::DagWrapper(Graph& org):orig(org){
	nextNumber = -1;
	dagsize = 0;
	constructDag();
}

//DagWrapper::DagWrapper(){}

DagWrapper::~DagWrapper(){
}

void DagWrapper::printStatistics(){
	int max = 1;
	VertexList::iterator vit;
	for(vit=vertices.begin();vit!=vertices.end();vit++){
		if(csize[vit->first] > max)
			max = csize[vit->first];
	}
	cout << "# nodes in DAG : " << dagsize << endl;
	cout << "# nodes in the largest SCC : " << max << endl;
	cout << "# nodes in the input graph : " << orig.vsize << endl;
	cout << "# edges in the input graph : " << orig.edgeCount << endl;
}


void DagWrapper::tarjan(int node){
//	cout << "In Tarjan " << node << endl;
	high[node] = low[node] = _tIndex++;
	nodeStack.push(node);
	inStack.insert(node);
//	cout << "In Tarjan " << node << endl;
	if(orig.vertices.find(node)== orig.vertices.end()){
		cout << " this shouldnt be the case " << endl;
	} 	
	EdgeList& el = orig[node].outEdges();
	for (EdgeList::iterator eit = el.begin(); eit != el.end(); eit++) {
		int tmp2;
		if (high.find(eit->first)== high.end()) {
			tarjan(eit->first);
			if( low[node] > (tmp2 = low[eit->first]))
				low[node] = tmp2;
		}
		else if (inStack.find(eit->first) != inStack.end()) {
			if( low[node] > (tmp2 = high[eit->first]))
				low[node] = tmp2;
		}
	}
	
	if(high[node] == low[node]){
		dagsize++;
		int tmp1 = nodeStack.top();
		if(tmp1 == node){
			nodeStack.pop();
			inStack.erase(tmp1);
//			cout << "Single " << tmp1 << endl;
		}
		else{
			int sccId = getNextId(), sz=0;
//			cout << sccId << " " ;
			vertices.insert(VertexList::value_type(sccId,Vertex(sccId)));
			while(!nodeStack.empty()){
				sccmap[tmp1] = sccId;
//				cout << " " << tmp1 ;
				inStack.erase(tmp1);
				nodeStack.pop();
				sz++;
				if(tmp1 == node){
					csize[sccId] = sz;
//					cout << endl;
					break;
				}
				tmp1 = nodeStack.top();
			}
		}
	}
	
}


void DagWrapper::constructDag(){

	int s,t;
	_tIndex = 1;
	MyMap::iterator mit, mit2;
	EdgeList::iterator eit;
	
	VertexList::iterator it;
	for(it=orig.vertices.begin(); it!= orig.vertices.end(); it++){
		if(high.find(it->first) == high.end()){
			tarjan(it->first);
		}
	}
	// arrange the edges of the supernodes
	it = orig.vertices.begin();
	while(it!=orig.vertices.end()){
		if((it->second).inDegree() == 0)
			roots.insert(it->first);
		s = getScc(it->first);
		EdgeList _trick;
		EdgeList& el = getOutEdges(it->first,_trick);
		for (eit = el.begin(); eit != el.end(); eit++) {
				t = getScc(eit->first);
				addDagEdge(s,t,eit->second);
		}			
		it++;
	}
	
	// add the root supernodes
	for(it = vertices.begin() ; it!=vertices.end(); it++){
		if((it->second).inDegree() == 0)
			roots.insert(it->first);
	}

//    writeGraph(cout);
}

/**********************************************************************
 * ACCESS FUNCTIONS
 */

int DagWrapper::dagSize(){
	return dagsize;
}

int DagWrapper::getNextId(){
	return nextNumber--;
}

int DagWrapper::getNodeSize(int index){
	return index >= 0 ? 1 : csize[index];
}


int DagWrapper::addDagEdge(int src, int trg, int inc=1){
	int res = 0;
	if(src==trg)
		return 0;

	if(src<0)
		res = vertices[src].outEdges()[trg]+=inc;

	if(trg<0)
		res = vertices[trg].inEdges()[src]+=inc;

	if(res==0){
		return 1;
	}
	return res;
}
	
int DagWrapper::removeDagEdge(int src, int trg){
	int res = 0;
//	cout << " in remove edge " << src << " " << trg << endl;
	if(src<0){
		EdgeList& el = vertices[src].outEdges();
		if((res = --el[trg])==0)
			el.erase(trg);
	}
	if(trg<0){
		EdgeList& el = vertices[trg].inEdges();
		if((res = --el[src])==0)
			el.erase(src);
	}
	return res;
}

int DagWrapper::getScc(int index){

	int cur=index, lsize = 0,nextVal,tmp;
	MyMap::iterator next = sccmap.find(index);
	
	if(next==sccmap.end())
		return index;
	// find the root
	while(next!=sccmap.end()){
		templist.push_back(cur); lsize++;
		cur = next->second;
		next = sccmap.find(cur);
	}

	// flatten the links
	// if a node is redundant, remove
	for(int i=lsize-1;i>0;i--){
		if(csize[templist[i]] == (templist[i-1]>=0 ? 1 : csize[templist[i-1]])){
			vertices.erase(templist[i]);
			sccmap.erase(templist[i]);
			csize.erase(templist[i]);
		}else{
			sccmap[templist[i]] = cur;
			csize[templist[i]]-= (templist[i-1]>=0 ? 1 : csize[templist[i-1]]);
		}	
	}
	sccmap[index] = cur;
	templist.clear();
	return cur;
}

int DagWrapper::getSccOld(int index){
	int cur=index, lsize = 0,nextVal,tmp;
	MyMap::iterator next = sccmap.find(index);
	
	if(next==sccmap.end())
		return index;

	while(next!=sccmap.end()){
		templist.push_back(cur); lsize++;
		cur = next->second;
		next = sccmap.find(cur);
	}
	sccmap[templist[0]] = cur;
	nextVal = 1;
	
	for(int i=1;i<lsize;i++){
		sccmap[templist[i]] = cur;
		tmp = csize[templist[i]];
		//no one else is pointing anymore
		//we can safely remove the node
		if(tmp-nextVal == 0){	
			vertices.erase(templist[i]);
			sccmap.erase(templist[i]);
			csize.erase(templist[i]);
		}else{
			csize[templist[i]]-=nextVal;
			nextVal = tmp;
		}
	}
	templist.clear();
	return cur;
}

EdgeList& DagWrapper::getOutEdgesFast(int index){
	return (index >= 0)? orig[index].outEdges() : vertices[index].outEdges();
}

EdgeList& DagWrapper::getInEdgesFast(int index){
	return (index >= 0)? orig[index].inEdges() : vertices[index].inEdges();
}

EdgeList& DagWrapper::getOutEdges(int index, EdgeList& trick){
	if(index >= 0){
		trick.clear();
		EdgeList::iterator eit;
		EdgeList realEl = orig[index].outEdges();
		for(eit=realEl.begin();eit!=realEl.end();eit++){
			trick[getScc(eit->first)]++;
		}
		return trick;
	}else{
		return vertices[index].outEdges();
	}
}

EdgeList& DagWrapper::getInEdges(int index, EdgeList& trick){
	if(index >= 0){
		trick.clear();
		EdgeList::iterator eit;
		EdgeList realEl = orig[index].inEdges();
		for(eit=realEl.begin();eit!=realEl.end();eit++){
			trick[getScc(eit->first)]++;
		}
		return trick;
	}else{
		return vertices[index].inEdges();
	}
}

void DagWrapper::writeMap(){
	MyMap::iterator mit;
	for(mit = sccmap.begin(); mit!=sccmap.end();mit++){
		cout << mit->first << " -> " << mit->second << endl;
	}
	cout << "Sizes " << endl;
	for(mit = csize.begin(); mit!=csize.end();mit++){
		cout << mit->first << " -> " << mit->second << endl;
	}
}

Vertex& DagWrapper::operator [](int index){
	if(index >=0 )
		return orig[index];
	else
		return vertices[index];
}

Vertex& DagWrapper::getVertex(int index){
	if(index >=0 )
		return orig[index];
	else
		return vertices[index];
}

/*******************************************************
 * GRAPH UPDATE FUNCTIONS
 */

// EDGE INSERTION

void DagWrapper::insertEdge(int u, int v, Grail*gr){

	int center,s = getScc(u), t = getScc(v);
//	cout << "component edge from " << s << " to " << t << endl;
	vector<int> list;
	if(s==t)
		return;
	opCnt++;
//	cout << "IE 1 " << endl;
	if(gr->query(t,s)){
//		cout << "IE 2.1 " << endl;
		center = t;
		mergeComponents(t,s,list,center,gr);
//		cout << "IE 2.2 " << list.size() << endl;
		updateDagAfterMerge(center,list);
//		cout << "IE 2.3 " << list.size()<< endl;
		gr->componentsMerged(center,list);
//		cout << "IE 2.4 " << list.size() << endl;
		dagsize -= (list.size()-1);
//		cout << "Components Merged " << center << " " <<endl ;
	}
	else if(addDagEdge(s,t)>0){
//		cout << "IE 3.1 " << endl;
		gr->dagEdgeAdded(s,t);
//		cout << "IE 3.2 " << endl;
//		cout << "Dag Edge Added " << s << " " << t << endl;
		return;
	}
}

void DagWrapper::updateDagAfterMerge(int& center, vector<int>& list){
	int mapto;
	EdgeList _trick1,_trick2,_trick3,_trick4;
	EdgeList::iterator eit,it;
	if(center>=0){
		center = getNextId();
		vertices.insert(VertexList::value_type(center,Vertex(center)));
	}else{
		// remove intra-component edges (i.e. center to mergedcomponents edges)
		EdgeList &el = getOutEdges(center,_trick1);
		for(it=el.begin(); it!=el.end();){
			eit = it++;			// we use this to be able to delete while traversing
			if(visited[eit->first]==-opCnt){ // meaning it is in the list
				el.erase(eit);
			}
		}
		EdgeList &el2 = getInEdges(center,_trick2);
		for(it=el2.begin(); it!=el2.end();){
			eit = it++;			// we use this to be able to delete while traversing
			if(visited[eit->first]==-opCnt){ // meaning it is in the list
				el2.erase(eit);
			}
		}
	}

	EdgeList &ol = getOutEdges(center,_trick1);
	EdgeList &il = getInEdges(center,_trick2);

	for(int i=0;i<list.size();i++){
		if(list[i]!=center){
			csize[center]+=getNodeSize(list[i]);
			sccmap[list[i]] = center;
			EdgeList &el= getOutEdges(list[i],_trick3);
			for(EdgeList::iterator eit=el.begin(); eit!=el.end();eit++){
				if(eit->first!=center && visited[eit->first]!=-opCnt){ // meaning it is not in the list
					ol[eit->first]+=eit->second;
					if(eit->first < 0){ // if it is not a single node component
						vertices[eit->first].inEdges().erase(list[i]);
						vertices[eit->first].inEdges()[center]+=eit->second;
					}
				}
			}
			el.clear();
			EdgeList &el2 = getInEdges(list[i],_trick4);
			for(EdgeList::iterator eit=el2.begin(); eit!=el2.end();eit++){
				if(eit->first!=center && visited[eit->first]!=-opCnt){ // meaning it is not in the list
					il[eit->first]+=eit->second;
					if(eit->first < 0){
						vertices[eit->first].outEdges().erase(list[i]);
						vertices[eit->first].outEdges()[center]+=eit->second;
					}
				}
			}
			el2.clear();
		}
	}
}

/* Old Function : update the inEdges of the dag nodes
void DagWrapper::removeNode(int u){
	EdgeList& el2 = orig[u].outEdges();
	EdgeList::iterator eit = el2.begin();
	while(eit != el2.end()){
		int next = eit->first;
//		cout << "removed " << u << " from inedges of " << next << endl;
		if((next=getScc(next)) < 0){
//			cout << " removed " << u << " from " << next << endl;
			vertices[next].inEdges().erase(u);
		}
		eit++;
	}
	dagsize--;
	visited.erase(u);
	csize.erase(u);
}
*/

// New Function : update the outEdges of the dag nodes
void DagWrapper::removeNode(int u){
	EdgeList& el2 = orig[u].inEdges();
	EdgeList::iterator eit = el2.begin();
	while(eit != el2.end()){
		int next = eit->first;
//		cout << "removed " << u << " from inedges of " << next << endl;
		if((next=getScc(next)) < 0){
//			cout << " removed " << u << " from " << next << endl;
			vertices[next].outEdges().erase(u);
		}
		eit++;
	}
	dagsize--;
	visited.erase(u);
	csize.erase(u);
}

int DagWrapper::mergeComponents(int src, int trg, vector<int>& list, int & center, Grail* gr){
	visited[src] = opCnt;
	EdgeList _trick;
//	cout << "src = " << src << " trg = " << trg << " center = " << center << endl;
	if(src == trg){
		list.push_back(src);
		visited[src] = -opCnt;
		if(getNodeSize(src) > getNodeSize(center))
			center = src;
		return true;
	}
	EdgeList &el = getOutEdges(src,_trick);
	for(EdgeList::iterator eit = el.begin(); eit!= el.end();eit++){
//		cout << " el.size() = " << el.size() << endl; 
		if(gr->contains(eit->first,trg)){
			int temp = visited[eit->first];
			if (temp == -opCnt){
				visited[src] = -opCnt;
			}
			else if (temp != opCnt){
				if(mergeComponents(eit->first,trg,list,center,gr))
					visited[src] = -opCnt;
			}
		}
	}
	if(visited[src]==-opCnt){
		list.push_back(src);
		if(getNodeSize(src) > getNodeSize(center))
			center = src;
		return true;
	}
	return false;
}

// EDGE DELETION

void DagWrapper::deleteEdge(int u, int v, Grail*gr){
	int node,center,s = getScc(u), t = getScc(v);
	MySet clist;
	high.clear();
	low.clear();
//	writeMap();
//	cout << "BP 1 " << s << " " << t << endl;
	opCnt++;
	if(s!=t && removeDagEdge(s,t)>0){
//		cout << "BP 2.1 " << endl;
		gr->dagEdgeRemoved(s,t);
//		cout << " Dag Edge Removed " << s << " " << t << endl;
	}
	else if(s==t){
//		cout << "BP 3.1 " << endl;
		nodeQueue.push(u);
//		cout << "BP 3.2 " << endl;
		while(!nodeQueue.empty()){
//			cout << "BP 3.3 " << endl;
			node = nodeQueue.front();
//			cout << "BP 3.3 " << node << endl;
			nodeQueue.pop();
//			cout << "node= " << node << " size " << nodeQueue.size() << endl;
			if(abs(visited[node])!=opCnt){
//				cout << "BP 3.4 " << endl;
				if(node!=v)
					extractComponents(node,v,s,clist);
//				cout << "BP 3.5 " << endl;
			}
		}
		
		if(csize[s]==1){
//			cout << "BP 4 source : " << s << endl;
			sccmap.erase(v);
			csize.erase(s);
			high[v];
			clist.insert(v);
		}
/*		cout << " nodes in the clist " << endl;
		MySet::iterator vit;
		for(vit=clist.begin();vit!=clist.end();vit++){
			cout << " " << *vit;
		}
		cout << endl;
*/		
//		cout << "BP 5 " << endl;
		updateDagEdgesAfterSplit(s,clist);
//		cout << "BP 6 " << endl;
		if(clist.size()!=0)
			dagsize+=clist.size()-1;
//		cout << "BP 7" << endl;
		gr->componentSplit(s,clist,v);
//		cout << "BP 8" << endl;
	}
}

void DagWrapper::updateDagEdgesAfterSplit(int& oldcenter, MySet& clist){
	MyMap::iterator mit;
	EdgeList::iterator eit;
	EdgeList _trick1,_trick2,_trick3,_trick4,_trick5;
	int nextNode, nextComp,mapto;
	// these are all moved nodes

	for(mit=high.begin();mit!=high.end();mit++){
		nextNode = mit->first;
		nextComp = getScc(nextNode);

		if(nextNode == nextComp || nextComp==oldcenter){
			// single node component, skip it
			continue;
		}
		EdgeList &eol = getOutEdges(nextNode,_trick1);		
		EdgeList &col =getOutEdges(nextComp,_trick2);

		for(eit=eol.begin();eit!=eol.end();eit++){
			mapto = getScc(eit->first);
			if(mapto!=nextComp)
				col[mapto]+=eit->second;
		}

		EdgeList &eil = getInEdges(nextNode,_trick1);		
		EdgeList &cil = getInEdges(nextComp,_trick2);

		for(eit=eil.begin();eit!=eil.end();eit++){
			mapto = getScc(eit->first);
			if(mapto!=nextComp)
				cil[mapto]+=eit->second;
		}
	}
//	if(vertices[-57].outEdges().find(13177)!=vertices[-57].outEdges().end() 
//			&& vertices[-57].inEdges().find(13177)!=vertices[-57].inEdges().end())
//		cout << "already a problem here" << endl;
		
	EdgeList &ol=	getOutEdges(oldcenter,_trick1);
	EdgeList &il=	getInEdges(oldcenter,_trick2);
	MySet::iterator vit;
	for(vit=clist.begin();vit!=clist.end();vit++){

		EdgeList &col = getOutEdges(*vit,_trick3);
		for(eit=col.begin();eit!=col.end();eit++){
			if(eit->first!=oldcenter && clist.find(eit->first)==clist.end()){
				if((ol[eit->first]-=eit->second) <= 0)
					ol.erase(eit->first);
				if(eit->first < 0){
					EdgeList &tel= 	getInEdges(eit->first,_trick5);
					if( (tel[oldcenter]-=eit->second) <=0)
						tel.erase(oldcenter);
					tel[*vit]+=eit->second;
				}
			}
		}
		EdgeList &col2 = getInEdges(*vit,_trick4);
		for(eit=col2.begin();eit!=col2.end();eit++){
			if(eit->first==oldcenter){
				ol[*vit]+=eit->second;
			}
			else if(clist.find(eit->first)==clist.end()){
				if( (il[eit->first]-=eit->second) <= 0)
					il.erase(eit->first);
				if(eit->first < 0){
					EdgeList &tel =	getOutEdges(eit->first,_trick5);
					if((tel[oldcenter]-=eit->second) <= 0){
						tel.erase(oldcenter);
					}
					tel[*vit]+=eit->second;
				}
			}
		}	
	}
}


int DagWrapper::extractComponents(int node, int trg, int dagnode, MySet& clist){
	int tmp1,tmp2;
	if(node == trg){
//		cout << "reached target " << trg << endl;
		visited[node]= -opCnt;
		return false;
	}
	visited[node] = opCnt;
	high[node] = low[node] = _tIndex++;
	nodeStack.push(node);
	inStack.insert(node);
	EdgeList &el = orig[node].outEdges();
	EdgeList::iterator eit;
	for (eit = el.begin(); eit != el.end(); eit++) {
//		cout << "child of " << node << " : " << eit->first << endl;
		if(getScc(eit->first)==dagnode){
			tmp1= visited[eit->first];
			if(abs(tmp1)<opCnt){
//				cout << "child of " << node << " : " << eit->first << endl;
				if(!extractComponents(eit->first, trg, dagnode, clist)){
					visited[eit->first]=-opCnt;
					high.erase(node);
//					cout << eit->first << " is reaching " << endl;
					return false;
				}
				if( low[node] > (tmp2 = low[eit->first]))
					low[node] = tmp2;
			}else if(tmp1 == -opCnt){
				visited[eit->first]=-opCnt;
				high.erase(node);
//				cout << eit->first << " is reaching " << endl;
				return false;				
			}else if (inStack.find(eit->first) != inStack.end()) {
				if( low[node] > (tmp2 = high[eit->first]))
					low[node] = tmp2;
			}
/*			
			if(abs(visited[eit->first])!=opCnt){
				cout << "child of " << node << " : " << eit->first << endl;
				if(!extractComponents(eit->first, trg, dagnode, clist)){
					visited[eit->first]=-opCnt;
					high.erase(node);
					cout << eit->first << " is reaching " << endl;
					return false;
				}
				if( low[node] > (tmp2 = low[eit->first]))
					low[node] = tmp2;
			}else if (inStack.find(eit->first) != inStack.end()) {
				if( low[node] > (tmp2 = high[eit->first]))
					low[node] = tmp2;
			}
*/
		}
	}

	if(high[node] == low[node]){
//		cout << " new component at " << node << endl;
		formComponentDuringSplit(node, dagnode, clist);
	}
	return true;
}

void DagWrapper::formComponentDuringSplit(int node, int dagnode,MySet& clist){
	int next = nodeStack.top();
	EdgeList::iterator eit;
	
	// single node component
	if(next == node){		
		nodeStack.pop();
		inStack.erase(next);
		sccmap.erase(next);
		clist.insert(node);
		csize[dagnode]--;
		EdgeList& el2 =orig[next].inEdges();
		for (eit = el2.begin(); eit != el2.end(); eit++) {
//			cout << "parents of " << node << " : " << eit->first << endl;
			if(abs(visited[eit->first])!=opCnt && getScc(eit->first)==dagnode){
//				cout << " addign to the queue " << eit->first << endl;
				nodeQueue.push(eit->first);
			}
		}
//		cout << " found single node component on " << node << endl;
	}
	else{
		int sccId = getNextId(), sz=0;
		clist.insert(sccId);
		vertices.insert(VertexList::value_type(sccId,Vertex(sccId)));
		while(!nodeStack.empty()){
			next = nodeStack.top();
			sccmap[next] = sccId;
			inStack.erase(next);
			nodeStack.pop();
			sz++;
			EdgeList& el2 =orig[next].inEdges();
			for (eit = el2.begin(); eit != el2.end(); eit++) {
//				cout << "parents of " << node << " : " << eit->first << endl;
				if(abs(visited[eit->first])!=opCnt && getScc(eit->first)==dagnode){
//					cout << " addign to the queue " << eit->first << endl;
					nodeQueue.push(eit->first);
				}
			}
			if(next == node){
				csize[sccId] = sz;
				csize[dagnode]-= sz;
				break;
			}
		}
	}
}
 
