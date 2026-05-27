#include "action.h"

Action::Action(QMainWindow *wnd, QString text, QString iconPath, QKeySequence shortcut)
  : action(new QAction(text, wnd))
  , iconPath(iconPath)
  , shortcut(shortcut)
{
  if (!shortcut.isEmpty())
    action->setShortcut(shortcut);
}
