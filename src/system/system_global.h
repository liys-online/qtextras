#ifndef SYSTEM_GLOBAL_H
#define SYSTEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SYSTEM_LIBRARY)
#define QTEXTRAS_SYSTEM_EXPORT Q_DECL_EXPORT
#else
#define QTEXTRAS_SYSTEM_EXPORT Q_DECL_IMPORT
#endif

#endif // SYSTEM_GLOBAL_H
