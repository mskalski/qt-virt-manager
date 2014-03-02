#include "log_doc.h"

LogDocContent::LogDocContent(QWidget *parent) :
    QWidget(parent)
{
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    docLayout = new QGridLayout();

    Log = new QTextEdit(this);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
    Log->setReadOnly(true);
    Log->setContextMenuPolicy(Qt::DefaultContextMenu);   //Qt::CustomContextMenu);
    //connect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));

    docLayout->addWidget(Log, 2, 0, 3, 2);
    setLayout(docLayout);
}
LogDocContent::~LogDocContent()
{
    disconnect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));
    Log->clear();
    delete Log;
    Log = 0;
    delete docLayout;
    docLayout = 0;
}
void LogDocContent::appendErrorMsg(QString &msg)
{
    if ( Log->toPlainText().count()>LOG_SIZE ) Log->clear();
    Log->append(msg);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
}
