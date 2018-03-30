#include "ui_utils.h"

#include <locale.h>

#include <cdk/cdk.h>
#include <cdk/cdk_util.h>
#include <cdk/entry.h>
#include <cdk/scroll.h>
#include <cdk/fselect.h>

#include "core/log/log.h"

/* ########################################################################## */
/* ########################################################################## */

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "UI", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "UI", format, ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */
/*
 *  Constants definitions
 */


/* ########################################################################## */
/* ########################################################################## */

void    ui_clearScreen(CDKSCREEN *argScreenToClear)
{
    int rows    = 0;
    int cols    = 0;
    int lTmp    = 0;

    getyx( argScreenToClear->window, rows, cols );

    while (rows > 0 )
    {
        for( lTmp = 0 ; lTmp < cols ; lTmp++ )
        {
            mvprintw( rows, lTmp, " " );
        }

        rows--;
    }

    refreshCDKScreen( argScreenToClear );
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_ncursesHold(void)
{
    def_prog_mode();    /*< Save the tty modes          */
    endwin();           /*< End curses mode temporarily */
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_ncursesRestore(void)
{
    reset_prog_mode();  /* Return to the previous tty mode  */
                        /* stored by def_prog_mode()        */
    refresh();          /* Do refresh() to restore the      */
                        /* Screen contents                  */
}

/* ########################################################################## */
/* ########################################################################## */

void        ui_popupBlocking( CDKSCREEN*    argScreen_p,
                              CDK_CSTRING2  argMessage,
                              int           argCount )
{
    popupLabel( argScreen_p, argMessage, argCount );
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_popupBlockingTimeout( CDKSCREEN*     argScreen_p,
                                 CDK_CSTRING2   argMessage,
                                 int            argCount,
                                 int            argTimeout_s )
{
    CDKDIALOG   *p_dialog   = ui_popupNonBlocking( argScreen_p,
                                                   argMessage,
                                                   argCount );

    sleep( argTimeout_s );

    destroyCDKDialog( p_dialog );
}

/* ########################################################################## */
/* ########################################################################## */

CDKDIALOG*  ui_popupNonBlocking( CDKSCREEN*     argScreen_p,
                                 CDK_CSTRING2   argMessage,
                                 int            argCount )
{
    const char *buttons[]   = {"", ""};
    CDKDIALOG   *p_dialog;


    p_dialog = newCDKDialog( argScreen_p,
                             CENTER,        /*< X */
                             CENTER,        /*< Y */
                             (CDK_CSTRING2) argMessage, argCount,   /*< Text */
                             (CDK_CSTRING2) buttons, 1,     /*< Buttons */
                             COLOR_PAIR (2) | A_REVERSE,    /*< highlight */
                             FALSE,         /*< Separator */
                             TRUE,          /*< Box */
                             TRUE );        /*< Shadow */

    drawCDKDialog( p_dialog, TRUE );

    return p_dialog;
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_printInMiddle( WINDOW        *win,
                          int           starty,     int     startx,
                          int           width,
                          const char    *string,
                          chtype        color ,     int     argBold )
{
    int length, x, y;
    float temp;

    if(win == NULL)
    {
        win = stdscr;
    }

    getyx(win, y, x);

    if(startx != 0)
    {
        x   = startx;
    }

    if(starty != 0)
    {
        y   = starty;
    }

    if(width == 0)
    {
        width   = 80;
    }

    length  = strlen(string);
    temp    	= (width - length)/ 2;
    x       = startx + (int)temp;

    if( argBold == TRUE )
    {
        wattron( win, A_BOLD );
    }
    wattron( win, color );
    mvwprintw( win, y, x, "%s", string );
    wattroff( win, color );
    if( argBold == TRUE )
    {
        wattroff( win, A_BOLD );
    }
    refresh();
}

/* ########################################################################## */
/* ########################################################################## */
