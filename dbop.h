#ifndef DBOP_H
#define DBOP_H

#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>

#include <hiredis/hiredis.h>

typedef struct DriverInfo{
    QString name;
    bool isFree;
    double lat;
    double lng;
    uint64_t geoh;
    QString tel;
    QString carNo;
    QString sessionTime;

}DriverInfo;
typedef struct PassengerInfo{
    QString name;
    double lat;
    double lng;
    uint64_t geoh;
    QString tel;
    QString sessionTime;
}PassengerInfo;

class dbop
{
public:
    dbop();
    static dbop* instance();
    int initRedis();
    int initdb();

    int closedb();
    int insertUserInfo(QString& err,QString& username,QString& md5,QString& tel);
    int readUserInfo(QString& err,QString& tel,QString& name,QString& dbPassWord);

 int saveDriverStatus(DriverInfo& driver);
 int savePassengerStatus(PassengerInfo& passenger);

private:
    QSqlDatabase m_db;
    redisContext* m_redisc;
    redisReply *m_reply;
};

#endif // DBOP_H
