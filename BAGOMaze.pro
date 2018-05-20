TEMPLATE    =   subdirs
CONFIG      +=  ordered


#
# Modules
#
SUBDIRS += src/modules.pro
modules.subdir      = src/
module_core.subdir  = src/core/
module_ui.subdir    = src/ui/


#
# Application principale
#
SUBDIRS += src/application_principale.pro
application_principale.subdir   = src/
application_principale.depends  +=  modules
application_principale.depends  +=  module_core
application_principale.depends  +=  module_ui


#
# Executables de test
#
SUBDIRS +=  tests/tests.pro
tests.subdir    = tests/
tests.depends   +=  modules
