# Makefile

TARGET=simulation
SRCDIR=src
INCDIRS=include
BUILDDIR=build

CXX=g++
CXXFLAGS=-std=c++20 -g -fPIC -MMD -MP $(addprefix -I,$(INCDIRS)) -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lsimlib

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

.PHONY: rebuild run clean

rebuild:
	$(MAKE) clean
	$(MAKE)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILDDIR) $(TARGET)