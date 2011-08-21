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
 * Index.h
 *
 *  Created on: Mar 25, 2011
 *      Author: yildih2
 */

#ifndef INDEX_H_
#define INDEX_H_

class Index {
protected:

public:
	virtual int query(int, int) = 0;
	virtual void writeIndex(){};
	virtual void integrityCheck(){};
	virtual void printStatistics(){};
	virtual void debug(int){};
};

#endif /* INDEX_H_ */
