# COMPILER that will be used
COMPILER= g++

# COMPILER FLAGS to prevent possible errors
CXXFLAGS=-std=c++11 -pedantic
CXXFLAGS+= -Wall -Wuninitialized -Werror
CXXFLAGS+= -fsanitize=address -fsanitize=undefined

# SDL2 FLAGS to correctly load SDL2 libraries
SDL2FLAGS= -lSDL2 -lSDL2_ttf -lSDL2_mixer

# FILES to consider in the final executable
CPP_FILES=./src/*.cpp
HEADERS=./src/*.h ./src/*.hpp

# EXECUTABLE name
EXECUTABLE=game

build: $(CPP_FILES)
	@echo "Compiling the game..."
	@$(CXX) $(CXXFLAGS) $(CPP_FILES) $(SDL2FLAGS) -o $(EXECUTABLE)
	

run:
	@clear
	@./$(EXECUTABLE)

clean:
	@echo "Removing game..."
	@rm $(EXECUTABLE)