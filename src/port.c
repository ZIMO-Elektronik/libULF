#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "../inc/port.h"

int list_ports()
{
	struct sp_port **port_list;
	enum sp_return result = sp_list_ports(&port_list);
	if(result != SP_OK)
    {
		printf("sp_list_ports() failed!\n");
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