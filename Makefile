CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -ggdb
EXE=./zippity

.PHONY: clean

all: clean main

run: clean main
	$(call print_in_color,$(GREEN),\nRUNNING ... \n\n)
	@printf "$(RESET)"
	@$(EXE)

main: pages.o ui.o main.c
	$(call print_in_color,$(BLUE),Building main ... \n  --> )
	@printf "$(GRAY)"
	$(CC) $(CFLAGS) -o $(EXE) main.c ui.o pages.o -lncurses -lpanel
	@printf "$(RESET)"

ui.o: ui.c
	$(call print_in_color,$(YELLOW),Compiling ui.c to obj... \n  --> )
	@printf "$(GRAY)"
	$(CC) $(CFLAGS) -o ui.o -c ui.c
	@printf "$(RESET)"

pages.o: pages.c
	$(call print_in_color,$(YELLOW),Compiling pages.c to obj... \n  --> )
	@printf "$(GRAY)"
	$(CC) $(CFLAGS) -o pages.o -c pages.c
	@printf "$(RESET)"

clean:
	$(call print_in_color,$(RED),Cleaning ... \n  --> )
	@printf "$(GRAY)"
	rm -rf $(EXE) *.o *.i

# COLOR ALIASES
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
MAGENTA = \033[35m
CYAN    = \033[36m
BG      = \033[100m
GRAY    = \033[38;5;244m
RESET   = \033[0m

# colored output function
define print_in_color
	@printf "$1"
	@printf "$2"
	@printf "\033[0m"
endef
