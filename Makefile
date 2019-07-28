FLAGS = -g -std=c11 -Wall -Wextra -O2 -coverage -o bld/pcr-test-runner -I /usr/include -lgc

all:
	mkdir -p bld
	$(CC) $(FLAGS) src/log.c src/mempool.c src/vector.c src/test.c test/runner.c
	./bld/pcr-test-runner

clean:
	rm -rfv bld
	rm -f *.gcda *.gcno

