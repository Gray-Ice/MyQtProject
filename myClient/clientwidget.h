#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H
#define BFSIZE 1024

#include <QWidget>
#include <QFile>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();
    void makeConnect();
    void readFile();

private:
    Ui::ClientWidget *ui;
    quint64 recvSize;
    quint64 fileSize;
    QFile file;
    QTcpSocket* socket;
    bool readStatus;
};
#endif // CLIENTWIDGET_H
