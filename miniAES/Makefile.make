exec: test_AES128.o AES128.o
	gcc -o exec test_AES128.o AES128.o

test_AES128.o: test_AES128.c AES128.h
	gcc -c test_AES128.c

AES128.o: AES128.c AES128.h
	gcc -c AES128.c

clean:
	rm test test_AES128.o AES128.o