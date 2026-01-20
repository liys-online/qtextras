#ifndef OPENAI_GLOBAL_H
#define OPENAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OPENAI_LIBRARY)
#define QTEXTRAS_OPENAI_EXPORT Q_DECL_EXPORT
#else
#define QTEXTRAS_OPENAI_EXPORT Q_DECL_IMPORT
#endif

#endif // OPENAI_GLOBAL_H
