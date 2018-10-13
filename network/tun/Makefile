CXX = g++
CXXFLAGS=-g -Wall -pthread

BINARIES=icmpecho rejecttcp accepttcp discardtcp connectmany

all: $(BINARIES)

icmpecho: faketcp.o icmpecho.o
	$(CXX) $(CXXFLAGS) $^ -o $@

rejecttcp: faketcp.o rejecttcp.o
	$(CXX) $(CXXFLAGS) $^ -o $@

accepttcp: faketcp.o accepttcp.o
	$(CXX) $(CXXFLAGS) $^ -o $@
	
discardtcp: faketcp.o discardtcp.o
	$(CXX) $(CXXFLAGS) $^ -o $@

connectmany: faketcp.o connectmany.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm *.o icmpecho rejecttcp accepttcp discardtcp connectmany
