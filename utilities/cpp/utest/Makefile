
LIB_FLAGS = -larmadillo
#LIB_FLAGS = -lblas -llapack 
#LIB_FLAGS = -lopenblas -llapack 
CXX = clang++-14
#CXX_FLAGS = -std=c++11 -Wshadow -Wall -pedantic -O0
CXX_FLAGS = -std=c++17 -I../../../vcpkg/packages/range-v3_x64-linux/include/ -Wshadow -Wall -pedantic -Og
#CXX_FLAGS = -std=c++11 -Wshadow -Wall -pedantic -Og -fopenmp
#CXX_FLAGS = -std=c++11 -Wshadow -Wall -pedantic -Og -DARMA_DONT_USE_WRAPPER
#CXX_FLAGS = -std=c++11 -Wshadow -Wall -pedantic -Og -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=bounds -fsanitize=bounds-strict -g


OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o: %.cpp $(DEPS)
	$(CXX) $(CXX_FLAGS) -o $@ -c $<

main: $(OBJECTS)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJECTS) $(LIB_FLAGS)


all: main

.PHONY: clean

clean:
	rm -f main *.o
