#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "ui/ui.h"
#include "ui/ui_menuPrincipal.h"


/* ########################################################################## */
/* ########################################################################## */

int main(void)
{
    /* Force the locale due to usage of multibyte UTF-8 characters. */
    setlocale(LC_ALL, "");


    TUiContext lUiContext   = ui_initialize();

    ui_menuPrincipal_exec( lUiContext );

    ui_finalize( lUiContext );


    return EXIT_SUCCESS;
}

/* ########################################################################## */
/* ########################################################################## */
