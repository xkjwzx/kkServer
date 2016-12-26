
#include<QDebug>
#include"dbop.h"
dbop* dbop::dp=NULL;

dbop::dbop()
{

}

dbop::~dbop()
{
 if(dp)
     delete dp;

 if(m_redisc)
    redisFree(m_redisc);
}

dbop *dbop::instance()
{
    if(dp)return dp;
    dp=new dbop;
    return dp;
}

int dbop::initRedis(){
    //redis默认监听端口为6387，可以在配置文件中修改
    m_redisc = redisConnect("127.0.0.1", 6379);
    if ( m_redisc->err)
    {
            redisFree(m_redisc);
            printf("Connect to redisServer faile\n");
            return 1;
    }
    printf("Connect to redisServer Success\n");
    return 0;

}

int dbop::initdb(){

    m_db=QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("car1");//
    if(!m_db.open()){
        qDebug()<<"open db error.";
        exit(0);
    }
}

int dbop::closedb()
{
    m_db.close();
}

int dbop::insertUserInfo(QString& err,QString& username,QString& md5,QString& tel)
{
    //insert into db
    QString sql=QString("insert into user (username,password,tel) values('%1','%2','%3')").arg(username,md5,tel);
    QSqlQuery query=m_db.exec(sql);
    QSqlError error=m_db.lastError();

    //response
    if(error.type()==QSqlError::NoError){//ok
        return 0;
    }
    err=error.text();
    return 1;
}

/*
    hmset username:aaa status 1 lat 13.12232 lng 11.123213

    sadd 1100110111000111111 driver1
    sadd 1100110111000111111 driver2

    sget 1100110111000111111
       driver1
       driver2

   sismember 1100110111000111111 driver2

*/

int dbop::saveDriverStatus(DriverInfo& driver){
//QString& name,bool& isFree, double& lat, double& lng, uint64_t& geoh, QString& tel, QString& carNo, QString& sessionTime)

    //  redisContent* redis = ...;
         // hmset status: 1 username:aaa status 1 lat 13.12232 lng 11.123213
          //hmset driver1 isFree 1 lat 12.234 lng 32.234 tel 12312312321 carNo 556565 sessionTime 20161234
      /* Set a key */
         m_reply = (redisReply*)redisCommand(m_redisc,"hmset %s isFree %d lat %d lng %d \
geoh %ld tel %s carNo %s sessionTime %s",driver.name.toStdString().c_str(),1, driver.lat,driver.lng,\
driver.geoh,driver.tel.toStdString().c_str(),driver.carNo.toStdString().c_str(),driver.sessionTime.toStdString().c_str() );
         printf("hmset: %s\n", m_reply->str);
         freeReplyObject(m_reply);

         //sadd 54312 "drive2"
         m_reply = (redisReply*)redisCommand(m_redisc,"sadd %ld %s",driver.geoh,driver.name.toStdString().c_str());
         printf("sadd: %s\n", m_reply->str);
         freeReplyObject(m_reply);
}

int dbop::savePassengerStatus(PassengerInfo& passenger)
{
//        QString name=passenger.name;
//        double lat, double& lng, uint64_t& geoh, QString& tel, QString& sessionTime

    m_reply = (redisReply*)redisCommand(m_redisc,"hmset %s lat %d lng %d geoh %ld tel %s \
sessionTime %s",passenger.name.toStdString().c_str(), passenger.lat ,passenger.lng,\
 passenger.geoh,passenger.tel.toStdString().c_str(),passenger.sessionTime.toStdString().c_str());
    printf("passenger hmset: %s\n", m_reply->str);
    freeReplyObject(m_reply);

    m_reply = (redisReply*)redisCommand(m_redisc,"sadd %ld %s",passenger.geoh,passenger.name.toStdString().c_str());
    printf("passenger sadd: %s\n", m_reply->str);
    freeReplyObject(m_reply);
}

int dbop::readUserInfo(QString& err,QString& tel,QString& name,QString& dbPassWord)
{
    QString sql=QString("select '%1' '%2' from tuser where tel='%3'").arg(name,dbPassWord,tel);
    QSqlQuery query(sql);
    query.exec();

    while(query.next())
    {
        name=query.value("name").toString();
        dbPassWord=query.value("passwrod").toString();
    }

    //response
    QSqlError error = m_db.lastError();
    if(error.type()==QSqlError::NoError){//ok
        return 0;
    }
    err=error.text();
    return 1;
}
