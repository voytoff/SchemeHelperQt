#ifndef ACTION_H
#define ACTION_H

#include <QAction>
#include <QMainWindow>

class Action
{
public:
  Action(QMainWindow* wnd, QString text, QString iconPath = nullptr, QKeySequence shortcut = QKeySequence());
  QAction *action;
  QString iconPath;
  QKeySequence shortcut;
};

#endif // ACTION_H
