TEMPLATE    =   subdirs
CONFIG      +=  ordered


#
# Modules
#
SUBDIRS += src/modules.pro


#
# Application principale
#
SUBDIRS += src/application_principale.pro
application_principale.depends  +=  modules


#
# Executables de test
#
SUBDIRS +=  tests
tests.depends   +=  modules
