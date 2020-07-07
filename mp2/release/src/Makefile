# Flags: C11 standard & no compiler optimising
CFLAGS += -std=c11 -O0
# Libmath
LIBS += -lm

# making the executable from object file
mp2_20161603: mp2_20161603.o
	$(CC) $(CFLAGS) mp2_20161603.o -o mp2_20161603 $(LIBS)

# an object file is made from one source code
mp2_20161603.o: mp2_20161603.c
	$(CC) $(CFLAGS) -c mp2_20161603.c

clean:
	rm mp2_20161603 mp2_20161603.o
