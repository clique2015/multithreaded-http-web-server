all:
	clear
	g++ server.cpp parser.cpp scanner.cpp semantics.cpp -o server
	./server
