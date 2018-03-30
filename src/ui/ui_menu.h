#ifndef UI_UTILS_MENU_H
#define UI_UTILS_MENU_H

#include </usr/include/menu.h>
#include <stdint.h>

typedef struct
{
    uint64_t        id;
    const char*     name;
} Ts_EnumMenuEntry;

extern const int    C_MENU_BORDER_HEIGHT;
extern const int    C_MENU_BORDER_WIDTH;
extern const int    C_MENU_HEADER_HEIGHT;
extern const int    C_MENU_ITEM_DESCRIPTION_MAXLENGTH;
extern const int    C_MENU_ITEM_NAME_MAXLENGTH;

extern const char*  C_MENU_EXIT_ITEMLABEL;
extern const char*  C_MENU_SPACERITEM_LABEL;



MENU*   ui_menu_creation( ITEM      **argInItems_pp,
                          WINDOW    **argOutMenuWindow_pp ,
                          int*      argOutMenuWindowHeight_p,
                          int       *argOutMenuWindowWidth_p );


void    ui_menu_draw( MENU          *argMenu_p,
                      WINDOW        *argMenuWindow_p,
                      const char    *argMenuName_p,
                      const int     argMenuWindowHeight,
                      const int     argMenuWindowWidth );

int     ui_menu_driverExec( MENU	*p_argMenu,
                            WINDOW	*p_argMenuWindow );


void    ui_menu_itemCreate( const char*    argName_p,
                            const char*    argDescription_p,
                            void*          argUserPtr,
                            const uint8_t  argNameFillChar,
                            const uint8_t  argDescriptionFillChar,
                            ITEM**         argOutItem_pp );

void    ui_menu_itemCreate_double( const char*  argName,
                                   const double argValue,
                                   const char*  argValueSuffix,
                                   void*        argUserPtr,
                                   ITEM**       argOutItem_pp );

void    ui_menu_itemCreate_enum(const char*                 argName,
                                const Ts_EnumMenuEntry*     argEntriesList,
                                const int                   argValue,
                                void*                       argUserPtr,
                                ITEM**                      argOutItem_pp );

void    ui_menu_itemCreate_int( const char* argName,
                                const int   argValue,
                                const char* argValueSuffix,
                                void*       argUserPtr,
                                ITEM**      argOutItem_pp );

void    ui_menu_updateItems( MENU*  argMenu_p );


void    ui_menu_destruction( MENU **argMenu_pp,
                             WINDOW **argMenuWindow_pp,
                             ITEM **argItemPointersList );

#endif  /*< UI_UTILS_MENU_H */
