DIR_BUILD=$$PWD/build/

CONFIG(debug, debug|release){
    DIR_BUILD= $${DIR_BUILD}/debug
    TARGET=$$join(TARGET,"_d",,)
}else{
    DIR_BUILD= $${DIR_BUILD}/release
}

MOC_DIR     = $${DIR_BUILD}/moc
OBJECTS_DIR = $${DIR_BUILD}/obj
RCC_DIR     = $${DIR_BUILD}/rcc
UI_DIR      = $${DIR_BUILD}/ui

DESTDIR = $$PWD/out/$${TEMPLATE}
