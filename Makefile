NAME = main
INPUTFILES = siphtCaso01 cloud1p passwd network_g
#INPUTFILES = sipht_g cloud0 passwd_g network_g
SOURCES = $(wildcard *.cpp)
CASO1 = -DFIXEDSCH
CASO2 = -DRANDOMSCH
CASO3 = -DRANDOMSCH -DNODESCH
CASO4 = -DRANDOMSCH -DNODESCH -DCLOUDSCH
CASO5 = -DRANDOMSCH -DNODESCH -DCLOUDSCH -DBURSTSCH
CASO6 = -DRANDOMSCH -DNODESCH -DBURSTSCH
CASO  = $(CASO6)

OBJS = $(SOURCES:.cpp=.o)
INCLUDES = $(wildcard *.h)
CC = g++
%CFLAGS = -std=c++17 -g -DFATVM
%CFLAGS = -std=c++17 -g -DTHINVM
CFLAGS = $(CASO) -g -std=c++17 -ffast-math -march=native
EXEC = main
	
main : $(OBJS)
	$(CC) -o $(NAME) $^ 

%.o : %.cpp $(INCLUDES) Makefile
	$(CC) -c $(CFLAGS) $< -o $@

git:
	rm -rf *.o $(NAME)
	git add .
	git commit -m "Antes de fazer Node : public BareMetal"
	git push

clean:
	rm -rf *.o $(NAME)

run: $(EXEC)
	./$(EXEC) $(INPUTFILES)
