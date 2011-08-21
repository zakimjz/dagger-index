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
#ifndef GRAPH_UPDATE_OBSERVER_H_
#define GRAPH_UPDATE_OBSERVER_H_

// ---------------- Observer interface -----------------
class GraphUpdateObserver {
    public:
        virtual void edgeAdded(int,int) = 0;
        virtual void edgeDeleted(int,int) = 0;
        virtual void nodeAdded(int, vector<int>& , vector<int>&) = 0;
        virtual void nodeDeleted(int) = 0;
};

#endif /* GRAPH_UPDATE_OBSERVER_H_  */
