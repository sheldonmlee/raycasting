TARGET = main

CC = g++

OBJD = obj
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=$(OBJD)/%.o)

CCFLAGS = -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: all run clean tags 

all: $(TARGET) tags

$(TARGET): $(OBJS)
	$(CC) $^ -o $(TARGET) $(LDFLAGS) $(CCFLAGS)

$(OBJS): $(OBJD)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) -c $? -o $@ $(CCFLAGS)

clean:
	rm -r $(TARGET) $(OBJD)

run: $(TARGET) tags
	./$(TARGET)

tags:
	ctags -R
