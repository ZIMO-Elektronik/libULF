// #include "inc/port.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <optional>
#include <cstdint>

#include "src/new_port.hpp"

int main(int argc, char **argv)
{
	init();
	if (!open_klug())
	{
		libusb_exit(nullptr); return -1;
	}
	ping_klug();
	close_klug();
}