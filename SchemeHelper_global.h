#ifndef SCHEMEHELPER_GLOBAL_H
#define SCHEMEHELPER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCHEMEHELPER_LIBRARY)
#define SCHEMEHELPER_EXPORT Q_DECL_EXPORT
#else
#define SCHEMEHELPER_EXPORT Q_DECL_IMPORT
#endif

enum ColorScheme : int {
  Light = 1,
  Dark = 2,
};

#endif // SCHEMEHELPER_GLOBAL_H
