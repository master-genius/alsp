
project = setmod.o mre.o

setmod : $(project)
	gcc -o setmod $(project)

setmod.o : setmod.c mre.h
	gcc -c setmod.c

mre.o : mre.c mre.h
	gcc -c mre.c

clean:
	rm setmod *.o

