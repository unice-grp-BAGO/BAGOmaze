#ifndef UI_PRIVATE_H
#define UI_PRIVATE_H

#include <cdk/cdkscreen.h>
#include <ncurses.h>



typedef struct  SUiContext
{
    CDKSCREEN   *cdkscreen_p;
    WINDOW      *cursesWin;

}   TContext;

#endif  /*< UI_PRIVATE_H */

