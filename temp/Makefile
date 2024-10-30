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
LINKFLAGS    = -lSDL2 -lSDL2main -I/usr/local/include/opencv4 -L/usr/local/lib -L/usr/local/lib/opencv4/3rdparty -lopencv_gapi -lopencv_stitching -lopencv_highgui -lopencv_videoio -lopencv_ml -lopencv_video -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_imgproc -lopencv_core -llibprotobuf -lade -littnotify -lippiw -lippicv -ljpeg -lpng -ltiff -lOpenEXR-3_1 -lz -lfreetype -lharfbuzz -ldl -lm -lpthread -lrt

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
