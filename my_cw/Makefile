all: main.o read_text.o without_repeating.o f_one.o f_two.o f_three.o f_four.o
	gcc main.o read_text.o without_repeating.o f_one.o f_two.o f_three.o f_four.o -o cw

main.o: main.c read_text.h without_repeating.h f_one.h f_two.h f_three.h f_four.h
	gcc -c main.c

read_text.o: read_text.c read_text.h
	gcc -c read_text.c

without_repeating.o: without_repeating.c without_repeating.h
	gcc -c without_repeating.c 

f_one.o: f_one.c f_one.h
	gcc -c f_one.c

f_two.o: f_two.c f_two.h
	gcc -c f_two.c

f_three.o: f_three.c f_three.h
	gcc -c f_three.c

f_four.o: f_four.c f_four.h
	gcc -c f_four.c