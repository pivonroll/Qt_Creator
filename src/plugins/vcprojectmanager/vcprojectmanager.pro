QT += xml xmlpatterns

HEADERS = vcprojectmanagerplugin.h \
    vcprojectmanagerconstants.h \
    vcprojectmanager_global.h \
    vcprojectmanager.h \
    vcprojectfile.h \
    vcproject.h \
    vcprojectbuildconfiguration.h \
    vcmakestep.h \
    msbuildoutputparser.h \
    vcprojectkitinformation.h \
    msbuildversionmanager.h \
    vcprojectbuildoptionspage.h \
    vcschemamanager.h \
    menuhandler.h \
    utils.h \
    vcxprojectmanager.h \
    vcxproject.h \
    vcxprojectfile.h \
    vcdocprojectnodes.h \

SOURCES = vcprojectmanagerplugin.cpp \
    vcprojectmanager.cpp \
    vcprojectfile.cpp \
    vcproject.cpp \
    vcprojectbuildconfiguration.cpp \
    vcmakestep.cpp \
    msbuildoutputparser.cpp \
    vcprojectkitinformation.cpp \
    msbuildversionmanager.cpp \
    vcprojectbuildoptionspage.cpp \
    vcschemamanager.cpp \
    menuhandler.cpp \
    utils.cpp \
    vcxprojectmanager.cpp \
    vcxproject.cpp \
    vcxprojectfile.cpp \
    vcdocprojectnodes.cpp \

OTHER_FILES += \
    VcProject.mimetypes.xml \
    VcProjectManager.json.in

RESOURCES += \
    vcproject.qrc

include(../../qtcreatorplugin.pri)
include($$PWD/interfaces/vsinterfaces.pri)
include($$PWD/vcprojectmodel/vcprojectmodel.pri)
include($$PWD/vcprojectxmodel/vcprojectxmodel.pri)
include($$PWD/widgets/vswidgets.pri)
