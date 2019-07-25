all:
	mkdir -p bld
	gcc -g -O2 src/mempool.c src/test.c test/runner.c -lgc -o bld/pcr-test-runner
	./bld/pcr-test-runner

clean:
	rm -rfv bld

