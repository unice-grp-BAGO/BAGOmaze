TEMPLATE    =   subdirs
CONFIG      +=  ordered

#
# Module "core"
#
SUBDIRS += core/module_core.pro
module_core.subdir  = core/


#
# Module "ui"
#
SUBDIRS += ui/module_ui.pro
module_ui.subdir    = ui/
module_ui.depends   = module_core
