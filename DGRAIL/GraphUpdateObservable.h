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
 * GraphUpdateObservable.h
 *
 *  Created on: Mar 14, 2011
 *      Author: yildih2
 */

#ifndef GRAPHUPDATEOBSERVABLE_H_
#define GRAPHUPDATEOBSERVABLE_H_
#include <set>
#include <vector>
#include "GraphUpdateObserver.h"

class GraphUpdateObservable {
        set<GraphUpdateObserver*> observers;
    public:
       void AddObserver(GraphUpdateObserver& o){
    	    observers.insert(&o);
    	};
       void RemoveObserver(GraphUpdateObserver& o){
    	    observers.erase(&o);
    	};
       void NotifyEdgeDeletion(int src,int trg){
    	    set<GraphUpdateObserver*>::iterator itr;

    	    for ( itr = observers.begin();
    	          itr != observers.end(); itr++ )
    	    (*itr)->edgeDeleted(src,trg);

    	};

       void NotifyEdgeInsertion(int src,int trg){
    	    set<GraphUpdateObserver*>::iterator itr;

    	    for ( itr = observers.begin();
    	          itr != observers.end(); itr++ )
    	    (*itr)->edgeAdded(src,trg);

    	};

       void NotifyNodeInsertion(int src,vector<int>& incoming,vector<int>& outgoing){
    	    set<GraphUpdateObserver*>::iterator itr;

    	    for ( itr = observers.begin();
    	          itr != observers.end(); itr++ )
    	    (*itr)->nodeAdded(src,incoming,outgoing);

    	};

       void NotifyNodeDeletion(int src){
    	    set<GraphUpdateObserver*>::iterator itr;

    	    for ( itr = observers.begin();
    	          itr != observers.end(); itr++ )
    	    (*itr)->nodeDeleted(src);

    	};
       
       GraphUpdateObservable() { };

};

#endif /* GRAPHUPDATEOBSERVABLE_H_ */
