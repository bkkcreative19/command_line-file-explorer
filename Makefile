CXX = g++

CXXFLAGS = -g

TARGET = ./main

TARGET_DEL = main.exe

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET) run

$(TARGET): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lncurses

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@ 

run: $(TARGET)
	@$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)