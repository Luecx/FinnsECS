# Compiler options
CXX = g++
CXXFLAGS = -std=c++17 -march=native -O3 -DNDEBUG

# Libraries
LIBS =

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# Files
SRCS := $(sort $(shell find $(SRCDIR) -name '*.cpp'))
LIB_SRCS := $(filter-out $(SRCDIR)/main.cpp, $(SRCS))
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LIB_OBJS := $(LIB_SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXE := $(BINDIR)/ECS
LIB := $(LIBDIR)/libECS.a


# Warnings
CXXFLAGS += -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wno-unused-parameter

# Targets
all: $(EXE)

$(EXE): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)

.PHONY: all clean
