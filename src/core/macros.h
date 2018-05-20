#ifndef MACROS_H
#define MACROS_H

#include <malloc.h>


#define FREE(pointer)               \
        if( pointer != 0 )          \
        {                           \
            free((void*)pointer);   \
            pointer = 0;            \
        }

#endif
