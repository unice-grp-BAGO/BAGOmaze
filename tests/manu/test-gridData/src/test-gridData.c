#include <stdio.h>
#include <stdlib.h>

#include "core/grid.h"


int main(void)
{
    printf( "Hello test !\n" );

    TCoreGrid   lGridData   = grid_create( 5, 7 );

    for(size_t col = 0 ; col < grid_columnsCount(lGridData) ; ++col )
    {
        grid_setCell( lGridData,
                      0, col,
                      EGridCellWall );
        grid_setCell( lGridData,
                      grid_rowsCount(lGridData) -1, col,
                      EGridCellWall );
    }

    for(size_t row = 0 ; row < grid_rowsCount(lGridData) ; ++row )
    {
        grid_setCell( lGridData,
                      row, 0,
                      EGridCellWall );
        grid_setCell( lGridData,
                      row, grid_columnsCount(lGridData) -1,
                      EGridCellWall );
    }

    grid_setCell( lGridData, 1, 0, EGridCellEmpty );

    grid_setCell( lGridData, 1, 2, EGridCellWall );
    grid_setCell( lGridData, 2, 2, EGridCellWall );

    grid_setCell( lGridData, 2, 4, EGridCellWall );
    grid_setCell( lGridData, 3, 4, EGridCellWall );

    grid_setCell( lGridData, 3, 6, EGridCellEmpty );

    grid_print( lGridData, stdout );

    grid_destroy( &lGridData );

    return EXIT_FAILURE;
}

