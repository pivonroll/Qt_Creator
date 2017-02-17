/**************************************************************************
**
** Copyright (c) 2016 Bojan Petrovic
** Copyright (c) 2016 Radovan Zivkovic
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "msbuildoutputparser.h"
#include "vcmakestep.h"
#include "../vcprojectbuildconfiguration.h"
#include "../vcprojectfile.h"
#include "../vcprojectkitinformation.h"
#include "../common/vcprojectmanagerconstants.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

#include <utils/qtcassert.h>
#include <QFormLayout>
#include <QLabel>

/*!
 * \class VcMakeStep
 * \brief This class represents an implementation of a make step for Visual Studio project.
 *
 * \fn bool init()
 * \reimp
 * Initializes a build process parameters befor the build starts.
 *
 * \fn void run(QFutureInterface<bool> &fi)
 * \reimp
 * Starts the build process for Visual Studio project.
 *
 * \fn ProjectExplorer::BuildStepConfigWidget *createConfigWidget()
 * \reimp
 * Creates a config widget for Visual Studio project's make step.
 *
 * \fn bool immutable() const
 * \reimp
 * Returns false.
 *
 * \fn VcProjectBuildConfiguration *vcProjectBuildConfiguration() const
 * Returns a Visual Studio project's build configuration associated with this make step.
 *
 * \fn QStringList buildArguments() const
 * Returns build process's arguments.
 *
 * \fn QString buildArgumentsToString() const
 * Returns build process's arguments as one string.
 *
 * \fn void addBuildArgument(const QString &argument)
 * Adds a build argument.
 *
 * \fn void removeBuildArgument(const QString &buildArgument)
 * Removes a build argument.
 *
 * \fn QVariantMap toMap() const
 * \reimp
 * Puts make step's build arguments into the map and returns that map.
 *
 * \fn bool fromMap(const QVariantMap &map)
 * \reimp
 * Returns true if it successfully read build arguemnts from \a map.
 *
 * \fn void stdOutput(const QString &line)
 * Sends build process's output to the parser.
 */

/*!
 * class VcMakeStepConfigWidget
 * The config widget for \a VcMakeStep.
 *
 * \fn QString displayName() const;
 * Returns a display name to be used for this widget.
 *
 * \fn QString summaryText() const;
 * Returns a summary text to be used for this widget.
 */

/*!
 * \class VcMakeStepFactory
 * Used to create make steps for a Visual Studio project.
 * It implements \a ProjectExplorer::IBuildStepFactory.
 */

namespace VcProjectManager {
namespace Internal {

namespace {
const char MS_ID[] = "VisualStudioProjectManager.MakeStep";
const char CLEAN_KEY[] = "VisualStudioProjectManager.MakeStep.Clean";
const char BUILD_TARGETS_KEY[] = "VisualStudioProjectManager.MakeStep.BuildTargets";
const char ADDITIONAL_ARGUMENTS_KEY[] = "VisualStudioProjectManager.MakeStep.AdditionalArguments";
}

/*!
 *  Constructor. \a bsl is a list of build steps that will be executed as a part of make step.
 */

VcMakeStep::VcMakeStep(ProjectExplorer::BuildStepList *bsl)
    : AbstractProcessStep(bsl, Core::Id(MS_ID))
    , m_processParams(nullptr)
{
}

VcMakeStep::~VcMakeStep()
{
}

bool VcMakeStep::init()
{
    VcProjectBuildConfiguration *bc = vcProjectBuildConfiguration();
    MsBuildInformation *msBuild = VcProjectKitInformation::msBuildInfo(target()->kit());

    if (!bc || !msBuild || msBuild->m_executable.isEmpty()) {
        m_tasks.append(ProjectExplorer::Task(ProjectExplorer::Task::Error,
                                             tr("Kit doesn't contain any proper MS Build tool for .vcproj files."),
                                             Utils::FileName(), -1,
                                             Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)));
    }

    m_processParams = processParameters();

    if (msBuild)
        m_processParams->setCommand(msBuild->m_executable);

    m_processParams->setMacroExpander(bc->macroExpander());
    m_processParams->setEnvironment(bc->environment());
    m_processParams->setWorkingDirectory(bc->buildDirectory());

    if (!m_buildArguments.isEmpty())
        m_processParams->setArguments(m_buildArguments.join(QLatin1String(" ")));

