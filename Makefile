
SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)
BIN = tParse

%.o: %.c
	gcc -g -Wall -c $< -o $@

all: $(BIN)

$(BIN): $(OBJS)
	gcc -g -Wall $^ -o $@

clean:
	rm -f $(OBJS) $(BIN)

