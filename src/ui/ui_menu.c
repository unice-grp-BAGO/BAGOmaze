#include "ui_menu.h"

#include <cdk/cdk.h>
#include <locale.h>
#include <malloc.h>
#include <ncurses.h>
#include <string.h>

#include "core/log/log.h"

#include "ui_utils.h"


/* ########################################################################## */
/* ########################################################################## */


const int   C_MENU_BORDER_HEIGHT    = 1; /*!< Epaisseur verticale bordure. */
const int   C_MENU_BORDER_WIDTH     = 1; /*!< Epaisseur horizontale bordure. */


/** @var    C_MENU_HEADER_HEIGHT
 *  @brief  Hauteur du "header" d'un menu.
 * Est en fait la hauteur de la bordure de la fenêtre (1 char) + la hauteur du
 * titre (1 char).
 */
const int   C_MENU_HEADER_HEIGHT    = 2;

/**
 *  @brief  Longueur max de la partie "description" d'un item de menu.
 */
const int    C_MENU_ITEM_DESCRIPTION_MAXLENGTH  = 30;

/**
 *  @brief  Longueur max du nom d'un item de menu.
 */
const int    C_MENU_ITEM_NAME_MAXLENGTH         = 50;

const char*         C_MENU_EXIT_ITEMLABEL   = "  [ Exit menu ]  ";
static const char*  C_MENU_MARK             = " [*]";
const char*         C_MENU_SPACERITEM_LABEL = " ";


/* ########################################################################## */
/* ########################################################################## */

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "UI", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "UI", format, ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */

