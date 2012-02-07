load(qt_plugin)

TARGET = qtgeoservices_qmltestplugin
QT += location

INCLUDEPATH += ../../../src/location/maps

DESTDIR = $$QT.location.plugins/geoservices

HEADERS += qgeocodingmanagerengine_test.h \
           qgeoserviceproviderplugin_test.h \
           qgeoroutingmanagerengine_test.h \
           qplacemanagerengine_test.h \
           qgeomappingmanagerengine_test.h

SOURCES += qgeoserviceproviderplugin_test.cpp

target.path += $$[QT_INSTALL_PLUGINS]/geoservices
INSTALLS += target
