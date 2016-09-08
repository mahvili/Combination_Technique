CC = g++
CFLAGS = -I.
.c.o:  ; $(CC) -c $(CFLAGS) $<

OBJ = 	CombinationTechnique.o 

all:  $(OBJ)
	$(CC) -o run_combination $(OBJ) $(CFLAGS)

%.o : %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $*.o

%.o : %.h
	$(CC) -c $(CFLAGS) $*.h -o $*.o

clean:
	rm -rf $(OBJ) run_combination Results 

CombinationTechnique.o        : Combination.h Definitions.h Fullgrid.h QuadTree.h Scheme.h Solution.h Visual.h	
