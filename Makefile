all:
	mkdir -p bld
	gcc test/runner.c -o bld/pcr-test-runner
	./bld/pcr-test-runner

clean:
	rm -rfv bld

