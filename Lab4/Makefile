#
# Makefile för bruteforcelösning
#

CCC	= g++ -std=c++17 -g -O3
LFLAGS	= -lsfml-graphics -lsfml-window -lsfml-system -lm

all: brute fast

brute: brute.o point.o window.o gui_window.o text_window.o
	$(CCC) -o brute brute.o point.o window.o gui_window.o text_window.o $(LFLAGS)

fast: fast.o point.o window.o gui_window.o text_window.o
	$(CCC) -o fast fast.o point.o window.o gui_window.o text_window.o $(LFLAGS)

brute.o: brute.cpp point.h
	$(CCC) -o brute.o -c brute.cpp

fast.o: fast.cpp point.h
	$(CCC) -o fast.o -c fast.cpp

point.o: point.cpp point.h
	$(CCC) -o point.o -c point.cpp

window.o: window.cpp window.h point.h
	$(CCC) -o window.o -c window.cpp

gui_window.o: gui_window.cpp gui_window.h window.h point.h
	$(CCC) -o gui_window.o -c gui_window.cpp

text_window.o: text_window.cpp text_window.h window.h point.h
	$(CCC) -o text_window.o -c text_window.cpp

clean:
	@ \rm -rf *.o

zap: clean
	@ \rm -f fast brute *~
