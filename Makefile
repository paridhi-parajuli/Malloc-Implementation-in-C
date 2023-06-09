remove_exe:
	rm main
build_library:
	gcc -o mymalloc.o -c mymalloc.c
	ar -rcs mymalloc.a mymalloc.o

compile:
	gcc -o main main.c mymalloc.a

execute:
	./main
