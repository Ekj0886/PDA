# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(INCDIR)

# Directories
SRCDIR = src
OBJDIR = obj
INCDIR = inc

# Source and object files
SRCS = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRCS)))

# Dependency files
DEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(notdir $(SRCS)))

# Name of the executable
TARGET = Legalizer

# Default rule
all: $(TARGET)

# Rule to create object directory if it doesn't exist
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Rule to build the target executable
$(TARGET): $(OBJS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Rule to compile the main file
$(OBJDIR)/main.o: main.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean rule
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Phony targets
.PHONY: all clean