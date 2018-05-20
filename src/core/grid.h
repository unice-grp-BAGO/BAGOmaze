#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdlib.h>


/* -----------------------------------------------------------------------------
 *  Déclarations de types
 */
/**
 *  @brief  Type privé décrivant une grille de jeu.
 */
typedef struct _SCoreGrid*  TCoreGrid;


/**
 *  @brief  Type énuméré identifiant les différents types de cellules possibles
 *          dans la grille.
 */
typedef enum    _EGridCellTypes
{
    EGridCellUnknown        = -1,
    EGridCellEmpty          = 0,
    EGridCellEmptyVisited,
    EGridCellWall,
    EGridCellExit,

    _EGridCellTypesCount

}   TEGridCellType;

extern const TEGridCellType C_CELLTYPE_DEFAULT;



/* -----------------------------------------------------------------------------
 *  Fonctions de gestion de la grille
 */
TCoreGrid   grid_create(size_t argRows, size_t argCols );
void        grid_destroy(TCoreGrid *argGridPtr );


size_t  grid_columnsCount(TCoreGrid argGrid);
size_t  grid_rowsCount(TCoreGrid argGrid);



/* -----------------------------------------------------------------------------
 *  Fonctions de gestion des cellules
 */
TEGridCellType  grid_getCell( TCoreGrid argGrid,
                              size_t    argRow,
                              size_t    argCol );

void            grid_setCell( TCoreGrid         argGrid,
                              size_t            argRow,
                              size_t            argCol,
                              TEGridCellType    argCellType );


TEGridCellType  grid_cellType_fromChar( const char  argChar );
char            grid_cellType_toChar( const TEGridCellType  argType );



void    grid_print(TCoreGrid argGrid , FILE* argFD);



#endif  /*< GRID_H */
