FLAGS = -g -std=c11 -Wall -Wextra -O2 -coverage -o bld/pcr-test-runner -lgc

all:
	mkdir -p bld
	gcc $(FLAGS) src/log.c src/mempool.c src/vector.c src/test.c test/runner.c
	./bld/pcr-test-runner

clean:
	rm -rfv bld
	rm -f *.gcda *.gcno

