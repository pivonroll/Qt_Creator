QT += xml xmlpatterns

HEADERS = \
    $$PWD/vcprojectmanagerplugin.h \
    $$PWD/vcprojectmanagerconstants.h \
    $$PWD/vcprojectmanager_global.h \
    $$PWD/vcprojectmanager.h \
    $$PWD/vcprojectfile.h \
    $$PWD/vcproject.h \
    $$PWD/vcprojectbuildconfiguration.h \
    $$PWD/vcmakestep.h \
    $$PWD/msbuildoutputparser.h \
    $$PWD/vcprojectkitinformation.h \
    $$PWD/msbuildversionmanager.h \
    $$PWD/vcprojectbuildoptionspage.h \
    $$PWD/vcschemamanager.h \
    $$PWD/menuhandler.h \
    $$PWD/utils.h \
    $$PWD/vcxprojectmanager.h \
    $$PWD/vcxproject.h \
    $$PWD/vcdocprojectnodes.h \
    $$PWD/visualstudiosolutionmanager.h \
    $$PWD/visualstudiosolutionfile.h \
    $$PWD/visualstudiosolutionnode.h \
    $$PWD/visualstudioproject.h \
    $$PWD/vcprojkitmatcher.h

SOURCES = vcprojectmanagerplugin.cpp \
    $$PWD/vcprojectmanager.cpp \
    $$PWD/vcprojectfile.cpp \
    $$PWD/vcproject.cpp \
    $$PWD/vcprojectbuildconfiguration.cpp \
    $$PWD/vcmakestep.cpp \
    $$PWD/msbuildoutputparser.cpp \
    $$PWD/vcprojectkitinformation.cpp \
    $$PWD/msbuildversionmanager.cpp \
    $$PWD/vcprojectbuildoptionspage.cpp \
    $$PWD/vcschemamanager.cpp \
    $$PWD/menuhandler.cpp \
    $$PWD/utils.cpp \
    $$PWD/vcxprojectmanager.cpp \
    $$PWD/vcxproject.cpp \
    $$PWD/vcdocprojectnodes.cpp \
    $$PWD/visualstudiosolutionmanager.cpp \
    $$PWD/visualstudiosolutionfile.cpp \
    $$PWD/visualstudiosolutionnode.cpp \
    $$PWD/visualstudioproject.cpp \
    $$PWD/vcprojkitmatcher.cpp

OTHER_FILES += \
    $$PWD/VcProject.mimetypes.xml \
    $$PWD/VcProjectManager.json.in

RESOURCES += \
    $$PWD/vcproject.qrc

include($$PWD/../../qtcreatorplugin.pri)
include($$PWD/visualstudiointerfaces/visualstudiointerfaces.pri)
include($$PWD/visualstudiotoolattributes/visualstudiotoolattributes.pri)
include($$PWD/vcprojectmodel/vcprojectmodel.pri)
include($$PWD/vcprojectxmodel/vcprojectxmodel.pri)
include($$PWD/widgets/widgets.pri)
include($$PWD/visualstudiosolution/visualstudiosolution.pri)
