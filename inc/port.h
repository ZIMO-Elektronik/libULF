#ifndef __PORT_H
#define __PORT_H

#include <libserialport.h>

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif


EXPORT int list_ports();

EXPORT struct sp_port* find_klug(char *serial_number);

EXPORT bool open_klug(struct sp_port *klug_port);

EXPORT bool close_klug(struct sp_port *klug_port);

EXPORT const char* ping_klug(struct sp_port *klug_port);

int check(enum sp_return result);


#ifdef __cplusplus
}
#endif

#endif //__PORT_H