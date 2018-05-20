#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/* ########################################################################## */
/* ########################################################################## */

const char*     C_FILENAME          = "traces.log";
static  FILE*   s_fileDescriptor    = NULL;

/* ########################################################################## */
/* ########################################################################## */

int log_begin(void)
{
    int retVal  = EXIT_SUCCESS;


    if( s_fileDescriptor != NULL )
    {
        /* File is already open ! */
        retVal  = EXIT_FAILURE;
    }
    else if( (s_fileDescriptor=fopen( C_FILENAME, "a" )) == NULL )
    {
        perror( "Error while opening logfile :: " );
        retVal  = EXIT_FAILURE;
    }
    else
    {
        /* File opened successfully */
    }


    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

void    log_end(void)
{
    if( s_fileDescriptor == NULL )
    {
        /* Trying to close a not opened file ! */
    }
    else
    {
        fflush( s_fileDescriptor );
        fclose( s_fileDescriptor );
    }
}

/* ########################################################################## */
/* ########################################################################## */
/**
 * @brief log_debug
 * @param argFormat
 *
 *  @note Taken from [this answer](https://stackoverflow.com/a/1056442/1303262)
 *          at stackoverflow.com.
 */
void    log_debug(const char *argFormat,...)
{
    if( s_fileDescriptor != NULL )
    {
        va_list p_args;

        fprintf( s_fileDescriptor, "DBG: " );

        va_start( p_args, argFormat );
        vfprintf( s_fileDescriptor, argFormat, p_args );
        va_end( p_args );

        fflush( s_fileDescriptor );
    }
}

/* ########################################################################## */
/* ########################################################################## */
/**
 * @brief log_error
 * @param argFormat
 *
 *  @note Taken from [this answer](https://stackoverflow.com/a/1056442/1303262)
 *          at stackoverflow.com.
 */
void    log_error(const char *argFormat,...)
{
    if( s_fileDescriptor != NULL )
    {
        va_list p_args;

        fprintf( s_fileDescriptor, "ERR: " );

        va_start( p_args, argFormat );
        vfprintf( s_fileDescriptor, argFormat, p_args );
        va_end( p_args );

        fflush( s_fileDescriptor );
    }
}

/* ########################################################################## */
/* ########################################################################## */
