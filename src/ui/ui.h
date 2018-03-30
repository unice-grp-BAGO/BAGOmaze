#ifndef UI_H
#define UI_H


typedef struct SUiContext*  TUiContext;

TUiContext  ui_initialize(void);
void        ui_finalize(TUiContext argContext);



#endif  /*< UI_H */
