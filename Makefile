LIB_SRC = src/string.c src/log.c src/mempool.c src/vector.c src/test.c \
          src/attribute.c src/sql.c src/resultset.c src/lua.c
TEST_SRC = test/string.c test/attribute.c test/sql.c test/resultset.c \
	   test/lua.c test/runner.c
TEST_OUT = bld/pcr-test-runner

all:
	mkdir -p bld
	gcc -g -O2 -Wall $(LIB_SRC) $(TEST_SRC) -lgc -llua -o $(TEST_OUT)
	./$(TEST_OUT)

doc:
	doxygen Doxyfile

clean:
	rm -rfv bld doc

