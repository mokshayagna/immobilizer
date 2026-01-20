a.out: main.o astrlen.o astrcmp.o astrncmp.o astrcpy.o astrncpy.o astrcat.o astrncat.o astr_test.o
	gcc -o a.out main.o astrlen.o astrcmp.o astrncmp.o astrcpy.o astrncpy.o astrcat.o astrncat.o astr_test.o

main.o: main.c astring.h astr_test.h
	gcc -c main.c

astrlen.o: astrlen.c astring.h
	gcc -c astrlen.c

astrcmp.o: astrcmp.c astring.h
	gcc -c astrcmp.c

astrncmp.o: astrncmp.c astring.h
	gcc -c astrncmp.c

astrcpy.o: astrcpy.c astring.h
	gcc -c astrcpy.c

astrncpy.o: astrncpy.c astring.h
	gcc -c astrncpy.c

astrcat.o: astrcat.c astring.h
	gcc -c astrcat.c

astrncat.o: astrncat.c astring.h
	gcc -c astrncat.c

astr_test.o: astr_test.c astr_test.h astring.h
	gcc -c astr_test.c

clean:
	rm -f *.o a.out

.PHONY: clean
