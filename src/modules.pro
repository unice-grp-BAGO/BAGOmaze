TEMPLATE    =   subdirs
CONFIG      +=  ordered

#
# Module "core"
#
SUBDIRS += core/module_core.pro


#
# Module "ui"
#
SUBDIRS += ui/module_ui.pro
module_ui.depends   = module_core
