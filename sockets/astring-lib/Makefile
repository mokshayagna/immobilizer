a.out: main.o astrcmp.o astrlen.o astr_test.o
	gcc -o a.out main.o astrcmp.o astrlen.o astr_test.o

main.o: main.c astring.h
	gcc -c main.c

astrlen.o: astrlen.c astring.h
	gcc -c astrlen.c

astrcmp.o: astrcmp.c astring.h
	gcc -c astrcmp.c

astr_test.o: astr_test.c astr_test.h
	gcc -c astr_test.c

clean:
	rm -f *.o a.out
	
.PHONY: clean	
