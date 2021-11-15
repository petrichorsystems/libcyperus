all:
	mkdir -p build/
	gcc -c -o build/libcyperus.o -Wall -Werror -fpic src/libcyperus.c
	gcc -c -o build/request.o -Wall -Werror -fpic src/request.c
	gcc -c -o build/parse.o -Wall -Werror -fpic src/parse.c
	gcc -shared -o build/libcyperus.so build/libcyperus.o build/request.o build/parse.o -luuid -llo -lpthread
clean:
	rm -r build/

test:
	mkdir -p build/test/
	gcc -Lbuild/ -o build/test/test -luuid -lcyperus src/test/test.c

lua:
	mkdir -p build/lua/
	gcc -Lbuild/ src/lua/Cyperus.c -shared -o build/lua/Cyperus.so -fPIC -llua -lcyperus -I/usr/include/lua5.4/
