
SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)

%.o: %.c
	gcc -g -c $< -o $@

all: $(OBJS)

clean:
	rm -f $(OBJS)
