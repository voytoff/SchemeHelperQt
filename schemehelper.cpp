#include "schemehelper.h"

#include <QFile>
#include <QGuiApplication>
#include <QPainter>
#include <QRegularExpression>
#include <QStyleHints>
#include <QSvgRenderer>

#if defined(Q_OS_WIN)
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib") // MSVC Only
#endif

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

SchemeHelper::SchemeHelper(QMainWindow *wnd, const QString windowIcon)
  : wnd(wnd)
  , windowIcon(windowIcon) {}

QAction *SchemeHelper::create(const QString &text, const QString iconPath, QKeySequence shortcut) {
  auto *action = new Action(wnd, text, iconPath, shortcut);
  actions.append(action);
  return action->action;
}

QAction *SchemeHelper::createLightAction(const QString &text, const QString iconPath, QKeySequence shortcut) {
  lightAction = create(text, iconPath, shortcut);
  QMainWindow::connect(lightAction, &QAction::triggered, wnd, [=]() {
    applayColorScheme(ColorScheme::Light);
  });
  return lightAction;
}

QAction *SchemeHelper::createDarkAction(const QString &text, const QString iconPath, QKeySequence shortcut) {
  darkAction = create(text, iconPath, shortcut);
  QMainWindow::connect(darkAction, &QAction::triggered, wnd, [=]() {
    applayColorScheme(ColorScheme::Dark);
  });
  return darkAction;
}

QIcon SchemeHelper::getIcon(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return QIcon(path);
  if (!path.endsWith(".svg"))
    return QIcon(path);

  QTextStream in(&file);
  QString content = in.readAll();
  file.close();
  auto scheme = QGuiApplication::styleHints()->colorScheme();
  auto color = scheme == Qt::ColorScheme::Dark ? fillDark : fillLight;
  content = changeFillSvg(content, QString("fill=\"%1\"").arg(color));
  return iconFromSvgString(content);
}

QString SchemeHelper::changeFillSvg(QString svg, QString fillColorHexText) {
  auto updatedSvg = svg.replace(QRegularExpression("fill=\"[^\"]*\""), fillColorHexText);
  return updatedSvg;
}

QIcon SchemeHelper::iconFromSvgString(const QString &svgString, int width, int height) {
  // 1. Prepare SVG data
  QByteArray byteArray = svgString.toUtf8();
  QSvgRenderer renderer(byteArray);
  // 2. Prepare a transparent Pixmap
  QPixmap pixmap(width, height);
  pixmap.fill(Qt::transparent);
  // 3. Paint the SVG onto the Pixmap
  QPainter painter(&pixmap);
  renderer.render(&painter);
  // 4. Return as QIcon
  return QIcon(pixmap);
}

#if defined(Q_OS_WIN)
void SchemeHelper::setDarkTitleBar(bool dark) {
  OSVERSIONINFOEX osvi;
  ZeroMemory(&osvi, sizeof(osvi));
  osvi.dwOSVersionInfoSize = sizeof(osvi);
  osvi.dwMajorVersion = 10;
  osvi.dwBuildNumber = 22000; // Build 22000 is Windows 11

  ULONGLONG conditionMask = 0;
  VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
  VER_SET_CONDITION(conditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);

  auto hwnd = reinterpret_cast<HWND>(wnd->winId());
  if (VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_BUILDNUMBER, conditionMask)) {
    BOOL preference = dark ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &preference, sizeof(preference));
  } else {
    qDebug() << "Windows 10 - Handle fallback";
    ShowWindow(hwnd, SW_MINIMIZE);
    ShowWindow(hwnd, SW_RESTORE);
  }
}
#endif

void SchemeHelper::setIcons() {
  wnd->setWindowIcon(getIcon(windowIcon));
  foreach (auto action, actions)
    action->action->setIcon(getIcon(action->iconPath));
}

void SchemeHelper::applayColorScheme(ColorScheme scheme, bool initialize) {
  QGuiApplication::styleHints()->setColorScheme((Qt::ColorScheme) scheme);
  setIcons();
  bool dark = scheme == ColorScheme::Dark;
#if defined(Q_OS_WIN)
  if (!initialize)
    setDarkTitleBar(dark);
#endif
  lightAction->setVisible(dark);
  darkAction->setVisible(!dark);
}

void SchemeHelper::setupToolbar(QToolBar *toolbar) {
  toolbar->setIconSize(QSize(16, 16));
  toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  toolbar->setStyleSheet("QToolButton {max-width:16px;max-height:16px;min-width:16px;min-height:16px;padding:1px;margin:1px;}");
}
