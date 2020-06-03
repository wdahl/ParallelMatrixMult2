run: Homework_2_serial.out Homework_2_parallel.out generateSquareMatrix.out

Homework_2_parallel.out: Homework_2_parallel.c timing.c 
	gcc -g Homework_2_parallel.c timing.c -o Homework_2_parallel.out -lpthread

Homework_2_serial.out: Homework_2_serial.c timing.c
	gcc -g Homework_2_serial.c timing.c -o Homework_2_serial.out

generateSquareMatrix.out: generateSquareMatrix.c
	gcc generateSquareMatrix.c -o generateSquareMatrix.out

clean:
	rm *.out