all:
	mkdir -p build/
	gcc -g -c -o build/libcyperus.o -Wall -Werror -fpic src/libcyperus.c
	gcc -g -c -o build/request.o -Wall -Werror -fpic src/request.c
	gcc -g -c -o build/parse.o -Wall -Werror -fpic src/parse.c
	gcc -g -c -o build/modules.o -Wall -Werror -fpic src/modules.c
	gcc -g -shared -o build/libcyperus.so build/libcyperus.o build/request.o build/parse.o build/modules.o -luuid -llo -lpthread
clean:
	rm -r build/

test:
	mkdir -p build/test/
	gcc -g -Lbuild/ -o build/test/test -luuid -lcyperus src/test/test.c

lua:
	mkdir -p build/lua/
	gcc -g -Lbuild/ src/lua/Cyperus.c -shared -o build/lua/Cyperus.so -fPIC -lcyperus -llua -I/usr/include/lua5.4/
