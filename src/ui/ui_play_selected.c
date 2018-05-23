#include "ui_play_selected.h"

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

#define C_BUFFER_LENGTH (256)

const char*     C_FILEPATH_GRID_L1  = "rsrc/grid01.csv";
const char*     C_FILEPATH_GRID_L2  = "rsrc/grid02.csv";
const char*     C_FILEPATH_GRID_L3  = "rsrc/grid03.csv";
const char*     C_FILEPATH_GRID_L4  = "rsrc/grid04.csv";
const char*     C_FILEPATH_GRID_L5  = "rsrc/grid05.csv";

const int       C_GRID_MINIMUMSIZE  = 5;

/* ########################################################################## */
/* ########################################################################## */

static int  s_loadGrid(TUiContext argContext, const char *argFileName)
{
    FILE*   p_file  = NULL;
    int     retVal  = EXIT_SUCCESS;


    /*
     *  Vérification des donnees d'entree
     */
    if( argContext->gridData != NULL )
    {
        TRACE_ERR( "Can't load the grid : A grid already exist !" );
        retVal  = EXIT_FAILURE;
    }


    /*
     *  Tentative d'ouverture du fichier
     */
    else if( (p_file=fopen( argFileName, "r" )) == NULL )
    {
        /* Si une erreur survient, on stoppe le traitement */
        TRACE_ERR( "Can't load the grid : An error occured while opening the"
                   " file '%s' !",
                   argFileName );
        retVal  = EXIT_FAILURE;
    }


    /*
     *  Extraction des données du fichier
     */
    else
    {
        char    lBuffer[C_BUFFER_LENGTH]    = {0};


        /* ---------------------------------------------------------------------
         *  Premiere lecture du fichier :
         *  On cherche a connaitre la taille de la grille qu'il contient.
         */
        int     lColumnsCount   = 0;
        int     lRowsCount      = 0;
        char*   p_token         = 0;

        while( ! feof( p_file ) )
        {
            if(     fgets( lBuffer, C_BUFFER_LENGTH, p_file ) == NULL
                ||  strlen( lBuffer ) == 0 )
            {
                /* Can't process line, just continue */
                continue;
            }
            else
            {
                lRowsCount++;
            }



            int lColumnsCountTmp    = 0;
            p_token = strtok(lBuffer,";");
            while( p_token != NULL )
            {
                /* Incrementation du nombre de colonnes trouvees dans cette
                 * ligne */
                lColumnsCountTmp++;

                /* Passage au token suivant */
                p_token = strtok(NULL,";");
            }


            if( lColumnsCountTmp > lColumnsCount )
            {
                lColumnsCount   = lColumnsCountTmp;
            }
        }

        TRACE_DBG( "rows : %d", lRowsCount );
        TRACE_DBG( "columns : %d", lColumnsCount );


        /*
         *  Create a data grid with the right size
         */
        if(     lColumnsCount   < C_GRID_MINIMUMSIZE
            ||  lRowsCount      < C_GRID_MINIMUMSIZE )
        {
            /* Si la taille lue est inférieure à la taille minimum d'aire de
             * jeu, alors refuser de créer la grille */
            TRACE_ERR( "Grid will be too small ! (detected a maximum of %d rows"
                       " and %d columns).",
                       lRowsCount, lColumnsCount );
            retVal  = EXIT_FAILURE;
        }
        else
        {
            /* Recuperation de la taille de l'ecran */
            getmaxyx(stdscr, argContext->gridOffsetY, argContext->gridOffsetX);

            /* Calcul de la difference de taille entre l'ecran et la grille */
            argContext->gridOffsetX -= lColumnsCount;
            argContext->gridOffsetY -= lRowsCount;

            if(     argContext->gridOffsetY < 0
                ||  argContext->gridOffsetY < 0 )
            {
                TRACE_ERR( "Grid is too big !" );

                getmaxyx( stdscr,
                          argContext->gridOffsetY,
                          argContext->gridOffsetX );
                TRACE_DBG( "Grid max size %d rows, %d cols",
                           argContext->gridOffsetY,
                           argContext->gridOffsetX );
                TRACE_DBG( "Grid current size %d rows, %d cols",
                           lRowsCount,
                           lColumnsCount );
            }

            /* Division de la difference de taille par 2 */
            argContext->gridOffsetX /= 2;
            argContext->gridOffsetY /= 2;


            /*
             *  Instanciation de la grille
             */
            argContext->gridData    = grid_create( lRowsCount,
                                                   lColumnsCount );



            /* -----------------------------------------------------------------
             *  Deuxieme lecture :
             *  On remplit le contenu de la grille
             */
            lColumnsCount   = 0;
            lRowsCount      = 0;
            rewind( p_file );       /*< Retour au debut du fichier. */

            while( ! feof( p_file ) )
            {
                lColumnsCount   = 0;
                if(     fgets( lBuffer, C_BUFFER_LENGTH, p_file ) == NULL
                    ||  strlen( lBuffer ) == 0 )
                {
                    /* On verifie qu'on a bien réussi a lire une ligne et que la
                     * ligne n'est pas vide. */
                    /* Can't process line, just continue */
                    continue;
                }


                /*
                 *  Decoupage de la ligne
                 */
                p_token = strtok(lBuffer,";");
                while( p_token != NULL )
                {
                    /* On recupere le premier caractere de chaque "morceau" de
                     * ligne car un seul caractere nous interesse.
                     * Ensuite on convertit ce caractere en valeur d'enum. */
                    TEGridCellType lCellType
                            = grid_cellType_fromChar( p_token[ 0 ] );


                    /* On definit la valeur de la cellule correspondante dans
                     * la matrice. */
                    grid_setCell( argContext->gridData,
                                  lRowsCount,
                                  lColumnsCount,
                                  lCellType );


                    /* get next token */
                    lColumnsCount++;
                    p_token = strtok(NULL,";");
                }


                lRowsCount++; /*< Incrementation du numero de ligne. */
            }
        }
    }


    /* Liberation des ressources locales */
    if( p_file != NULL )
    {
        fclose( p_file );
    }


    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_play_selected(TUiContext argContext, const char *argFileName)
{
//    ui_clearScreen( argContext->cdkscreen_p );
    clear();

    argContext->playerPos.x = 1;
    argContext->playerPos.y = 1;

    if( s_loadGrid( argContext, argFileName) != EXIT_SUCCESS )
    {
        char*   pp_text[1]  = {0};

        pp_text[ 0 ]    = "An error occured while loading the grid !";

        ui_popupBlocking( argContext->cdkscreen_p,
                          pp_text, 1 );
        return;
    }


    while(      argContext->playerPos.x
            !=  (int)grid_columnsCount(argContext->gridData)-2
        ||      argContext->playerPos.y
            !=  (int)grid_rowsCount(argContext->gridData)-2 )
    {
        ui_play_drawGrid( argContext );
        ui_play_movePlayer( argContext );
    }

    char*   pp_text[3] = {
        CDK_STYLE_TITLE2 "You won the game ! :-)",
        "",
        CDK_STYLE_EMPHASIS1 "<Press any key to continue>"
    };

    ui_popupBlocking( argContext->cdkscreen_p,
                      pp_text, 3 );

    /* Release resources */
    grid_destroy( &argContext->gridData );
    clear();
}

/* ########################################################################## */
/* ########################################################################## */
