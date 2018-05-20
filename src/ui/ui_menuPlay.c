#include "ui_menuPlay.h"

#include <assert.h>

#include "core/log/log.h"

#include "ui_menu.h"
#include "ui_utils.h"
#include "ui_play_random.h"
#include "ui_private.h"

/* ########################################################################## */
/* ########################################################################## */

#define TRACE_DBG(format,...) \
        TRACE_DBG_BASE( "UI", format, ##__VA_ARGS__ );

#define TRACE_ERR(format,...) \
        TRACE_ERR_BASE( "UI", format, ##__VA_ARGS__ );

/* ########################################################################## */
/* ########################################################################## */

int     ui_menuPlay_exec(TUiContext argContext)
{
    enum    EItemsMenuPrincipal
    {
        EMenuItemArcade = 0,
        /*!< Item 1. */

        EMenuItemChooseLevel,
        /*!< item 2. */

        EMenuItemRandom,
        /*!< item 3. */


        _ESpaceritem1,
        /*!< "Invisible" item to do a space between mode items and exit menu. */

        EMenuItemExit,
        /*!< "Exit menu" item. */


        /** In the menu declaration, the items list shall contain a NULL item as
         * the last one. */
        EMenuLastItem,

        /** To know how many items the menu's table shall contain. */
        EMenuItemsCount
    };



    u_int8_t            lContinue               = TRUE;
    ITEM                **lItemsPointersList    = 0;
    int                 lIterator               = 0;
    char                *lMsg_pp[10]            = {0};
    MENU                *p_menu                 = 0;
    WINDOW              *p_menuWindow           = 0;
    int                 retVal                  = EXIT_SUCCESS;



    /* -------------------------------------------------------------------------
     *  Initialisation
     */
    ui_ncursesRestore();
    cbreak();
    noecho();
    keypad( stdscr, TRUE );


    /* -------------------------------------------------------------------------
     *  Instanciation des items du menu
     */

    lItemsPointersList
            = (ITEM **)calloc( EMenuItemsCount,
                               sizeof(ITEM *) );


    for( lIterator = 0 ; lIterator < EMenuLastItem ; lIterator++ )
    {
        ITEM**      pp_item             = &lItemsPointersList[ lIterator ];
        char*       p_itemDescription   = 0;
        const char* p_itemName          = 0;
        void*       p_itemUserPtr       = 0;


        switch( lIterator )
        {
            case _ESpaceritem1:
                p_itemName          = C_MENU_SPACERITEM_LABEL;
                p_itemDescription   = "";
                p_itemUserPtr       = (void*)_ESpaceritem1;
                break;


            case EMenuItemArcade:
                p_itemName          = "Arcade";
                p_itemDescription   = "";
                p_itemUserPtr       = (void*)EMenuItemArcade;
                break;


            case EMenuItemChooseLevel:
                p_itemName          = "Choose Level";
                p_itemDescription   = "";
                p_itemUserPtr       = (void*)EMenuItemChooseLevel;
                break;


            case EMenuItemExit:
                p_itemName          = C_MENU_EXIT_ITEMLABEL;
                p_itemDescription   = "";
                p_itemUserPtr       = (void*)EMenuItemExit;
                break;


            case EMenuItemRandom:
                p_itemName          = "Random";
                p_itemDescription   = "";
                p_itemUserPtr       = (void*)EMenuItemRandom;
                break;


            default:
                assert( ! "the value should always be one of the enum tags" );
                break;


        }

        ui_menu_itemCreate( p_itemName,
                            p_itemDescription,
                            p_itemUserPtr,
                            '\0', '\0',
                            pp_item );
    }

    /* Terminating item. */
    lItemsPointersList[EMenuLastItem]   = (ITEM*)NULL;



    /* -------------------------------------------------------------------------
     *  Instanciation du menu
     */
    int lMenuWindowHeight   = 0;
    int lMenuWindowWidth    = 0;
    p_menu  = ui_menu_creation( lItemsPointersList,
                                &p_menuWindow,
                                &lMenuWindowHeight,
                                &lMenuWindowWidth );


    /* -------------------------------------------------------------------------
     *	Boucle de gestion de l'affichage
     */
    do
    {
        /* ---------------------------------------------------------------------
         *	Dessin des "éléments fixes"
         */
        ui_menu_draw( p_menu, p_menuWindow,
                      "Play",
                      lMenuWindowHeight,
                      lMenuWindowWidth );



        /* ---------------------------------------------------------------------
         *	Gestion des actions du menu
         */
        int	lSelectedItemIndex
                = ui_menu_driverExec( p_menu, p_menuWindow );

        switch( lSelectedItemIndex )
        {
            case -1:
                /* This is a "menu management" loop / No action is needed */
                break;


            case EMenuItemExit:
            {
                retVal      = EXIT_SUCCESS;
                lContinue   = FALSE;
                break;
            }

#if 1
#warning "Not implemented !"
#else
            case EMenuItemArcade:
            {
                TRACE_DBG( "Selected : Arcade" );
                ui_play_random( argContext );
                break;
            }


            case EMenuItemChooseLevel:
            {
                TRACE_DBG( "Selected : Choose Level" );
                break;
            }
#endif

            case EMenuItemRandom:
            {
                TRACE_DBG( "Selected : Random" );
                ui_play_random( argContext );
                break;
            }


            default:
                lMsg_pp[0]	= CDK_STYLE_TITLE1 "Error !";
                lMsg_pp[1]	= "";
                lMsg_pp[2]	= CDK_STYLEALIGN_CENTERED
                              "This case has not been implemented !";
                lMsg_pp[3]	= (char*)__PRETTY_FUNCTION__;
                lMsg_pp[4] = CDK_STYLE_EMPHASIS1
                             "Press any key to continue.";
                popupLabel( argContext->cdkscreen_p, lMsg_pp, 5 );
                break;
        }

    } while( lContinue == TRUE );



    /* -------------------------------------------------------------------------
     *  Finalisation
     */

    ui_menu_destruction( &p_menu,
                         &p_menuWindow,
                         lItemsPointersList );


    ui_ncursesHold();

    return retVal;
}

/* ########################################################################## */
/* ########################################################################## */
