.PHONY : link clean

link : linker t.o
	@./linker

t : t.o
	ld $< -e main -static -s -o $@

t.o : t.c
	gcc $< -c -fno-builtin -fno-asynchronous-unwind-tables -Werror -o $@

clean :
	-rm t.o t