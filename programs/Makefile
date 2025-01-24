SOURCE1		= myprime
SOURCE2		= inter
SOURCE3		= prime1
SOURCE4		= prime2
SOURCE5		= prime3
CFLAGS		= -c
CC		    = gcc
OUT1		= myprime
OUT2		= inter
OUT3		= prime1
OUT4		= prime2
OUT5		= prime3
ARGS        = -l 50 -u 100 -w 3
INCLUDE = ../include
MODULES = ../modules
LDFLAGS = -lm

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall  -g -I$(INCLUDE)

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4) $(OUT5)

# Αρχεία .o
OBJS = $(SOURCE1).o $(MODULES)/ADTBList.o $(MODULES)/func.o
OBJS1 = $(SOURCE2).o $(MODULES)/ADTBList.o $(MODULES)/func.o
OBJS2 = $(SOURCE3).o $(MODULES)/func.o
OBJS3 = $(SOURCE4).o $(MODULES)/func.o
OBJS4 = $(SOURCE5).o $(MODULES)/func.o

$(OUT1): $(OBJS)
	$(CC) $(OBJS) -o $(OUT1)

$(OUT2): $(OBJS1)
	$(CC) $(OBJS1) -o $(OUT2)

$(OUT3): $(OBJS2)
	$(CC) $(OBJS2) -o $(OUT3)

$(OUT4): $(OBJS3)
	$(CC) $(OBJS3) -o $(OUT4) $(LDFLAGS)

$(OUT5): $(OBJS4)
	$(CC) $(OBJS4) -o $(OUT5) $(LDFLAGS)

run: $(OUT1)
	./$(OUT1) $(ARGS)

valgrind: $(OUT1)
	valgrind --leak-check=full ./$(OUT1) $(ARGS)

clean:	
	rm -f \
		$(SOURCE1).o $(OUT1) \
		$(SOURCE2).o $(OUT2) \
		$(SOURCE3).o $(OUT3) \
		$(SOURCE4).o $(OUT4) \
		$(SOURCE5).o $(OUT5) \