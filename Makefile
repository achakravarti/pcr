
LIB_INP = bld/string.o bld/log.o bld/mempool.o bld/vector.o bld/test.o \
	  bld/attribute.o bld/sql.o bld/resultset.o bld/lua.o
LIB_OUT = bld/libpcr.so
LIB_OPT = -shared -g -O2


TEST_INP = test/string.c test/attribute.c test/sql.c test/resultset.c \
	   test/lua.c test/runner.c
TEST_OUT = bld/pcr-test-runner
TEST_DEP = $(LIB_OUT) -lgc -llua
TEST_OPT = -g -O2 -Wall


$(TEST_OUT): $(LIB_OUT) $(TEST_INP)
	gcc $(TEST_OPT) $(TEST_INP) $(TEST_DEP) -o $@


$(LIB_OUT): $(LIB_INP)
	gcc $(LIB_OPT) $(LIB_INP) -o $@


bld/%.o: src/%.c
	mkdir -p bld; gcc -c -fPIC $(TEST_OPT) $< -o $@


clean:
	rm -rfv bld doc

doc:
	doxygen Doxyfile

install:
	sudo mkdir -p /usr/local/include/pcr
	sudo cp src/api.h /usr/local/include/pcr/api.h
	sudo cp $(LIB_OUT) /usr/local/lib

uninstall:
	sudo rm -rf /usr/local/include/pcr
	sudo rm -f /usr/local/lib/libpcr.so

test: $(TEST_OUT)
	./$(TEST_OUT)

