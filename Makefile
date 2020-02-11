CXX = c++
FLAGS = -Wall -Wextra -std=c++14
SRC = bst.hpp
OBJ = $(SRC:.hpp=.o)

all: $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $^ -o $@ -c $(FLAGS)

clean:
	rm -f $(OBJ)