    setOutputParser(new MsBuildOutputParser);
    return AbstractProcessStep::init();
}

void VcMakeStep::run(QFutureInterface<bool> &fi)
{
    bool canContinue = true;

    foreach (const ProjectExplorer::Task &t, m_tasks) {
        addTask(t);
        canContinue = false;
    }

    if (!canContinue) {
        emit addOutput(tr("Configuration is faulty. Check the Issues view for details."), BuildStep::MessageOutput);
        fi.reportResult(false);
        emit finished();
        return;
    }

    AbstractProcessStep::run(fi);
}

ProjectExplorer::BuildStepConfigWidget *VcMakeStep::createConfigWidget()
{
    return new VcMakeStepConfigWidget(this);
}

bool VcMakeStep::immutable() const
{
    return false;
}

VcProjectBuildConfiguration *VcMakeStep::vcProjectBuildConfiguration() const
{
    return static_cast<VcProjectBuildConfiguration *>(buildConfiguration());
}

QStringList VcMakeStep::buildArguments() const
{
    return m_buildArguments;
}

/*!
 * \return build process's arguments as one string.
 * Arguments are separated by space character.
 */
QString VcMakeStep::buildArgumentsToString() const
{
    return m_buildArguments.join(QLatin1String(" "));
}

/*!
 * \brief Adds a build argument to a list of build arguments.
 */
void VcMakeStep::addBuildArgument(const QString &argument)
{
    QTC_ASSERT(!m_buildArguments.contains(argument), return);
    m_buildArguments.append(argument);
}

/*!
 * \brief Removes a build argument from the list of build arguments.
 */
void VcMakeStep::removeBuildArgument(const QString &buildArgument)
{
    m_buildArguments.removeAll(buildArgument);
}

/*!
 * \return a map with make step's build arguments.

 */
QVariantMap VcMakeStep::toMap() const
{
    QVariantMap map = BuildStep::toMap();
    map.insert(QLatin1String(Constants::VC_PROJECT_MS_BUILD_ARGUMENT_LIST), m_buildArguments);
    return map;
}

/*!
 * \brief Reads make step's build arguments from \a map.
 * \return \b true if reading was successfull. Otherwise it returns false.
 */
bool VcMakeStep::fromMap(const QVariantMap &map)
{
    m_buildArguments = map.value(QLatin1String(Constants::VC_PROJECT_MS_BUILD_ARGUMENT_LIST)).toStringList();
    return BuildStep::fromMap(map);
}

/*!
 * \brief Sends process's output to the attached parser.
 * See \a MsBuildOutputParser for parser details.
 */
void VcMakeStep::stdOutput(const QString &line)
{
    AbstractProcessStep::stdOutput(line);
}

/*!
 * Constructor.
 */
VcMakeStep::VcMakeStep(ProjectExplorer::BuildStepList *parent, VcMakeStep *vcMakeStep) :
    AbstractProcessStep(parent, vcMakeStep)
{
}

/////////////////////////
// VcMakeStepConfigWidget
/////////////////////////
VcMakeStepConfigWidget::VcMakeStepConfigWidget(VcMakeStep *makeStep) :
    m_makeStep(makeStep)
{
    QFormLayout *mainLayout = new QFormLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_msBuildPath = new QLabel(this);
    mainLayout->addRow(tr("Command:"), m_msBuildPath);
    setLayout(mainLayout);

    connect(m_makeStep->target(), &ProjectExplorer::Target::kitChanged, this, &VcMakeStepConfigWidget::msBuildUpdated);

    MsBuildInformation *msBuild = VcProjectKitInformation::msBuildInfo(m_makeStep->target()->kit());
    QTC_ASSERT(msBuild, return);
    m_msBuildPath->setText(msBuild->m_executable);
}

QString VcMakeStepConfigWidget::displayName() const
{
    return tr("Vc Make Step Config Widget");
}

QString VcMakeStepConfigWidget::summaryText() const
{
    MsBuildInformation *msBuild = VcProjectKitInformation::msBuildInfo(m_makeStep->target()->kit());

    QTC_ASSERT(msBuild, return QString());
    QFileInfo fileInfo(msBuild->m_executable);
    return QString(QLatin1String("<b>MsBuild:</b> %1 %2")).arg(fileInfo.fileName())
            .arg(m_makeStep->buildArgumentsToString());
}

/*!
 * \brief VcMakeStepConfigWidget::msBuildUpdated
 * Called when MS Build information in Kit is changed.
 */
void VcMakeStepConfigWidget::msBuildUpdated()
{
    VcProjectBuildConfiguration *bc = static_cast<VcProjectBuildConfiguration *>(m_makeStep->buildConfiguration());

    if (bc && bc->target() && bc->target()->kit()) {
        MsBuildVersionManager *msBVM = MsBuildVersionManager::instance();
        MsBuildInformation *info = msBVM->msBuildInformation(Core::Id::fromSetting(bc->target()->kit()->value(Core::Id(Constants::VC_PROJECT_KIT_INFO_ID))));

        if (info)
            m_msBuildPath->setText(info->m_executable);
        else
            m_msBuildPath->setText(tr("<MS Build not available>"));
    }
}

////////////////////
// VcMakeStepFactory
////////////////////
VcMakeStepFactory::VcMakeStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{
}

VcMakeStepFactory::~VcMakeStepFactory()
{
}

bool VcMakeStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, Core::Id id) const
{
    if (parent->target()->project()->id() == Constants::VC_PROJECT_ID ||
            parent->target()->project()->id() == Constants::VC_PROJECT_2005_ID)
        return id == MS_ID;
    return false;
}

ProjectExplorer::BuildStep* VcMakeStepFactory::create(ProjectExplorer::BuildStepList *parent, Core::Id id)
{
    if (!canCreate(parent, id))
        return nullptr;
    VcMakeStep *step = new VcMakeStep(parent);
    return step;
}

bool VcMakeStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) const
{
    return canCreate(parent, product->id());
}

ProjectExplorer::BuildStep *VcMakeStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product)
{
    if (!canClone(parent, product))
        return nullptr;
    return new VcMakeStep(parent, static_cast<VcMakeStep *>(product));
}

bool VcMakeStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *VcMakeStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return nullptr;
    VcMakeStep *bs = new VcMakeStep(parent);
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return nullptr;
}

QList<Core::Id> VcMakeStepFactory::availableCreationIds(ProjectExplorer::BuildStepList *parent) const
{
    if (parent->target() && parent->target()->project() &&
            (parent->target()->project()->id() == Constants::VC_PROJECT_ID ||
             parent->target()->project()->id() == Constants::VC_PROJECT_2005_ID))
            return QList<Core::Id>() << Core::Id(MS_ID);

    return QList<Core::Id>();
}

QString VcMakeStepFactory::displayNameForId(Core::Id id) const
{
    if (id == MS_ID)
        return tr("Make", "Vc Project Make Step Factory id.");
    return QString();
}


} // namespace Internal
} // namespace VcProjectManager
