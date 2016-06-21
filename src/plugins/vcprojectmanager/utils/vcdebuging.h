#pragma once

#include <QDebug>
#include <QFileInfo>
#include <QString>

#if _MSC_VER && !__INTEL_COMPILER
#define vs_dbg_print	qDebug() << qPrintable(QFileInfo(QLatin1String(__FILE__)).fileName()) \
                    << qPrintable(QString::number(__LINE__)) \
                    << qPrintable(QString::fromLatin1(__FUNCTION__).section(QLatin1String("::"), -3).append(QLatin1String(":")))
#elif __GNUC__ && __cplusplus
#define vs_dbg_print	qDebug() << qPrintable(QFileInfo(QLatin1String(__FILE__)).fileName()) \
                    << qPrintable(QString::number(__LINE__)) \
                    << qPrintable(QString::fromLatin1(__PRETTY_FUNCTION__).toString().section(QLatin1String("::"), -3).section('(', 0, 0).append(QLatin1String(":")))
#endif

#ifdef VISUAL_STUDIO_PLUGIN_DEBUG
#define vs_debugPrint(data) vs_dbg_print << data
#else
#define vs_debugPrint(data)
#endif

