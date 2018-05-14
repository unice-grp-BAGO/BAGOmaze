#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <menu.h>
#include <cdk/cdk.h>
#include <cdk/dialog.h>

#include "ui_menu.h"

#define CDK_STYLEALIGN_CENTERED     "<C>"
#define CDK_STYLEALIGN_LEFT         "<L>"

#define CDK_STYLECOLOR_GREEN        "</24>"
#define CDK_STYLECOLOR_RED          "</16>"
#define CDK_STYLECOLOR_YELLOW       "</32>"

#define CDK_STYLEPOLICE_BOLD        "</B>"
#define CDK_STYLEPOLICE_UNDERLINE   "</U>"


#define CDK_STYLE_TITLE1                    \
                CDK_STYLEALIGN_CENTERED     \
                CDK_STYLECOLOR_RED          \
                CDK_STYLEPOLICE_BOLD        \
                CDK_STYLEPOLICE_UNDERLINE

#define CDK_STYLE_TITLE2                    \
                CDK_STYLEALIGN_CENTERED     \
                CDK_STYLECOLOR_RED          \
                CDK_STYLEPOLICE_BOLD

#define CDK_STYLE_EMPHASIS1                 \
                CDK_STYLEALIGN_CENTERED     \
                CDK_STYLECOLOR_YELLOW       \
                CDK_STYLEPOLICE_BOLD


#define	NCURSES_STYLE_COLORID_TITLE1    (1)

#define NCURSES_STYLE_COLORID_MAZE_EXIT (2)
#define	NCURSES_STYLE_COLORID_NORMAL    (3)





void        ui_ncursesHold(void);
void        ui_ncursesRestore(void);


void        ui_clearScreen( CDKSCREEN* argScreenToClear );

void        ui_printInMiddle( WINDOW    *win,
                              int       starty,     int startx,
                              int       width,
                              const char *string,
                              chtype color,         int argBold );

void        ui_popupBlocking( CDKSCREEN*    argScreen_p,
                              CDK_CSTRING2  argMessage,
                              int           argCount );

void        ui_popupBlockingTimeout( CDKSCREEN*         argScreen_p,
                                     CDK_CSTRING2       argMessage,
                                     int                argCount,
                                     int                argTimeout_s );

CDKDIALOG*  ui_popupNonBlocking( CDKSCREEN              *argScreen_p,
                                 CDK_CSTRING2           argMessage,
                                 int                    argCount );



#endif  /*< UI_UTILS_H */
