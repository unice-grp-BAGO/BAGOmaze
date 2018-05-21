#include "grid.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/macros.h"

/* ########################################################################## */
/* ########################################################################## */

#define C_CHAR_CELLTYPE_EMPTY   ('E')
#define C_CHAR_CELLTYPE_EXIT    ('X')
#define C_CHAR_CELLTYPE_WALL    ('W')

/* ########################################################################## */
/* ########################################################################## */

struct  _SCoreGrid
{
    size_t  rowsCount;
    size_t  columsCount;

    TEGridCellType  *data;
};

/* ########################################################################## */
/* ########################################################################## */

const TEGridCellType    C_CELLTYPE_DEFAULT  = EGridCellWall;

/* ########################################################################## */
/* ########################################################################## */

TEGridCellType grid_cellType_fromChar(const char argChar)
{
    TEGridCellType  retVal  = EGridCellUnknown;


    switch( argChar )
    {
        case    C_CHAR_CELLTYPE_EMPTY:
            retVal  = EGridCellEmpty;
            break;


        case    C_CHAR_CELLTYPE_EXIT:
            retVal  = EGridCellExit;
            break;


        case    C_CHAR_CELLTYPE_WALL:
            retVal  = EGridCellWall;
            break;


        default:
            break;
    }


    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

char    grid_cellType_toChar(const TEGridCellType argType)
{
    char    retVal  = '?';

    switch( argType )
    {
        case    EGridCellEmpty:
        case    EGridCellEmptyVisited:
            retVal  = C_CHAR_CELLTYPE_EMPTY;
            break;


        case    EGridCellExit:
            retVal  = C_CHAR_CELLTYPE_EXIT;
            break;


        case    EGridCellWall:
            retVal  = C_CHAR_CELLTYPE_WALL;
            break;


        default:
            break;
    }

    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

size_t  grid_columnsCount(TCoreGrid argGrid)
{
    return  argGrid->columsCount;
}

/* ########################################################################## */
/* ########################################################################## */

TCoreGrid   grid_create(size_t argRows, size_t argCols)
{
    TCoreGrid   retVal  = (TCoreGrid)malloc( sizeof( struct _SCoreGrid ) );


    retVal->columsCount = argCols;
    retVal->rowsCount   = argRows;

    int lCellsCount = retVal->columsCount * retVal->rowsCount;

    retVal->data    = (TEGridCellType*)malloc(      sizeof( TEGridCellType )
                                                *   lCellsCount );

    for( size_t lRow = 0 ; lRow < retVal->rowsCount ; lRow++ )
    {
        for( size_t lCol = 0 ; lCol < retVal->columsCount ; lCol++ )
        {
            grid_setCell( retVal, lRow, lCol, C_CELLTYPE_DEFAULT );
        }

    }
//    memset(retVal->data, EGridCellUnknown, lCellsCount);


    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */

void    grid_destroy(TCoreGrid* argGridPtr)
{
    FREE( (*argGridPtr)->data );
    FREE( (*argGridPtr) );
}

/* ########################################################################## */
/* ########################################################################## */

TEGridCellType  grid_getCell( TCoreGrid argGrid,
                              size_t    argRow,
                              size_t    argCol )
{
    assert( argCol  < argGrid->columsCount);
    assert( argRow  < argGrid->rowsCount  );


    return argGrid->data[argRow * argGrid->columsCount + argCol];
}

/* ########################################################################## */
/* ########################################################################## */

void    grid_print(TCoreGrid argGrid, FILE* argFD)
{
    assert( argFD != NULL && "argFD must be valid !" );


    for(size_t row = 0 ; row < argGrid->rowsCount ; row++ )
    {
        for( size_t col = 0 ; col < argGrid->columsCount ; col++ )
        {
            if( col != 0 )
            {
                putc( ';', argFD );
            }

            putc( grid_cellType_toChar( grid_getCell( argGrid, row, col ) ),
                  argFD );
        }

        putc( '\n', argFD );
    }
}

/* ########################################################################## */
/* ########################################################################## */

size_t  grid_rowsCount(TCoreGrid argGrid)
{
    return  argGrid->rowsCount;
}

/* ########################################################################## */
/* ########################################################################## */

void    grid_setCell( TCoreGrid         argGrid,
                      size_t            argRow,
                      size_t            argCol,
                      TEGridCellType    argCellType )
{
    assert( argCol  < argGrid->columsCount);
    assert( argRow  < argGrid->rowsCount  );


    argGrid->data[argRow * argGrid->columsCount + argCol]   = argCellType;
}

/* ########################################################################## */
/* ########################################################################## */