MENU*   ui_menu_creation( ITEM**    argInItems_pp,
                          WINDOW*   *argOutMenuWindow_pp,
                          int*      argOutMenuWindowHeight_p,
                          int*      argOutMenuWindowWidth_p )
{
    MENU*   ret_p   = NULL;


    /*
     *  Instanciation du menu
     */
    ret_p   = new_menu( argInItems_pp );
    set_menu_mark( ret_p, C_MENU_MARK );


    /* calcul des différentes dimentions */
    const int   c_marginH   = 2;
    const int   c_marginW   = 2;
    const int   c_separatorH= 1;

    (*argOutMenuWindowHeight_p)
            = ret_p->height
            + (2 * C_MENU_BORDER_HEIGHT)   /*< Epaisseur bordure. */
            + (2 * c_marginH)	/*< Marge de 2 chars */
            + C_MENU_HEADER_HEIGHT;

    (*argOutMenuWindowWidth_p)
            = ret_p->width
            + (2 * c_marginW)   /*< Marge de 2 caractères. */
            + (2 * C_MENU_BORDER_WIDTH);		/*< Epaisseur bordure. */

    int	lMenuWinBeginY	=	(LINES - (*argOutMenuWindowHeight_p)) / 3;
    int	lMenuWinBeginX	=	(COLS - (*argOutMenuWindowWidth_p)) / 2;


#if 0
    TRACE_DBG( "ret_p->height    == %d", ret_p->height );
    TRACE_DBG( "ret_p->width     == %d", ret_p->width );
    TRACE_DBG( "lMenuWindowHeight == %d", lMenuWindowHeight );
    TRACE_DBG( "lMenuWindowWidth  == %d", lMenuWindowWidth );
    TRACE_DBG( "lMenuWinBeginY    == %d", lMenuWinBeginY );
    TRACE_DBG( "lMenuWinBeginX    == %d", lMenuWinBeginX );
#endif


    /*
     *	Instanciation de la fenêtre principale du menu
     */
    (*argOutMenuWindow_pp)
            = newwin( (*argOutMenuWindowHeight_p),
                      (*argOutMenuWindowWidth_p),
                      lMenuWinBeginY,
                      lMenuWinBeginX );
    keypad( (*argOutMenuWindow_pp), TRUE );

    set_menu_win( ret_p, *argOutMenuWindow_pp );


    /*
     *	Creation d'une sous-fenêtre contenant les choix du menu
     */
    set_menu_sub( ret_p,
                  derwin( *argOutMenuWindow_pp,
                          ret_p->height,
                          (*argOutMenuWindowWidth_p) - c_marginW,
                          C_MENU_HEADER_HEIGHT + c_separatorH, /*< rows begin */
                          C_MENU_BORDER_WIDTH ) ); /*< cols begin */


    /*
     *	Display the menu in its associated subwindow
     */
    post_menu( ret_p );
    wrefresh( *argOutMenuWindow_pp );



    return ret_p;
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_menu_destruction( MENU   **argMenu_pp,
                             WINDOW **argMenuWindow_pp,
                             ITEM   **argItemPointersList)
{
    int lIterator   = 0;


    unpost_menu(*argMenu_pp);

    for(    lIterator = 0
        ;   argItemPointersList[ lIterator ]    != NULL
        ;   lIterator++ )
    {
        free_item( argItemPointersList[ lIterator ] );
    }
    free( argItemPointersList );
//    free_menu( *argMenu_pp );
    wclear( *argMenuWindow_pp );
    wrefresh( *argMenuWindow_pp );
    delwin( *argMenuWindow_pp );
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_menu_draw(   MENU*		argMenu_p,
                        WINDOW*		argMenuWindow_p,
                        const char*	argMenuName_p,
                        const int	argMenuWindowHeight,
                        const int	argMenuWindowWidth )
{
    /* Dessin du contour de la fenêtre principale */
    box( argMenuWindow_p, 0, 0 );

    /* ligne de séparation header/menu - intersection de gauche */
    mvwaddch( argMenuWindow_p,	C_MENU_HEADER_HEIGHT,	0, ACS_LTEE);

    /* ligne de séparation header/menu - Ligne horizontale */
    mvwhline( argMenuWindow_p,
              C_MENU_HEADER_HEIGHT,
              C_MENU_BORDER_WIDTH,
              ACS_HLINE,
              argMenuWindowWidth - (C_MENU_BORDER_WIDTH * 2) );

    /* ligne de séparation header/menu - intersection de droite */
    mvwaddch( argMenuWindow_p,
              C_MENU_HEADER_HEIGHT,
              argMenuWindowWidth - C_MENU_BORDER_WIDTH,
              ACS_RTEE );

    /* Affichage du titre du menu */
    ui_printInMiddle( argMenuWindow_p,
                          C_MENU_BORDER_HEIGHT,
                          C_MENU_BORDER_WIDTH,
                          argMenuWindowWidth - (2 * C_MENU_BORDER_WIDTH ),
                          argMenuName_p,
                          COLOR_PAIR( NCURSES_STYLE_COLORID_TITLE1 ),
                          TRUE );

    /* Affichage des informations complémentaires en bas de page */
    mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
    mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage");

    /* Actualisation de l'affichage */
    pos_menu_cursor( argMenu_p );
    wrefresh( argMenuWindow_p );
}

/* ########################################################################## */
/* ########################################################################## */
/**
 * @brief ui_menu_driverExec
 *
 * @param p_argMenu
 * @param p_argMenuWindow
 *
 * @retval  -1  If no selection has been done.
 * @return  The (zero-origin) index of the selected menu item in the menu's item
 *          pointers list otherwise.
 */
int     ui_menu_driverExec( MENU	*p_argMenu,
                            WINDOW  *p_argMenuWindow )
{
    int		lCharIn		= '\0';
    int		lRet		= -1;
    ITEM	*p_item		= NULL;

    lCharIn	= getch();

    switch( lCharIn )
    {
        case KEY_DOWN:
        case 'd':
        case 'D':
        {
            /* On descend d'un item */
            menu_driver(p_argMenu, REQ_DOWN_ITEM);

            /* On récupère un pointeur vers l'item courant */
            p_item	= current_item(p_argMenu);
            if( p_item == NULL )
            {
                TRACE_ERR( "NULL item !!" );
            }

            /* Gestion de l'item "vide" ("spacer") - Si on tombe dessus,
             * on descend encore une fois */
            else if( strcmp( p_item->name.str, C_MENU_SPACERITEM_LABEL ) == 0 )
            {
                menu_driver(p_argMenu, REQ_DOWN_ITEM);
            }
            else
            {
                /* To be MISRA_14_10 compliant. */
        #ifdef	DEBUG
                TRACE_DBG( "No error detected." );
        #endif
            }

            lRet	= -1;
            break;
        }


        case KEY_UP:
        case 'u':
        case 'U':
        {
            /* On monte d'un item */
            menu_driver(p_argMenu, REQ_UP_ITEM);

            /* On récupère un pointeur vers l'item courant */
            p_item	= current_item(p_argMenu);

            /* Gestion de l'item "vide" ("spacer") - Si on tombe dessus,
             * on monte encore une fois */
            if( strcmp( p_item->name.str, C_MENU_SPACERITEM_LABEL ) == 0 )
            {
                menu_driver(p_argMenu, REQ_UP_ITEM);
            }

            lRet	= -1;
            break;
        }


        case KEY_RETURN: /* Enter */
        {
            p_item	= current_item( p_argMenu );
            lRet	= item_index( p_item );
            break;
        }


        default:
            lRet	= -1;
            break;
    }

    wrefresh(p_argMenuWindow);

    return lRet;
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_menu_itemCreate( const char*     argName_p,
                            const char*     argDescription_p,
                            void*           argUserPtr,
                            const uint8_t   argNameFillChar,
                            const uint8_t   argDescriptionFillChar,
                            ITEM**          argOutItem_pp )
{
    char    *p_itemDescription  = 0;
    char    *p_itemName         = 0;


    /* Création du nom de l'item */
    if( argNameFillChar == (uint8_t)'\0' )
    {
        p_itemName  = strndup( argName_p, C_MENU_ITEM_NAME_MAXLENGTH );
    }
    else
    {
        p_itemName  = (char*) malloc( sizeof( char )
                                      * C_MENU_ITEM_NAME_MAXLENGTH );
        strncpy( p_itemName, argName_p, C_MENU_ITEM_NAME_MAXLENGTH );
        memset( p_itemName + strlen( argName_p ),
                argNameFillChar,
                C_MENU_ITEM_NAME_MAXLENGTH - strlen( argName_p ) );
        p_itemName[ C_MENU_ITEM_NAME_MAXLENGTH -1 ]   = '\0';
    }


    /* Création de la description de l'item */
    if( argDescriptionFillChar == (uint8_t)'\0' )
    {
        p_itemDescription   = strndup( argDescription_p,
                                       C_MENU_ITEM_DESCRIPTION_MAXLENGTH );
    }
    else
    {
        p_itemDescription
                = (char*) malloc( sizeof(char)
                                  * C_MENU_ITEM_DESCRIPTION_MAXLENGTH );
        strncpy( p_itemDescription, argDescription_p,
                 C_MENU_ITEM_DESCRIPTION_MAXLENGTH );
        memset( (p_itemDescription + strlen( argDescription_p )),
                argDescriptionFillChar,
                C_MENU_ITEM_DESCRIPTION_MAXLENGTH - strlen(argDescription_p) );
        p_itemDescription[ C_MENU_ITEM_DESCRIPTION_MAXLENGTH -1 ]   = '\0';
    }



    (*argOutItem_pp)    = new_item( p_itemName, p_itemDescription );
    set_item_userptr( (*argOutItem_pp), argUserPtr );

    free( p_itemDescription );
}

/* ########################################################################## */
/* ########################################################################## */
/**
 * @brief Permet de créer un item de menu représentant une valeur décimale.
 *
 * Fonction de commodité appelant \ref ui_menu_itemCreate.
 *
 * @param argName
 * @param argValue
 * @param argUserPtr
 * @param argOutItem_pp
 */
void    ui_menu_itemCreate_double( const char*  argName,
                                       const double argValue,
                                       const char*  argValueSuffix,
                                       void*        argUserPtr,
                                       ITEM**       argOutItem_pp )
{
    char    p_tmpBuff[ 128 ]  = {0};


    snprintf(  p_tmpBuff, C_MENU_ITEM_DESCRIPTION_MAXLENGTH,
               "%.3f %s",
               argValue,
               argValueSuffix != 0 ? argValueSuffix : "" );


    ui_menu_itemCreate( argName,
                            p_tmpBuff,
                            argUserPtr,
                            '_',
                            ' ',
                            argOutItem_pp );
}

/* ########################################################################## */
/* ########################################################################## */
/**
 *  @brief Permet de créer un item de menu représentant une valeur d'énuméré.
 *
 *  Lors de la création de l'item, la description est construite à partir des
 * valeurs contenues dans le tableau \p argEntriesList censé contenir l'ensemble
 * des valeurs possibles cd \p argValue et leur nom "humainement lisible".
 *
 * @param argName
 * Le nom de l'item.
 *
 * @param argEntriesList
 * L'ensemble des valeurs possibles de \p argValue sous forme d'un tableau
 * de \ref Ts_EnumMenuEntry terminé par une structure \ref Ts_EnumMenuEntry dont
 * le champ Ts_EnumMenuEntry::name est une chaine vide (`""`).
 *
 * @param argValue
 * La valeur courante de l'item.
 *
 * @param argUserPtr
 * Un pointeur `void*` dont l'usage est défini par l'utilisateur.
 *
 * @param argOutItem_pp
 * Adresse du pointeur dans lequel stocker l'item qui va être créé.
 */
void    ui_menu_itemCreate_enum( const char* argName,
                                     const Ts_EnumMenuEntry *argEntriesList,
                                     const int   argValue,
                                     void*       argUserPtr,
                                     ITEM**      argOutItem_pp )
{
    int         lIterator   = 0;
    const char* p_valueName = "[Unknown enum value]";


    /* Recherche du nom de la valeur */
    for( lIterator = 0 ;
         strlen( argEntriesList[ lIterator ].name ) != 0U ;
         lIterator++ )
    {
        if( argEntriesList[ lIterator ].id == argValue )
        {
            p_valueName = argEntriesList[ lIterator ].name;
            break;
        }
    }

    /* Création de l'item */
    ui_menu_itemCreate( argName,
                            p_valueName,
                            argUserPtr,
                            '_',
                            ' ',
                            argOutItem_pp );
}

/* ########################################################################## */
/* ########################################################################## */

void    ui_menu_itemCreate_int( const char* argName,
                                    const int   argValue,
                                    const char* argValueSuffix,
                                    void*       argUserPtr,
                                    ITEM**      argOutItem_pp )
{
    char    p_tmpBuff[25]   = {0};


    snprintf(  p_tmpBuff, 25, "%d %s",
               argValue,
               argValueSuffix != 0 ? argValueSuffix : "" );


    ui_menu_itemCreate( argName,
                            p_tmpBuff,
                            argUserPtr,
                            '_',
                            ' ',
                            argOutItem_pp );
}

/* ########################################################################## */
/* ########################################################################## */

void ui_menu_updateItems(MENU *argMenu_p)
{
    ITEM* p_itemTmp	= current_item( argMenu_p );

    menu_driver( argMenu_p, REQ_NEXT_ITEM );
    set_current_item( argMenu_p, p_itemTmp );
}

/* ########################################################################## */
/* ########################################################################## */
