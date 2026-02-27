#include "inc/port.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	list_ports();
	struct sp_port *klug_port = find_klug(NULL);
	if(!klug_port || !open_klug(klug_port))
		return -1;
	const char *ping_rsp = ping_klug(klug_port);
	if(!ping_rsp)
		return -1;
	printf("PING -> %s\n", ping_rsp);
	free((void*) ping_rsp);
	close_klug(klug_port);
}