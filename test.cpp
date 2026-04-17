//#include "inc/port.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include <optional>
#include <cstdint>

#include "src/new_port.hpp"

int main(int argc, char **argv)
{
	init();
	open_klug();
	ping_klug();
	close_klug();
}