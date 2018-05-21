#include "ui_play_random.h"

#include "core/grid.h"

#include "ui_utils.h"
#include "ui_private.h"
#include "ui_play_common.h"

/* ########################################################################## */
/* ########################################################################## */
#include "core/log/log.h"

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "UI", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "UI", format, ##__VA_ARGS__ );

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
    const int   c_maze_cols = 69;
    const int   c_maze_rows = 19;

    argContext->gridData    = grid_create( c_maze_rows, c_maze_cols );

    getmaxyx(stdscr, argContext->gridOffsetY, argContext->gridOffsetX);
    argContext->gridOffsetX -= c_maze_cols;
    argContext->gridOffsetX /= 2;
    argContext->gridOffsetY -= c_maze_rows;
    argContext->gridOffsetY /= 2;


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
    srand( time( NULL ) );
    s_maze_DFS( argContext, lGridVisited,
                argContext->playerPos.x, argContext->playerPos.y );


    /* Definition du point de sortie */
    grid_setCell( argContext->gridData,
                  grid_rowsCount(argContext->gridData)-2,
                  grid_columnsCount(argContext->gridData)-2,
                  EGridCellExit );


    /* Liberation des ressources locales */
    grid_destroy( &lGridVisited );
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_play_random(TUiContext argContext)
{
//    ui_clearScreen( argContext->cdkscreen_p );
    clear();

    argContext->playerPos.x = 1;
    argContext->playerPos.y = 1;
    s_maze_generate( argContext );

#define D_DEBUG
#ifdef  D_DEBUG
    /* Print the grid in a file */
    TRACE_DBG( "Printing grid in a debug file..." );
    FILE* p_file    = fopen( "grid_dbg.csv", "w" );
    grid_print( argContext->gridData, p_file);
    fclose( p_file );
#endif



    while(      argContext->playerPos.x
            !=  (int)grid_columnsCount(argContext->gridData)-2
        ||      argContext->playerPos.y
            !=  (int)grid_rowsCount(argContext->gridData)-2 )
    {
        ui_play_drawGrid( argContext );
        ui_play_movePlayer( argContext );
    }

    /* Release resources */
    grid_destroy( &(argContext->gridData) );
    clear();
}

/* ########################################################################## */
/* ########################################################################## */
