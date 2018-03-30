#include <stdio.h>
#include <stdlib.h>

#include "ui/ui.h"
#include "ui/ui_menuPrincipal.h"


/* ########################################################################## */
/* ########################################################################## */

int main(void)
{
    TUiContext lUiContext   = ui_initialize();

    ui_menuPrincipal_exec( lUiContext );

    ui_finalize( lUiContext );


    return EXIT_SUCCESS;
}

/* ########################################################################## */
/* ########################################################################## */
