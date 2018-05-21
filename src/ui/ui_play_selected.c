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


    if( argContext->gridData != NULL )
    {
        TRACE_ERR( "Can't load the grid : A grid already exist !" );
        retVal  = EXIT_FAILURE;
    }
    else if( (p_file=fopen( argFileName, "r" )) == NULL )
    {
        TRACE_ERR( "Can't load the grid : An error occured while opening the"
                   " file '%s' !",
                   argFileName );
        retVal  = EXIT_FAILURE;
    }


    else
    {
        char    lBuffer[C_BUFFER_LENGTH]    = {0};


        /*
         *  Find the size of the grid in the file
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
                lColumnsCountTmp++;
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
            TRACE_ERR( "Grid will be too small ! (detected a maximum of %d rows"
                       " and %d columns).",
                       lRowsCount, lColumnsCount );
            retVal  = EXIT_FAILURE;
        }
        else
        {
            getmaxyx(stdscr, argContext->gridOffsetY, argContext->gridOffsetX);
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
            argContext->gridOffsetX /= 2;
            argContext->gridOffsetY /= 2;



            argContext->gridData    = grid_create( lRowsCount,
                                                   lColumnsCount );
            lColumnsCount   = 0;
            lRowsCount      = 0;
            rewind( p_file );

            while( ! feof( p_file ) )
            {
                lColumnsCount   = 0;
                if(     fgets( lBuffer, C_BUFFER_LENGTH, p_file ) == NULL
                    ||  strlen( lBuffer ) == 0 )
                {
                    /* Can't proces line, just continue */
                    continue;
                }



                p_token = strtok(lBuffer,";");
                while( p_token != NULL )
                {
                    TEGridCellType lCellType
                            = grid_cellType_fromChar( p_token[ 0 ] );

                    grid_setCell( argContext->gridData,
                                  lRowsCount,
                                  lColumnsCount,
                                  lCellType );


                    /* get next token */
                    lColumnsCount++;
                    p_token = strtok(NULL,";");
                }


                lRowsCount++;
            }
        }
    }


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
