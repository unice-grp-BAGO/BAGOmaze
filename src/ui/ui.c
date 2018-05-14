#include "ui.h"

#include "ui_utils.h"
#include "ui_private.h"




/* ########################################################################## */
/* ########################################################################## */

void    ui_finalize(TUiContext argContext)
{
    if( argContext->cdkscreen_p != (CDKSCREEN*)0 )
    {
        destroyCDKScreen( argContext->cdkscreen_p );
    }

    endCDK();
    endwin();
    free( argContext );
}

/* ########################################################################## */
/* ########################################################################## */

TUiContext  ui_initialize(void)
{
    TUiContext  retVal  = malloc( sizeof( struct SUiContext ) );


    retVal->gridData    = NULL;
    retVal->gridOffsetX = 0;
    retVal->gridOffsetY = 0;


    /* Set up CDK. */
    retVal->cursesWin   = initscr();
    retVal->cdkscreen_p = initCDKScreen( retVal->cursesWin );


    /* Start color. */
    start_color();
    initCDKColor();
    init_pair( NCURSES_STYLE_COLORID_NORMAL,    COLOR_WHITE,COLOR_BLACK );
    init_pair( NCURSES_STYLE_COLORID_TITLE1,    COLOR_RED,  COLOR_BLACK );
    init_pair( NCURSES_STYLE_COLORID_MAZE_EXIT, COLOR_BLACK,COLOR_GREEN );


    /* Pause the ncurses mode while we don't need it */
    ui_ncursesHold();


    return retVal;
}
