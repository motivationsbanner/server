# https://bugs.launchpad.net/ubuntu/+source/gcc-defaults/+bug/1228201/comments/4
CPPFLAGS = -pthread -std=c++11 -Wall -Wl,--no-as-needed
LDLIBS = -lsfml-system -lsfml-network -lmysqlcppconn
RM = rm -rf
TOUCH = touch

all: bin/server

bin/server:  obj/Command.obj obj/Communicator.obj obj/Connection.obj obj/DatabaseConnection.obj obj/Game.obj obj/Logger.obj obj/main.obj obj/PacketHandler.obj obj/Session.obj
	$(CXX) -o bin/server obj/Command.obj obj/Communicator.obj obj/Connection.obj obj/DatabaseConnection.obj obj/Game.obj obj/Logger.obj obj/main.obj obj/PacketHandler.obj obj/Session.obj $(LDLIBS) $(CPPFLAGS)

obj/Command.obj: src/Command.cpp src/Command.h
	$(CXX) -c -o obj/Command.obj src/Command.cpp $(CPPFLAGS)

obj/Communicator.obj: src/Communicator.cpp src/Communicator.h src/Connection.h src/Logger.h
	$(CXX) -c -o obj/Communicator.obj src/Communicator.cpp $(CPPFLAGS)

obj/Connection.obj: src/Connection.cpp src/Connection.h src/Logger.h
	$(CXX) -c -o obj/Connection.obj src/Connection.cpp $(CPPFLAGS)

obj/DatabaseConnection.obj: src/DatabaseConnection.cpp src/DatabaseConnection.h src/Logger.h
	$(CXX) -c -o obj/DatabaseConnection.obj src/DatabaseConnection.cpp $(CPPFLAGS)

obj/Game.obj: src/Game.cpp src/Game.h
	$(CXX) -c -o obj/Game.obj src/Game.cpp $(CPPFLAGS)

obj/Logger.obj: src/Logger.cpp src/Logger.h
	$(CXX) -c -o obj/Logger.obj src/Logger.cpp $(CPPFLAGS)

obj/main.obj: src/main.cpp src/Communicator.h src/DatabaseConnection.h src/Game.h src/Logger.h src/PacketHandler.h
	$(CXX) -c -o obj/main.obj src/main.cpp $(CPPFLAGS)

obj/PacketHandler.obj: src/PacketHandler.cpp src/PacketHandler.h src/Logger.h
	$(CXX) -c -o obj/PacketHandler.obj src/PacketHandler.cpp $(CPPFLAGS)

obj/Session.obj: src/Session.cpp src/Session.h
	$(CXX) -c -o obj/Session.obj src/Session.cpp $(CPPFLAGS)

src/Command.h: src/Game.h

src/Communicator.h: src/Connection.h src/DatabaseConnection.h src/PacketHandler.h src/Session.h

src/Game.h: src/Communicator.h

src/PacketHandler.h: src/Communicator.h src/Connection.h src/DatabaseConnection.h

src/Session.h: src/Command.h src/Connection.h

clean:
	$(RM) bin/server obj/Command.obj obj/Communicator.obj obj/Connection.obj obj/DatabaseConnection.obj obj/Game.obj obj/Logger.obj obj/main.obj obj/PacketHandler.obj obj/Session.obj
