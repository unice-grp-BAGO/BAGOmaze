#include "ui_play_random.h"

#include "core/grid.h"

#include "ui_utils.h"
#include "ui_private.h"

/* ########################################################################## */
/* ########################################################################## */

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "UI", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "UI", format, ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */

#define c_flag_wallDown     (0x08)
#define c_flag_wallLeft     (0x04)
#define c_flag_wallRight    (0x01)
#define c_flag_wallUp       (0x02)


struct coordinate
{
    int x;
    int y;
};

const struct coordinate C_UP    = {  0, -1 };
const struct coordinate C_DOWN  = {  0,  1 };
const struct coordinate C_RIGHT = {  1,  0 };
const struct coordinate C_LEFT  = { -1,  0 };

struct coordinate LOOK_AROUND[4] = {
    {    1,  0  },
    {    0, -1  },
    {   -1,  0  },
    {    0,  1  }
};

/* ########################################################################## */
/* ########################################################################## */
/**
 *  @brief  Maze generation using depth-first search
 */
static void s_maze_DFS( TUiContext  argContext,
                        TCoreGrid   argGridVisited,
                        int         argX,
                        int         argY )
{
    /* Si la cellule courante a été visitée, aucun intérêt à continuer. */
    if( grid_getCell( argGridVisited, argY, argX ) != EGridCellUnknown )
    {
        return;
    }

    /*
     *  On indique que la cellule courante a été visitée, et que c'est une case
     * vide.
     */
//    visited[x][y]   = TRUE;
//    game_area[x][y] = NOTHING;
    grid_setCell( argGridVisited,       argY, argX, EGridCellEmpty );
    grid_setCell( argContext->gridData, argY, argX, EGridCellEmpty );


    char unvisited_neighbour_counter;
    int lGridColumnsCount   = grid_columnsCount( argContext->gridData );
    int lGridRowsCount      = grid_rowsCount( argContext->gridData );
    do
    {
        /*
         * Pour chaque direction de deplacement possible a partir de la cellule
         * actuelle, on regarde si la cellule de destination a été visitée ou
         * pas. */
        unvisited_neighbour_counter = 0;
        int i;
        for (i = 0; i < 4; i++)
        {
            /* Calcul des coordonées de la cellule de destination pour un
             * déplacement dans la direction actuellement testée :
             * Le tableau \ref LOOK_AROUND contient des facteurs multiplicateurs
             * pour les axes X et Y pour chaque orientation à visiter ;
             * pour chaque orientation à visiter, on les multiplie par le nombre
             * de cases à sauter. Cela permet d'utiliser le même code pour
             * toutes les directions. */
            int _x = argX + (2 * LOOK_AROUND[i].x);
            int _y = argY + (2 * LOOK_AROUND[i].y);


            /* On regarde si la position de destination calculée est valide
             * (elle est située dans la grille et elle n'a pas encore été
             * visitée). */
            if(     lGridRowsCount      >   _y
                &&  _y                  >   0
                &&  lGridColumnsCount   >   _x
                &&  _x                  >   0
                &&  grid_getCell(argGridVisited, _y, _x )
                    == EGridCellUnknown )
            {
                /* Si la cellule de destination n'a pas encore été visitée,
                 * incrémenter le compteur. */
                unvisited_neighbour_counter++;
            }
        }


        /* Si il y a des cases non visitées au voisinage de la case courante, */
        if (unvisited_neighbour_counter > 0)
        {
            int internal_counter = -1;

            /* Cette variable va servir d'index pour connaitre la position vers
             * laquelle se déplacer (qui est différent de l'index de la zone non
             * visitée !) */
            int j = -1;

            /* On choisit une des zones non visitées vers laquelle se déplacer :
             * D'abord on choisit de manière aléatoire la position de celle à
             * visiter, puis on boucle pareil que précédemment pour trouver les
             * zones non visitées ; A chaque fois que l'on tombe sur une zone
             * non visitee, on incrémente le compteur internal_counter ; quand
             * on a atteint la zone choisie, on stoppe la boucle. */
            char choice = rand () % (unvisited_neighbour_counter + 1);
            do
            {
                int _x = argX + (2 * LOOK_AROUND[j].x);
                int _y = argY + (2 * LOOK_AROUND[j].y);
                if(     lGridRowsCount          > _y
                        &&  _y                  > 0
                        &&  lGridColumnsCount   > _x
                        &&  _x                  > 0
                        &&  grid_getCell(argGridVisited, _y, _x )
                            == EGridCellUnknown )
                {
                    internal_counter++;
                }
                j++;

            }
            while (internal_counter != choice - 1);
            j--;


            /* On "casse le mur" entre la cellule courante et la cellule choisie
             * (par défaut, elle contient un mur). */
//            game_area[x + LOOK_AROUND[j].x][y + LOOK_AROUND[j].y] = NOTHING;
            grid_setCell( argContext->gridData,
                          argY + LOOK_AROUND[j].y,
                          argX + LOOK_AROUND[j].x,
                          EGridCellEmpty );

            /* On fait un appel recursif a la fonction DFS, cette fois-ci avec
                 * les coordonnées de la cellule choisie. */
            s_maze_DFS( argContext, argGridVisited,
                        argX + (2 * LOOK_AROUND[j].x),
                        argY + (2 * LOOK_AROUND[j].y) );
        }
    }
    while (unvisited_neighbour_counter > 0);
}

