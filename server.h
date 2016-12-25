#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include<tufao-1/Tufao/HttpServer>
#include<tufao-1/Tufao/HttpServerRequest>
#include<tufao-1/Tufao/HttpServerResponse>

#include<QJsonObject>
#include<QJsonValue>
#include<QJsonDocument>
#include<QJsonArray>





using namespace Tufao;

class Server : public QObject
{
    Q_OBJECT
public:

    explicit Server(QObject *parent = 0);

    HttpServer* server;

    QByteArray handle(QByteArray&);
    QJsonObject Reg(QJsonObject& req);
    QJsonObject Login(QJsonObject& req);

    QJsonObject UpdateStatus(QJsonObject &req);
    QJsonObject Pp(QJsonObject &req);

    QJsonObject Order(QJsonObject &req);
    QJsonObject UpdateOrder(QJsonObject &req);
    QJsonObject Request(QJsonObject &req);
    QJsonObject UpdatePos(QJsonObject &req);



signals:

public slots:
    void slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&);
};

#endif // SERVER_H
