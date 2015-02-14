CC=g++
CFLAGS= -Wall -std=c++11
LDFLAGS= -Wall
SOURCES=main.cpp AirDB.cpp json_server.cpp
LIBS=-lboost_system -lcppnetlib-client-connections -lcppnetlib-server-parsers -lcppnetlib-uri -lpthread -lcrypto -lssl -lsqlite3
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=travel_agency_backend

all: $(OBJECTS) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@  $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)


clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
