all:
	mkdir -p bld
	gcc -g -O2 -Wall src/string.c src/log.c src/mempool.c src/vector.c src/test.c src/attribute.c src/sql.c src/resultset.c test/runner.c -lgc -o bld/pcr-test-runner
	./bld/pcr-test-runner

clean:
	rm -rfv bld

