#ifndef LIBSTRING_GLOBAL_H
#define LIBSTRING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBSTRING_LIBRARY)
#  define LIBSTRINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSTRINGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBSTRING_GLOBAL_H
