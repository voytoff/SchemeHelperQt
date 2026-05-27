#ifndef SCHEMEHELPER_H
#define SCHEMEHELPER_H

#include "SchemeHelper_global.h"
#include "action.h"

#include <QMainWindow>

class SCHEMEHELPER_EXPORT SchemeHelper
{
public:
  SchemeHelper(QMainWindow *wnd, const QString windowIcon);

  QAction *create(const QString &text, const QString iconPath = nullptr, QKeySequence shortcut = QKeySequence());
  QAction *createLightAction(const QString &text, const QString iconPath = nullptr, QKeySequence shortcut = QKeySequence());
  QAction *createDarkAction(const QString &text, const QString iconPath = nullptr, QKeySequence shortcut = QKeySequence());
  void setIcons();
  void applayColorScheme(ColorScheme scheme, bool initialize = false);

protected:
  const QString fillLight = "#1f1f1f";
  const QString fillDark = "#e3e3e3";

  QMainWindow *wnd;
  QList<Action *> actions;
  QString windowIcon;

  QAction *lightAction;
  QAction *darkAction;

  QIcon getIcon(const QString &path);
  QString changeFillSvg(QString svg, QString fillColorHexText);
  QIcon iconFromSvgString(const QString &svgString, int width = 24, int height = 24);
  void setDarkTitleBar(bool dark);
};

#endif // SCHEMEHELPER_H
