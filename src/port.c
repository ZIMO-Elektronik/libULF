#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../inc/port.h"

int list_ports()
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);
	if(result != SP_OK)
	{
		fprintf(stderr, "sp_list_ports() failed!\n");
		return -1;
	}
	bool klug_found = false;
	for(int i = 0; port_list[i] != NULL; i++)
	{
		struct sp_port *port = port_list[i];
		char *port_name = sp_get_port_name(port);
		char *port_desc = sp_get_port_description(port);
		char *port_manu = sp_get_port_usb_manufacturer(port);
		printf("Found port: %s = %s %s\n", port_name, port_manu, port_desc);
		if(port_manu && port_desc)
			klug_found |= (!strcmp(port_manu, "ZIMO Elektronik") && !strncmp(port_desc, "KLUG ", 5));
	}
	if(klug_found)
		printf("ZIMO KLUG is indeed connected, I love it!\n");
	else
		printf("Very sorry to tell you we didn't find a KLUG :*(\n");
	sp_free_port_list(port_list);
	/* Note that this will also free all the sp_port structures
		* it points to. If you want to keep one of them (e.g. to
		* use that port in the rest of your program), take a copy
		* of it first using sp_copy_port(). */
	return 0;
}

struct sp_port* find_klug(char *serial_number)
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);
	if(result != SP_OK)
	{
		fprintf(stderr, "sp_list_ports() failed!\n");
		return NULL;
	}
	bool klug_found = false;
	struct sp_port *klug_port = NULL;
	for(int i = 0; port_list[i] != NULL; i++)
	{
		struct sp_port *port = port_list[i];
		char *port_name = sp_get_port_name(port);
		char *port_desc = sp_get_port_description(port);
		char *port_manu = sp_get_port_usb_manufacturer(port);
		printf("Found port: %s = %s %s\n", port_name, port_manu, port_desc);
		if(port_manu && port_desc)
			klug_found |= (!strcmp(port_manu, "ZIMO Elektronik") && !strncmp(port_desc, "KLUG ", 5));
		if(klug_found && serial_number) {
			if(strlen(port_desc) > 7)
				klug_found &= !strcmp(&port_desc[7], serial_number);
			else
			 	klug_found = false;
		}
		if(klug_found) {
			if(check(sp_copy_port(port, &klug_port)) != SP_OK)
				klug_port = NULL;
			break;
		}
	}
	sp_free_port_list(port_list);
	if(!klug_found || !klug_port) {
		printf("Very sorry to tell you we didn't find a KLUG :*(\n");
		return NULL;
	}
	printf("ZIMO KLUG is indeed connected, I love it!\n");
	return klug_port;
}

bool open_klug(struct sp_port *klug_port)
{
	check(sp_open(klug_port, SP_MODE_READ_WRITE));
	printf("Setting port to 115200 8N1, no flow control.\n");
	check(sp_set_baudrate(klug_port, 115200));
	check(sp_set_bits(klug_port, 8));
	check(sp_set_parity(klug_port, SP_PARITY_NONE));
	check(sp_set_stopbits(klug_port, 1));
	check(sp_set_flowcontrol(klug_port, SP_FLOWCONTROL_NONE));
	return true;
}

bool close_klug(struct sp_port *klug_port)
{
	check(sp_close(klug_port));
	return true;
}

const char* ping_klug(struct sp_port *klug_port)
{
	if(!klug_port) {
		fprintf(stderr, "Won't ping KLUG at port NULL.\n");
		return NULL;
	}
	printf("ping_klug @ %s\n", sp_get_port_name(klug_port));
	const char *txbuf = "PING\r";
	int n = check(sp_blocking_write(klug_port, txbuf, 5, 0));
	printf("ping_klug: sent %d bytes\n", n);
	if(n != 5)
		return NULL;
	char *rxbuf = (char*) calloc(33, 1);
	n = check(sp_blocking_read(klug_port, rxbuf, 32, 100));
	printf("ping_klug: received %d bytes\n", n);
	return rxbuf;
}

int check(enum sp_return result)
{
	/* For this example we'll just exit on any error by calling abort(). */
	char *error_message;
	switch (result)
	{
		case SP_ERR_ARG:
			fprintf(stderr, "Error: Invalid argument.\n");
			abort();
		case SP_ERR_FAIL:
			error_message = sp_last_error_message();
			fprintf(stderr, "Error: Failed: %s\n", error_message);
			sp_free_error_message(error_message);
			abort();
		case SP_ERR_SUPP:
			fprintf(stderr, "Error: Not supported.\n");
			abort();
		case SP_ERR_MEM:
			fprintf(stderr, "Error: Couldn't allocate memory.\n");
			abort();
		case SP_OK:
		default:
			return result;
	}
}