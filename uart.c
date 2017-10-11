/********************************
* Module:       UART            *
* Author:       Josh Chen       *
* Date:         2016/03/23      *
********************************/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#include "uart.h"

int uart_open(uart *uart)
{
	struct termios cfg;
	speed_t speed;

	uart->uart_fd = open(uart->uart_path, O_RDWR);
	if (uart->uart_fd < 0)
		return -4;

	switch (uart->baud_rate) {
	case 1200:
		speed = B1200;		break;
	case 2400:
		speed = B2400;		break;
	case 4800:
		speed = B4800;		break;
	case 9600:
		speed = B9600;		break;
	case 19200:
		speed = B19200;	break;
	case 38400:
		speed = B38400;	break;
	case 57600:
		speed = B57600;	break;
	case 115200:
		speed = B115200;	break;
	default:
		close(uart->uart_fd);
		return -1;
	}

	if (tcgetattr(uart->uart_fd, &cfg))
	{
		printf("tcgetattr() failed");
		close(uart->uart_fd);
		/* TODO: throw an exception */
		return -2;
	}

	cfmakeraw(&cfg);
	cfsetispeed(&cfg, speed);
	cfsetospeed(&cfg, speed);
	//cfg.c_cflag &= ~CSIZE; /*mask the character size bits*/
	//cfg.c_cflag |= ICANON;
	//cfg.c_cflag |= CSTOPB;
	//cfg.c_cflag |= CLOCAL | CREAD;
	//cfg.c_cflag &= ~PARENB;
	//cfg.c_cc[VTIME] =0;
	//cfg.c_cc[VMIN]=0;

	//cfg.c_iflag |= IGNBRK;

	if (tcsetattr(uart->uart_fd, TCSANOW, &cfg))
	{
		printf("tcsetattr() failed");
		close(uart->uart_fd);
		/* TODO: throw an exception */
		return -3;
	}

	return 0;
}

int uart_close(uart *uart)
{
	if (uart->uart_fd)
	{
		close(uart->uart_fd);
		uart->uart_fd = 0;
	}
	return 1;
}

int uart_send(uart *uart, char *data, unsigned int data_len)
{
	int send_len = 0;
	if (uart->uart_fd == 0)
		return 0;

	send_len = write(uart->uart_fd, data, data_len);
	if (send_len != data_len)
		return send_len;

	return send_len;
}
