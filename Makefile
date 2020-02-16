CXX = g++
FLAGS = -Wall -Wextra -std=c++14
TEST_SRC = test.cpp
TEST_EXE = test
BNCH_SRC = benchmark.cpp
BNCH_EXE = benchmark
INCLUDE = bst

all: $(TEST_EXE) $(BNCH_EXE) $(IMAGE)

$(TEST_EXE): $(TEST_SRC)
	$(CXX) $^ -o $@ -I $(INCLUDE) $(FLAGS)

$(BNCH_EXE): $(BNCH_SRC)
	$(CXX) $^ -o $@ -I $(INCLUDE) $(FLAGS)

clean:
	rm -fr $(TEST_EXE) $(BNCH_EXE) $(IMAGE)

.PHONY: clean all
