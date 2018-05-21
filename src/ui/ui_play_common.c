#include "ui_play_common.h"

#include <cdk/cdk.h>

#include "ui_private.h"
#include "ui_utils.h"

/* ########################################################################## */
/* ########################################################################## */

//const struct coordinate C_UP    = {  0, -1 };
//const struct coordinate C_DOWN  = {  0,  1 };
//const struct coordinate C_RIGHT = {  1,  0 };
//const struct coordinate C_LEFT  = { -1,  0 };

struct coordinate LOOK_AROUND[4] = {
    {    1,  0  },
    {    0, -1  },
    {   -1,  0  },
    {    0,  1  }
};

/* ########################################################################## */
/* ########################################################################## */

chtype  ui_grid_getChar(TUiContext argContext, int argX, int argY)
{
    unsigned char   lFlags  = 0x00;
    chtype          retVal  = '?';


    int lSizeX  = grid_columnsCount( argContext->gridData );
    int lSizeY  = grid_rowsCount( argContext->gridData );

    TEGridCellType  lCellType
            = grid_getCell( argContext->gridData, argY, argX );

    if( lCellType == EGridCellEmpty )
    {
        retVal  = ' ';
    }
    else if( lCellType == EGridCellEmptyVisited )
    {
        retVal  = ACS_BULLET;
    }
    else if( lCellType == EGridCellExit )
    {
        retVal  = ACS_DIAMOND;
    }
    else if( lCellType == EGridCellWall )
    {
        /*
         *  Check presence of walls around this cell
         */
        /* right side */
        if(     (argX + 1 < lSizeX)
            &&  grid_getCell( argContext->gridData, argY, argX+1 )
                == EGridCellWall )
        {
            lFlags  |= C_PLAY_FLAG_WALL_RIGHT;
        }

        /* up side */
        if(     (argY - 1 >= 0)
            &&  grid_getCell( argContext->gridData, argY-1, argX )
                == EGridCellWall )
        {
            lFlags  |= C_PLAY_FLAG_WALL_UP;
        }

        /* left side */
        if(     (argX - 1 >= 0)
            &&  grid_getCell( argContext->gridData, argY, argX-1 )
                == EGridCellWall )
        {
            lFlags  |= C_PLAY_FLAG_WALL_LEFT;
        }

        /* down side */
        if(     (argY + 1 < lSizeY)
            &&  grid_getCell( argContext->gridData, argY+1, argX )
                == EGridCellWall )
        {
            lFlags  |= C_PLAY_FLAG_WALL_DOWN;
        }


        switch( lFlags )
        {
            case    C_PLAY_FLAG_WALL_LEFT:
            case    C_PLAY_FLAG_WALL_RIGHT:
            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_LEFT):
    //            retVal  = 0xCD;
                retVal  = ACS_HLINE;
                break;

            case    C_PLAY_FLAG_WALL_UP:
            case    C_PLAY_FLAG_WALL_DOWN:
            case    (C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xBA;
                retVal  = ACS_VLINE;
                break;

            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_UP):
    //            retVal  = 0xC8;
                retVal  = ACS_LLCORNER;
                break;

            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xC9;
                retVal  = ACS_ULCORNER;
                break;
                break;

            case    (C_PLAY_FLAG_WALL_LEFT|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xC9;
                retVal  = ACS_URCORNER;
                break;

            case    (C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_LEFT):
    //            retVal  = 0xBC;
                retVal  = ACS_LRCORNER;
                break;

            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_LEFT):
    //            retVal  = 0xCA;
                retVal  = ACS_BTEE;
                break;

            case    (C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_LEFT|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xB9;
                retVal  = ACS_RTEE;
                break;

            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_LEFT|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xCB;
                retVal  = ACS_TTEE;
                break;

            case    (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xCC;
                retVal  = ACS_LTEE;
                break;

            case (C_PLAY_FLAG_WALL_RIGHT|C_PLAY_FLAG_WALL_UP|C_PLAY_FLAG_WALL_LEFT|C_PLAY_FLAG_WALL_DOWN):
    //            retVal  = 0xCE;
                retVal  = ACS_PLUS;
                break;
        }
    }


    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_play_drawGrid(TUiContext argContext)
{
    TEGridCellType  lCellType   = EGridCellUnknown;
    int lGridCols   = grid_columnsCount( argContext->gridData );
    int lGridRows   = grid_rowsCount( argContext->gridData );


    for( int lRow = 0 ; lRow < lGridRows ; lRow++ )
    {
        for( int lCol = 0 ; lCol < lGridCols ; lCol++ )
        {
            lCellType   = grid_getCell( argContext->gridData,
                                        lRow, lCol );
            int lPosX   = lCol  + argContext->gridOffsetX;
            int lPosY   = lRow  + argContext->gridOffsetY;


            move( lPosY, lPosX );
            if( lCellType == EGridCellExit )
            {
                attron( COLOR_PAIR(NCURSES_STYLE_COLORID_MAZE_EXIT) );
            }
            else if( lCellType == EGridCellEmptyVisited )
            {
                attron( COLOR_PAIR(NCURSES_STYLE_COLORID_MAZE_VISITED) );
            }
            else
            {
                attron( COLOR_PAIR( NCURSES_STYLE_COLORID_NORMAL ) );
            }
            addch( ui_grid_getChar( argContext, lCol, lRow ) );
        }
    }

    /* Draw player position */
    move( argContext->playerPos.y + argContext->gridOffsetY,
          argContext->playerPos.x + argContext->gridOffsetX );
    attron( COLOR_PAIR(NCURSES_STYLE_COLORID_MAZE_PLAYER) );
    addch( 'P' );
//    addch( ACS_BULLET );
    move( argContext->playerPos.y + argContext->gridOffsetY,
          argContext->playerPos.x + argContext->gridOffsetX );
    attron( COLOR_PAIR(NCURSES_STYLE_COLORID_NORMAL) );

    refresh();
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_play_movePlayer(TUiContext argContext)
{
    int lCharIn         = getch();
    int lProcessVisited = 0;
    int lOldPosX        = argContext->playerPos.x;
    int lOldPosY        = argContext->playerPos.y;



    switch( lCharIn )
    {
        case KEY_DOWN:
        case 's':
        case 'S':
        {
            if(         argContext->playerPos.y+1
                    <   (int)grid_rowsCount(argContext->gridData)
                &&  grid_getCell( argContext->gridData,
                                  argContext->playerPos.y+1,
                                  argContext->playerPos.x )
                    !=  EGridCellWall )
            {
                argContext->playerPos.y++;
                lProcessVisited++;
            }
            break;
        }


        case KEY_UP:
        case 'z':
        case 'Z':
        {
            if(         argContext->playerPos.y-1 > 0
                &&  grid_getCell( argContext->gridData,
                                  argContext->playerPos.y-1,
                                  argContext->playerPos.x )
                    !=  EGridCellWall )
            {
                argContext->playerPos.y--;
                lProcessVisited++;
            }
            break;
        }


        case KEY_LEFT:
        case 'q':
        case 'Q':
        {
            if(         argContext->playerPos.x-1 > 0
                &&  grid_getCell( argContext->gridData,
                                  argContext->playerPos.y,
                                  argContext->playerPos.x-1 )
                    !=  EGridCellWall )
            {
                argContext->playerPos.x--;
                lProcessVisited++;
            }
            break;
        }


        case KEY_RIGHT:
        case 'd':
        case 'D':
        {
            if(         argContext->playerPos.x+1
                    <   (int)grid_columnsCount(argContext->gridData)
                &&  grid_getCell( argContext->gridData,
                                  argContext->playerPos.y,
                                  argContext->playerPos.x+1 )
                    !=  EGridCellWall )
            {
                argContext->playerPos.x++;
                lProcessVisited++;
            }
            break;
        }
    }



    if( lProcessVisited != 0 )
    {

        if(     grid_getCell( argContext->gridData,
                              argContext->playerPos.y,
                              argContext->playerPos.x ) == EGridCellEmpty )
        {
            grid_setCell( argContext->gridData,
                          argContext->playerPos.y,
                          argContext->playerPos.x,
                          EGridCellEmptyVisited );
        }
        else
        {
            grid_setCell( argContext->gridData,
                          lOldPosY,
                          lOldPosX,
                          EGridCellEmpty );
        }
    }
}

/* ########################################################################## */
/* ########################################################################## */
