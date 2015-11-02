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
    $$PWD/vcxprojectfile.h \
    $$PWD/vcdocprojectnodes.h \
    visualstudiosolutionmanager.h \
    visualstudiosolutionfile.h \
    visualstudiosolutionnode.h \
    visualstudioproject.h \

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
    $$PWD/vcxprojectfile.cpp \
    $$PWD/vcdocprojectnodes.cpp \
    visualstudiosolutionmanager.cpp \
    visualstudiosolutionfile.cpp \
    visualstudiosolutionnode.cpp \
    visualstudioproject.cpp \

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
include($$PWD/visualstudiowidgets/visualstudiowidgets.pri)
include($$PWD/visualstudiosolution/visualstudiosolution.pri)
