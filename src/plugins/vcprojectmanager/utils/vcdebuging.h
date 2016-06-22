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
#define VS_DEBUG_PRINT(message) vs_dbg_print << message
#else
#define VS_DEBUG_PRINT(mesage)
#endif

#define VS_DEBUG_ASSERT(cond, message) if (cond) {} else { VS_DEBUG_PRINT(QString::fromLatin1("\"" #cond "\"") + QLatin1String(" - ") + message); } do {} while (0)
#define VS_DEBUG_ASSERT_ACTION(cond, message, action) if (cond) {} else { VS_DEBUG_PRINT(QString::fromLatin1("\"" #cond "\"") + QLatin1String(" - ") + message); action; } do {} while (0)


