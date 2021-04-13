#include "serverwidget.h"
#include "ui_serverwidget.h"
#define BFSIZE 1024

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
    , server(nullptr)
    , socket(nullptr)
    , fileSize(0)
    , sendSize(0)
    , timer(this)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);
    server->listen(QHostAddress::Any, 8888);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->fileButton->setEnabled(false);
    ui->sendButton->setEnabled(false);

    connect(server, &QTcpServer::newConnection, [=](){
        QTcpSocket* last_socket = socket;
        socket = server->nextPendingConnection();
        delete last_socket;
        ui->fileButton->setEnabled(true);
    });
    connect(socket, &QTcpSocket::disconnected, [=](){
        ui->textBrowser->append("Connect close.");
        socket->disconnectFromHost();
        socket->close();
    });
    connect(ui->fileButton, &QPushButton::clicked, this, &ServerWidget::chooseFile);
    connect(ui->sendButton, &QPushButton::clicked, this, &ServerWidget::sendFile);
    connect(&timer, &QTimer::timeout, this, &ServerWidget::sendData);
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::chooseFile()
{
    QString filepath = QFileDialog::getOpenFileName(this, "choose a file", "../");
    file.setFileName(filepath);
    QFileInfo fInfo(file);
    fileName = fInfo.fileName();
    fileSize = fInfo.size();
    if(fileSize == 0)
    {
        ui->textBrowser->append("This file can not be open.");
    }
    else
    {
        auto status = file.open(QIODevice::ReadOnly);
        if(!status)
        {
            ui->textBrowser->append("Open file failed.");
        }
        else
        {
            ui->sendButton->setEnabled(true);
        }
    }
}
void ServerWidget::sendFile()
{
    ui->sendButton->setEnabled(false);
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    socket->write(head.toUtf8());
    timer.start(200);
}

void ServerWidget::sendData()
{
    timer.stop();
    sendSize = 0;
    ui->progressBar->setRange(0, fileSize);
    ui->progressBar->setValue(0);
    char buffer[BFSIZE] = {0};
    quint64 sendLen = 0;
    do
    {
        sendLen = file.read(buffer, BFSIZE);
        sendSize += sendLen;
        socket->write(buffer, sendLen);
        ui->progressBar->setValue(sendSize);
        memset(buffer, 0, BFSIZE);
    }while(sendLen > 0);
    ui->textBrowser->append("Send file success.");
    ui->textBrowser->append(QString("senSize:%1, fileSize:%2").arg(sendSize).arg(fileSize));
}
