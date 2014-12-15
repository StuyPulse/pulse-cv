INCLUDES = -Isrc/
LIBS = `pkg-config --libs opencv` `curl-config --libs`
DIRS = camera filter net
OBJS = $(patsubst src/%.cpp,obj/%.o, $(wildcard src/*.cpp) $(foreach d, $(DIRS), $(wildcard src/$(d)/*.cpp)))

main: dirs $(OBJS)
	g++ -o cv.out $(OBJS) $(LIBS)

obj/camera/%.o: src/camera/%.cpp src/camera/%.h
	g++ -c -o $@ $< $(INCLUDES)

obj/filter/%.o: src/filter/%.cpp src/filter/%.h
	g++ -c -o $@ $< $(INCLUDES)

obj/net/%.o: src/net/%.cpp src/net/%.h
	g++ -c -o $@ $< $(INCLUDES)

obj/%.o: src/%.cpp
	g++ -c -o $@ $<

dirs:
	@test -d obj || mkdir obj
	@for DIRECTORY in $(DIRS) ; do \
		test -d obj/$$DIRECTORY || mkdir obj/$$DIRECTORY; \
	done

clean:
	rm -f cv.out $(OBJS)

run: main
	./cv.out
