# Makefile

TARGET=simulation
SRCDIR=src
INCDIRS=include
BUILDDIR=build
SIMDIR=sim

CXX=g++
CXXFLAGS=-std=c++20 -g -fPIC -MMD -MP $(addprefix -I,$(INCDIRS)) -I/usr/local/include
LDFLAGS=-lsimlib

SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPS=$(OBJECTS:.o=.d)

all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	@mkdir -p $@

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: rebuild run sim zip clean sim-clean

rebuild:
	$(MAKE) clean
	$(MAKE)

run: $(TARGET)
	./$(TARGET)

sim: $(TARGET)
	cd $(SIMDIR) && ./run.sh

zip:
	zip T9_xhrbac10_xprochp00.zip -r docs/ include/ sim/ src/ Makefile technicka_sprava.pdf

clean:
	rm -rf $(BUILDDIR) $(TARGET)

sim-clean:
	rm -f sim/*/*.out