/* ########################################################################## */
/* ########################################################################## */
/**
 *  @brief  Maze generation using depth-first search
 */
static void s_maze_generate(TUiContext argContext)
{
    argContext->gridData    = grid_create( 19, 39 );


    /*
     *  Creation de la grille temporaire permettant de connaitre les
     *  emplacements deja visités. */
    TCoreGrid   lGridVisited
            = grid_create( grid_rowsCount( argContext->gridData ),
                           grid_columnsCount( argContext->gridData ) );

    TEGridCellType  lCellType       = EGridCellUnknown;
    int             lColumnsCount   = grid_columnsCount( lGridVisited );
    int             lRowsCount      = grid_rowsCount( lGridVisited );
    for( int lRow = 0 ; lRow < lRowsCount ; lRow++ )
    {
        for( int lCol = 0 ; lCol < lColumnsCount ; lCol++ )
        {
            if(     lCol    == 0
                ||  lCol    == lColumnsCount-1
                ||  lRow    == 0
                ||  lRow    == lRowsCount-1 )
            {
                lCellType   = EGridCellWall;
            }
            else
            {
                lCellType   = EGridCellUnknown;
            }

            grid_setCell( lGridVisited, lRow, lCol, lCellType );
        }
    }


    /* Generation du labyrinthe */
    s_maze_DFS( argContext, lGridVisited, 1, 1 );


    /* Definition du point de sortie */
    grid_setCell( argContext->gridData,
                  grid_rowsCount(argContext->gridData)-2,
                  grid_columnsCount(argContext->gridData)-2,
                  EGridCellExit );


    /* Liberation des ressources locales */
    grid_destroy( lGridVisited );
}

