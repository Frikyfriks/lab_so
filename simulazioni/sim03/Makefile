NAME=coda
DEST=.
output: main.c
	@if [ -d "$(DEST)" ]; then \
        cd $(DEST); \
    else \
        echo "?ERROR"; \
    fi
	@gcc -std=gnu90 main.c -o $(DEST)/$(NAME)