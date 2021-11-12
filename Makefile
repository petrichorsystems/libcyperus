all:
	mkdir -p build/
	gcc -c -o build/libcyperus.o -Wall -Werror -fpic src/libcyperus.c
	gcc -shared -o build/libcyperus.so build/libcyperus.o -luuid -llo -lpthread

clean:
	rm -r build/
	rm -r test/

test:
	mkdir -p test/
	gcc -Lbuild/ -o test/test -luuid -lcyperus src/test/test.c
