/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "qnxabstractrunsupport.h"
#include "qnxrunconfiguration.h"

#include <projectexplorer/devicesupport/deviceusedportsgatherer.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/target.h>
#include <utils/portlist.h>
#include <utils/qtcassert.h>

using namespace ProjectExplorer;
using namespace RemoteLinux;

namespace Qnx {
namespace Internal {

QnxAbstractRunSupport::QnxAbstractRunSupport(RunControl *runControl)
    : ToolRunner(runControl)
    , m_state(Inactive)
{
    m_launcher = new ApplicationLauncher(this);
    m_portsGatherer = new DeviceUsedPortsGatherer(this);

    connect(m_portsGatherer, &DeviceUsedPortsGatherer::error,
            this, &QnxAbstractRunSupport::handleError);
    connect(m_portsGatherer, &DeviceUsedPortsGatherer::portListReady,
            this, &QnxAbstractRunSupport::handlePortListReady);
}

void QnxAbstractRunSupport::handleAdapterSetupRequested()
{
    QTC_ASSERT(m_state == Inactive, return);

    m_state = GatheringPorts;
    m_portsGatherer->start(m_device);
}

void QnxAbstractRunSupport::handlePortListReady()
{
    QTC_ASSERT(m_state == GatheringPorts, return);
    m_portList = device()->freePorts();
    startExecution();
}

void QnxAbstractRunSupport::handleRemoteProcessStarted()
{
    m_state = Running;
}

void QnxAbstractRunSupport::handleRemoteProcessFinished(bool)
{
}

void QnxAbstractRunSupport::setFinished()
{
    if (m_state != GatheringPorts && m_state != Inactive)
        m_launcher->stop();

    m_state = Inactive;
}

QnxAbstractRunSupport::State QnxAbstractRunSupport::state() const
{
    return m_state;
}

void QnxAbstractRunSupport::setState(QnxAbstractRunSupport::State state)
{
    m_state = state;
}

ApplicationLauncher *QnxAbstractRunSupport::appRunner() const
{
    return m_launcher;
}

void QnxAbstractRunSupport::handleProgressReport(const QString &)
{
}

void QnxAbstractRunSupport::handleRemoteOutput(const QByteArray &)
{
}

void QnxAbstractRunSupport::handleError(const QString &)
{
}

bool QnxAbstractRunSupport::setPort(Utils::Port &port)
{
    port = m_portsGatherer->getNextFreePort(&m_portList);
    if (!port.isValid()) {
        handleError(tr("Not enough free ports on device for debugging."));
        return false;
    }
    return true;
}

} // namespace Internal
} // namespace Qnx
