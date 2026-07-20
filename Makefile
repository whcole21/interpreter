CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET   = interpreter
SRC      = src/token.cc \
           src/lexer.cc
OBJ      = $(SRC:src/%.cc=build/%.o)

TEST_SRC = tests/test_lexer.cc
TEST_BIN = test_runner

all: $(TARGET)

$(TARGET): $(OBJ) build/main.o
	$(CXX) $(CXXFLAGS) $(OBJ) build/main.o -o $(TARGET)

build/main.o: src/main.cc | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.cc | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(SRC)
	$(CXX) $(CXXFLAGS) -I src/ -I external/ $(TEST_SRC) $(SRC) -o $(TEST_BIN)

.PHONY: clean test
clean:
	rm -rf build $(TARGET) $(TEST_BIN)
