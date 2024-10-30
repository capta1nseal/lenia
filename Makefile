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
CXXFLAGS    := -std=c++23 -pthread -O3 $(CXXWARNFLAGS)
LINKFLAGS    = -Wl,-Bstatic -lSDL2 -Wl,--as-needed -Wl,-Bdynamic `pkg-config --cflags --libs --static sdl2 opencv4`

.PHONY: build test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^ $(CXXFLAGS) $(LINKFLAGS)

build: $(TARGET)

test: $(TARGET)
	cd  $(BINDIR); ../$(TARGET); cd ..

.SECONDEXPANSION:
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(@D)
	g++ $(CPPFLAGS) $(CXXFLAGS) $(LINKFLAGS) -o $@ -c $<

$(TREE): %:
	mkdir -p $(BINDIR)
	mkdir -p $@
	mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)

clean:
	$(RM) -r $(TARGET) $(BINDIR) $(OBJDIR) $(DEPDIR)

ifeq "$(MAKECMDGOALS)" ""
	-include $(DEPS)
endif
