
C_OBJS = FileIO.o
C_HEADERS = FileIO.h
EXE = P1

CC = g++
CFLAGS = -Wall -I. -c

$(EXE): $(C_OBJS)
	@echo "Linking..."
	$(CC) -o $(EXE) $(C_OBJS)
	@echo ""
	
%.o: %.cpp
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $<
	@echo ""
	
#$(C_OBJS):${C_HEADERS}
	
#clean up directory
clean:
	rm -rf *.o
	rm -rf $(EXE)
