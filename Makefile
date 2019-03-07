all: fork_and_execv

fork_and_execv:
	gcc -o fork_and_execv main.c -std=c99

clean:
	rm -rf fork_and_execv
