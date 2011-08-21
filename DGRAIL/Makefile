CXFLAGS =	-O2 -Wall -fmessage-length=0

OBJS = Vertex.o Graph.o DFSIndex.o BFSIndex.o DagWrapper.o Grail.o BfsGrail.o
# OBJS = Vertex.o Graph.o Grail.o GraphUtil.o MinimalGrail.o DFSIndex.o

CCX = g++ $(CXFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -std=c++0x $<
 
all:	DGRAIL testGenerator baGenerator erGenerator correctness graphExtractor graphExtractorFromPairs pairFileSorter

DGRAIL:	DGRAIL.cpp DGRAIL.o $(OBJS)
	$(CCX) DGRAIL.o $(OBJS) -o $@

correctness:	correctness.cpp correctness.o $(OBJS)
	$(CCX) correctness.o $(OBJS) -o $@

graphExtractor:   graphExtractor.cpp graphExtractor.o $(OBJS)
	$(CCX) graphExtractor.o $(OBJS) -o $@

graphExtractorFromPairs:   graphExtractorFromPairFile.cpp graphExtractorFromPairFile.o $(OBJS)
	$(CCX) graphExtractorFromPairFile.o $(OBJS) -o $@

pairFileSorter:   pairFileSorter.cpp pairFileSorter.o $(OBJS)
	$(CCX) pairFileSorter.o $(OBJS) -o $@

testGenerator:	testGenerator.cpp testGenerator.o Vertex.o Graph.o Grail.o
	$(CCX) testGenerator.o Vertex.o Graph.o -o $@

baGenerator:	baGenerator.cpp baGenerator.o Vertex.o Graph.o
	$(CCX) baGenerator.o Vertex.o Graph.o -o $@

erGenerator:	erGenerator.cpp erGenerator.o Vertex.o Graph.o
	$(CCX) erGenerator.o Vertex.o Graph.o -o $@

clean:
	rm -f $(OBJS) $(TARGET) DGRAIL correctness testGenerator  baGenerator graphExtractor erGenerator graphExtractorFromPairs pairFileSorter *.o
