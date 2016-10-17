SRC = main.c\
      make_nodes.c\
      re_parse.c\
      re_compile.c\
      re_vm.c

OBJS = $(SRC:%.c=%.o)

BIN = Re

%.o: %.c
	gcc -g -Wall -c $< -o $@

all: $(BIN)

$(BIN): $(OBJS)
	gcc -g -Wall $^ -o $@

$(OBJS): diyre.h

clean:
	rm -f $(OBJS) $(BIN)

.PHONY: clean

