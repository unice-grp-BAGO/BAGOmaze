#ifndef UI_PRIVATE_H
#define UI_PRIVATE_H

#include <cdk/cdkscreen.h>
#include <ncurses.h>

#include "core/grid.h"



typedef struct  SUiContext
{
    CDKSCREEN   *cdkscreen_p;
    WINDOW      *cursesWin;

    TCoreGrid   gridData;

    size_t      gridOffsetX;
    size_t      gridOffsetY;

    struct
    {
        int x;
        int y;

    }   playerPos;

}   TContext;

#endif  /*< UI_PRIVATE_H */

