all:
	gcc -Wall -Werror -g reverse.c -o reverse
clean:
	rm reverse
	rm -rf tests-out/