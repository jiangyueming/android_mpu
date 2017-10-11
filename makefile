mpu: main.o uart.o mpu_event.o
	cc -o mpu main.o uart.o mpu_event.o -lpthread
main.o : main.c uart.h mpu.h
	cc -c -g main.c

uart.o: uart.c uart.h
	cc -c -g uart.c

mpu_event.o: mpu_event.c mpu.h
	cc -c -g mpu_event.c