/* ########################################################################## */
/* ########################################################################## */
chtype  ui_grid_getChar(TUiContext argContext, int argX, int argY)
{
//    const unsigned char c_flag_wallDown     = 0x08;
//    const unsigned char c_flag_wallLeft     = 0x04;
//    const unsigned char c_flag_wallRight    = 0x01;
//    const unsigned char c_flag_wallUp       = 0x02;

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
            lFlags  |= c_flag_wallRight;
        }

        /* up side */
        if(     (argY - 1 >= 0)
            &&  grid_getCell( argContext->gridData, argY-1, argX )
                == EGridCellWall )
        {
            lFlags  |= c_flag_wallUp;
        }

        /* left side */
        if(     (argX - 1 >= 0)
            &&  grid_getCell( argContext->gridData, argY, argX-1 )
                == EGridCellWall )
        {
            lFlags  |= c_flag_wallLeft;
        }

        /* down side */
        if(     (argY + 1 < lSizeY)
            &&  grid_getCell( argContext->gridData, argY+1, argX )
                == EGridCellWall )
        {
            lFlags  |= c_flag_wallDown;
        }


        switch( lFlags )
        {
            case    c_flag_wallLeft:
            case    c_flag_wallRight:
            case    (c_flag_wallRight|c_flag_wallLeft):
    //            retVal  = 0xCD;
                retVal  = ACS_HLINE;
                break;

            case    c_flag_wallUp:
            case    c_flag_wallDown:
            case    (c_flag_wallUp|c_flag_wallDown):
    //            retVal  = 0xBA;
                retVal  = ACS_VLINE;
                break;

            case    (c_flag_wallRight|c_flag_wallUp):
    //            retVal  = 0xC8;
                retVal  = ACS_LLCORNER;
                break;

            case    (c_flag_wallRight|c_flag_wallDown):
    //            retVal  = 0xC9;
                retVal  = ACS_ULCORNER;
                break;
                break;

            case    (c_flag_wallLeft|c_flag_wallDown):
    //            retVal  = 0xC9;
                retVal  = ACS_URCORNER;
                break;

            case    (c_flag_wallUp|c_flag_wallLeft):
    //            retVal  = 0xBC;
                retVal  = ACS_LRCORNER;
                break;

            case    (c_flag_wallRight|c_flag_wallUp|c_flag_wallLeft):
    //            retVal  = 0xCA;
                retVal  = ACS_BTEE;
                break;

            case    (c_flag_wallUp|c_flag_wallLeft|c_flag_wallDown):
    //            retVal  = 0xB9;
                retVal  = ACS_RTEE;
                break;

            case    (c_flag_wallRight|c_flag_wallLeft|c_flag_wallDown):
    //            retVal  = 0xCB;
                retVal  = ACS_TTEE;
                break;

            case    (c_flag_wallRight|c_flag_wallUp|c_flag_wallDown):
    //            retVal  = 0xCC;
                retVal  = ACS_LTEE;
                break;

            case (c_flag_wallRight|c_flag_wallUp|c_flag_wallLeft|c_flag_wallDown):
    //            retVal  = 0xCE;
                retVal  = ACS_PLUS;
                break;
        }
    }


    return retVal;
}

void    ui_grid_draw(TUiContext argContext)
{
    TEGridCellType  lCellType   = EGridCellUnknown;
    int lGridCols   = grid_columnsCount( argContext->gridData );
    int lGridRows   = grid_rowsCount( argContext->gridData );
    int lOffsetX    = 0;
    int lOffsetY    = 0;

    getmaxyx(stdscr, lOffsetY, lOffsetX);
    lOffsetX    -= lGridCols;
    lOffsetX    /= 2;
    lOffsetY    -= lGridRows;
    lOffsetY    /= 2;

    for( int lRow = 0 ; lRow < lGridRows ; lRow++ )
    {
        for( int lCol = 0 ; lCol < lGridCols ; lCol++ )
        {
            lCellType   = grid_getCell( argContext->gridData,
                                        lRow, lCol );
            int lPosX   = lCol  + lOffsetX;
            int lPosY   = lRow  + lOffsetY;


            move( lPosY, lPosX );
            if( lCellType == EGridCellExit )
            {
                attron( COLOR_PAIR(NCURSES_STYLE_COLORID_MAZE_EXIT) );
            }
            else
            {
                attron( COLOR_PAIR( NCURSES_STYLE_COLORID_NORMAL ) );
            }
            addch( ui_grid_getChar( argContext, lCol, lRow ) );
        }
    }

    refresh();
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_play_random(TUiContext argContext)
{
//    ui_clearScreen( argContext->cdkscreen_p );
    clear();

    s_maze_generate( argContext );

    FILE* p_file    = fopen( "grid.csv", "w" );
    grid_print( argContext->gridData, p_file);
    fclose( p_file );


    ui_grid_draw( argContext );


    /* Wait for a user action */
    fflush( stdin );
    fgetc( stdin );


    /* Release resources */
    grid_destroy( argContext->gridData );
    clear();
}

/* ########################################################################## */
/* ########################################################################## */
