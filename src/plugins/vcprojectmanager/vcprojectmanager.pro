QT += xml xmlpatterns

CONFIG (debug, debug | release) {
    DEFINES += VISUAL_STUDIO_PLUGIN_DEBUG
}

OTHER_FILES += \
    $$PWD/VcProject.mimetypes.xml \
    $$PWD/VcProjectManager.json.in

RESOURCES += \
    $$PWD/vcproject.qrc

include($$PWD/../../qtcreatorplugin.pri)
include($$PWD/utils/utils.pri)
include($$PWD/visualstudiointerfaces/visualstudiointerfaces.pri)
include($$PWD/visualstudiotoolattributes/visualstudiotoolattributes.pri)
include($$PWD/vcprojectmodel/vcprojectmodel.pri)
include($$PWD/vcprojectxmodel/vcprojectxmodel.pri)
include($$PWD/widgets/widgets.pri)
include($$PWD/visual_studio_old_version/visual_studio_old_version.pri)
include($$PWD/visual_studio_new_version/visual_studio_new_version.pri)
include($$PWD/visualstudiosolution/visualstudiosolution.pri)
include($$PWD/common/common.pri)
include($$PWD/ms_build/ms_build.pri)

HEADERS += \
    menuhandler.h \
    vcdocprojectnodes.h \
    vcprojectbuildconfiguration.h \
    vcprojectbuildoptionspage.h \
    vcprojectfile.h \
    vcprojectkitinformation.h \
    vcprojectmanager_global.h \
    vcprojectmanagerplugin.h \
    visualstudioproject.h

SOURCES += \
    menuhandler.cpp \
    vcdocprojectnodes.cpp \
    vcprojectbuildconfiguration.cpp \
    vcprojectbuildoptionspage.cpp \
    vcprojectfile.cpp \
    vcprojectkitinformation.cpp \
    vcprojectmanagerplugin.cpp \
    visualstudioproject.cpp

