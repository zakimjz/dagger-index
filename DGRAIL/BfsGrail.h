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
 * Grail.h
 *
 *  Created on: Mar 14, 2011
 *      Author: yildih2
 */

#ifndef BFSGRAIL_H_
#define BFSGRAIL_H_
#include "Graph.h"
#include "Grail.h"

using namespace::std;

class BfsGrail :public Grail{

public:
	BfsGrail();
	BfsGrail(Graph&, int, int, int );
	virtual ~BfsGrail();
	virtual int query(int,int);

private :
	virtual int rquery(int,int);

};

#endif /* BFSGRAIL_H_ */
