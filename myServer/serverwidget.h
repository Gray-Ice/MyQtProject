#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
    void sendFile();
    void chooseFile();
    void sendData();

private:
    Ui::ServerWidget *ui;
    QTcpServer* server;
    QTcpSocket* socket;
    QString fileName;
    QFile file;
    quint64 fileSize;
    quint64 sendSize;
    QTimer timer;
};
#endif // SERVERWIDGET_H
