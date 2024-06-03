SRCDIR := src
BINDIR := bin
OBJDIR := obj
DEPDIR := dep
TARGET := $(BINDIR)/lenia

SRCS := $(shell find $(SRCDIR) -name "*.cpp")
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEPS := $(SRCS:$(SRCDIR)/%.cpp=$(DEPDIR)/%.d)
TREE := $(sort $(patsubst %/,%,$(dir $(OBJS))))

CPPFLAGS     = -MMD -MP -MF $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
CXXWARNFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wzero-as-null-pointer-constant -Wunused -Woverloaded-virtual -Wformat=2 -Werror=vla -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference
# add -march=native after -O3 if you wish to optimise the code for your machine. may not run on other machines.
CXXFLAGS    := -std=c++20 -O3 $(CXXWARNFLAGS)
INCLUDEFLAGS = -I/usr/local/include/opencv4 -L/usr/local/lib
LINKFLAGS    = -lSDL2 -lSDL2main -lopencv_imgproc -lopencv_core -ldl -lm -lpthread -lrt

.PHONY: build test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(INCLUDEFLAGS) $(LINKFLAGS)

build: $(TARGET)

test: $(TARGET)
	cd  $(BINDIR); ../$(TARGET); cd ..

.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)
	g++ $(CPPFLAGS) $(CXXFLAGS) $(INCLUDEFLAGS) $(LINKFLAGS) -o $@ -c $<

$(TREE): %:
	mkdir -p $(BINDIR)
	mkdir -p $@
	mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)

clean:
	$(RM) -r $(TARGET) $(BINDIR) $(OBJDIR) $(DEPDIR)

ifeq "$(MAKECMDGOALS)" ""
	-include $(DEPS)
endif
