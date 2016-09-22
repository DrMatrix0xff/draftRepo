
SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)
BIN = TParse

%.o: %.c
	gcc -g -Wall -c $< -o $@

all: $(BIN)

$(BIN): $(OBJS)
	gcc -g -Wall $^ -o $@

clean:
	rm -f $(OBJS) $(BIN)

