#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
    , recvSize(0)
    , fileSize(0)
    , readStatus(true)
{
    ui->setupUi(this);
    ui->ipLine->setText("127.0.0.1");
    ui->portLine->setText("8888");
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    socket = new QTcpSocket(this);
    connect(ui->coonButton, &QPushButton::clicked, this, &ClientWidget::makeConnect);
    connect(socket, &QTcpSocket::readyRead, this, &ClientWidget::readFile);
    connect(socket, &QTcpSocket::connected, [=](){
       ui->textBrowser->append("Connect success.");
    });
    connect(socket, &QTcpSocket::disconnected, [=](){
        ui->textBrowser->append("Connect closed.");
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::makeConnect()
{
    QString ip = ui->ipLine->text();
    quint16 port = ui->portLine->text().toUInt();
    socket->connectToHost(ip, port);
}

void ClientWidget::readFile()
{
    if(readStatus)
    {
        readStatus = false;
        recvSize = 0;
        QString head = socket->readAll();
        QString fileName = head.section("##", 0, 0);
        fileSize = head.section("##", 1, 1).toUInt();
        file.setFileName(fileName);
        ui->progressBar->setRange(0, fileSize);
        ui->progressBar->setValue(0);
        if(!file.open(QIODevice::WriteOnly))
        {
            ui->textBrowser->append("Failed to create file.");
            ui->textBrowser->append("For safe, we close your connect with your server.Please check your disk memory or if your have write permission.");
            socket->close();
        }
    }
    else
    {
        if(file.isOpen())
        {
            QByteArray buffer = socket->readAll();
            auto len = file.write(buffer);
            if(len < 0)
            {
                ui->textBrowser->append("Error: write file error.For safe, we closed the connect.If you want to try again, you can connect again.");
                socket->disconnectFromHost();
                socket->close();
                file.close();
            }
            else
            {
                ui->textBrowser->append(QString("len:%1").arg(len));
                recvSize += len;
                ui->progressBar->setValue(recvSize);
                if(recvSize == fileSize)
                {
                    readStatus = true;
                    ui->textBrowser->append("File recieve success.");
                    file.close();
                }
                else
                {
                    ui->textBrowser->append(QString("recvSize:%1, fileSize:%2").arg(recvSize).arg(fileSize));
                }
            }
        }
        else
        {
            ui->textBrowser->append("File close.");
        }
    }
}
