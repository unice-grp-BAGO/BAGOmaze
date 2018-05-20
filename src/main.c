#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/log/log.h"
#include "ui/ui.h"
#include "ui/ui_menuPrincipal.h"

/* ########################################################################## */
/* ########################################################################## */

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "main", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "main", format, ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */

int main(void)
{
    /* Force the locale due to usage of multibyte UTF-8 characters. */
    setlocale(LC_ALL, "");

#ifdef  D_DEBUG
    if( log_begin() != EXIT_SUCCESS )
    {
        fprintf( stderr, "An error occured while opening logfile !" );
    }
    else
    {
        TRACE_DBG( "##################################################" );
        TRACE_DBG( "Application starting." );
    }
#endif

    TUiContext lUiContext   = ui_initialize();

    ui_menuPrincipal_exec( lUiContext );

    ui_finalize( lUiContext );


#ifdef  D_DEBUG
    TRACE_DBG( "Application stop." );
    TRACE_DBG( "##################################################" );
    log_end();
#endif

    return EXIT_SUCCESS;
}

/* ########################################################################## */
/* ########################################################################## */
