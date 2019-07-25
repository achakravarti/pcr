all:
	mkdir -p bld
	gcc -g -O2 test/runner.c -o bld/pcr-test-runner
	./bld/pcr-test-runner

clean:
	rm -rfv bld

