#
# Makefile för bildjämförelse
#

CXX      := g++ -std=c++17 -pipe
OPTIMIZE := -O3
FLAGS    := $(OPTIMIZE) -Wall -Wextra -g
LDFLAGS  := -pthread -lsfml-graphics -lsfml-window -lsfml-system
EXEC     := slow fast
MAKEDEPS := -MMD -MP

OBJECTS  := $(patsubst %.cpp,build/%.o,$(wildcard *.cpp))
DEPS     := $(patsubst %.o,%.d,$(OBJECTS))
SHAREDOBJECTS := $(filter-out $(addprefix build/,$(addsuffix .%,$(EXEC))),$(OBJECTS))

$(shell mkdir -p build)

.PHONY: all clean zap
all: $(EXEC)

$(EXEC): %: build/%.o $(SHAREDOBJECTS)
	$(CXX) $(FLAGS) -o $@ $^ $(LDFLAGS)

$(OBJECTS): build/%.o: %.cpp
	$(CXX) $(FLAGS) $(MAKEDEPS) -c -o $@ $<

clean:
	@ \rm -rf build/

zap: clean
	@ \rm -f $(EXEC) *~

-include $(DEPS)
