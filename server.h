#ifndef SERVER_H
#define SERVER_H

#include<QtSql/QSqlDatabase>

#include <QObject>
#include<tufao-1/Tufao/HttpServer>
#include<tufao-1/Tufao/HttpServerRequest>
#include<tufao-1/Tufao/HttpServerResponse>

#include<QJsonObject>
#include<QJsonValue>
#include<QJsonDocument>
#include<QJsonArray>

#include <hiredis/hiredis.h>



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


    int initRedis();

    int saveDriverStatus(bool isFree,double lat,double lng,uint64_t geoh,QString tel,QString carNo,QString sessionTime);
   int savePassengerStatus(double lat,double lng,uint64_t geoh,QString tel,QString sessionTime);

private:
   QSqlDatabase m_db;
   redisContext* m_redisc;
   redisReply *m_reply;

signals:

public slots:
    void slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&);
};

#endif // SERVER_H
