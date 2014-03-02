#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QMenu>
#include <QDebug>

class ToolBar : public QToolBar
{
  Q_OBJECT
public:
  ToolBar(QWidget *parent);
  ~ToolBar();

  QAction    *_hideAction;
  QAction    *_createAction;
  QAction    *_editAction;
  QAction    *_deleteAction;
  QAction    *_openAction;
  QAction    *_showAction;
  QAction    *_closeAction;
  QAction    *_closeAllAction;
  QAction    *_docsUpAction;
  QAction    *_exitAction;

  QAction    *_logUpAction;
  QAction    *_domUpAction;
  QAction    *_netUpAction;
  QAction    *_stVolUpAction;
  QAction    *_stPoolUpAction;

signals:
  void        warningShowed();

private:
  QAction    *itemControlAction;
  QMenu      *itemControlMenu;

  QMenu      *showDocsControlMenu;

private slots:
  void initActions();
  void addItemControlMenu();
  void addDocsControlMenu();
  void mainWindowUp();
  void showHoveredMenu();

public slots:
  Qt::ToolBarArea get_ToolBarArea(int) const;
};
#endif   // TOOLBAR_H
