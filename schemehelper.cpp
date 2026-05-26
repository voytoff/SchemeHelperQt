#include "schemehelper.h"

#include <QFile>
#include <QGuiApplication>
#include <QStyleHints>
#include <QRegularExpression>
#include <QSvgRenderer>
#include <QPainter>

#include <windows.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib") // MSVC Only

SchemeHelper::SchemeHelper(QMainWindow *wnd, const QString windowIcon) :
  wnd(wnd),
  windowIcon(windowIcon) {}

QAction *SchemeHelper::create(const QString &text, const QString iconPath, QKeySequence shortcut) {
  auto *action = new Action(wnd, text, iconPath, shortcut);
  actions.append(action);
  //if (!shortcut.isEmpty()) action->action->setShortcut(shortcut);
  return action->action;
}

QAction *SchemeHelper::createLightAction(const QString &text, const QString iconPath, QKeySequence shortcut) {
  lightAction = create(text, iconPath, shortcut);
  QMainWindow::connect(lightAction, &QAction::triggered, wnd, [=]() { applayColorScheme(ColorScheme::Light); });
  return lightAction;
}

QAction *SchemeHelper::createDarkAction(const QString &text, const QString iconPath, QKeySequence shortcut) {
  darkAction = create(text, iconPath, shortcut);
  QMainWindow::connect(darkAction, &QAction::triggered, wnd, [=]() { applayColorScheme(ColorScheme::Dark); });
  return darkAction;
}

QIcon SchemeHelper::getIcon(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
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

void SchemeHelper::setDarkTitleBar(bool dark) {
  BOOL value = dark ? TRUE : FALSE;
  DwmSetWindowAttribute(reinterpret_cast<HWND>(wnd->winId()), 20, &value, sizeof(value));
}

void SchemeHelper::setIcons() {
  wnd->setWindowIcon(getIcon(windowIcon));
  foreach (auto action, actions) {
    action->action->setIcon(getIcon(action->iconPath));
  }
}

void SchemeHelper::applayColorScheme(ColorScheme scheme) {
  QGuiApplication::styleHints()->setColorScheme((Qt::ColorScheme)scheme);
  setIcons();
  bool dark = scheme == ColorScheme::Dark;
  lightAction->setVisible(dark);
  darkAction->setVisible(!dark);

}

void SchemeHelper::applayColorDark()
{

}
