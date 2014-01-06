LIBS = `pkg-config --libs opencv`

main: main.cpp
	g++ main.cpp $(LIBS) -o cv.out

run: main
	./cv.out
