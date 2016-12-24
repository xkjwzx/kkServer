#include"def.h"
#include "server.h"

#include<QCryptographicHash>

#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>

#include"unit.h"

int Server::initRedis(){
    //redis默认监听端口为6387，可以在配置文件中修改
    redisc = redisConnect("127.0.0.1", 6379);
    if ( redisc->err)
    {
            redisFree(redisc);
            printf("Connect to redisServer faile\n");
            return 1;
    }
    printf("Connect to redisServer Success\n");
    return 0;

}

Server::Server(QObject *parent) : QObject(parent)
{
    m_db=QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("car1");//
    if(!m_db.open()){
        qDebug()<<"open db error.";
        exit(0);
    }

    //initial redis
    if(0!=initRedis()){
          qDebug()<<"initRedis error.";
            exit(0);
    }


    server=new HttpServer;
    connect(server,&HttpServer::requestReady,
            this,&Server::slotRequestReady);
    if(server->listen( QHostAddress::Any,8808))   //开始监听
    {
        qDebug()<<"listen ok";

    }else{
        qDebug()<<"listen error";
    }

}

//负责处理客户端发送过来的命令。
QByteArray Server::handle(QByteArray& request)
{
    QJsonDocument doc=QJsonDocument::fromJson(request);// 个json报文
    QJsonObject obj=doc.object();

    //提取命令
    int cmd=obj.value(KK_CMD).toInt();
    QJsonObject resp;
    resp.insert(KK_RESULT,QString(KK_ERR));
    resp.insert(KK_REASON,QString("unknown cmd"));//默认是未知命令，做错误返回

    //分析命令
    switch(cmd){
    case MKK_REG:{
        resp=Reg(obj);
        }
        break;
    case MKK_LOGIN:{
        resp=Login(obj);
        }
        break;
    case MKK_UPDATEPOS:{
        resp=UpdatePos(obj);
        }
        break;
    case MKK_ORDERREQUEST:
        resp=Request(obj);
        break;
    case MKK_UPDATEORDER:
        resp=UpdateOrder(obj);
        break;
    case MKK_ORDER:
        resp=Order(obj);
        break;
    case MKK_PP:
        resp=Pp(obj);
        break;
    case MKK_UPDATESTATUS:
        resp=UpdateStatus(obj);
        break;
    default:
        break;
    }

    //返回处理结果
    return QJsonDocument(resp).toJson();
}


QJsonObject Server::UpdateStatus(QJsonObject &req){

}

QJsonObject Server::Pp(QJsonObject &req){

}

QJsonObject Server::Order(QJsonObject &req){

}

QJsonObject Server::UpdateOrder(QJsonObject &req){
    QString username=req.value(KK_USERNAME).toString();
    QString orderid=req.value(KK_ORDERID).toString();
    int status=req.value(KK_UPDATESTATUS).toInt();

    switch(status){
    case 1://乘客发送约车请求
        break;

    case 2://司机已经接单
        break;
    case 3://乘客已经上车状态
        break;
    case 4://fini
        break;
    default:
        break;
    }

    //这时不太会失败，直接返回ok
    QJsonObject resp;
      resp.insert(KK_CMD,QString(KK_UPDATEORDER));
    resp.insert(KK_RESULT,QString(KK_OK));
    return resp;

}

QJsonObject Server::Request(QJsonObject &req){
    double lat1=req.value(KK_LAT1).toDouble();
    double lng1=req.value(KK_LNG1).toDouble();
    double lat2=req.value(KK_LAT2).toDouble();
    double lng2=req.value(KK_LNG2).toDouble();
    QString geohash=req.value(KK_GEOHASH).toString();
    QString tel=req.value(KK_TEL).toString();
    QString username=req.value(KK_USERNAME).toString();

    /*
响应，成功的时候
{
    cmd:”orderrequest”,
    result:”ok”,
    username:”driver username”,
    carid:”carid”,
    tel:”driver tel”
    orderid:111111
    lat:12.12
    lng:23
}

失败，附近没有车(5公里意外)
{
    cmd:”orderrequest”,
    result:”err”,
    reason:”no car in near”
}
*/

    QJsonObject resp;
      resp.insert(KK_CMD,QString(KK_ORDERREQUEST));
    resp.insert(KK_RESULT,QString(KK_ERR));
    resp.insert(KK_REASON,QString("NO CAR IN NEAR"));
    return resp;

}

QJsonObject Server::UpdatePos(QJsonObject &req){
    QString username=req.value(KK_UPDATEPOS).toString();
    double lat=req.value(KK_LAT).toDouble();
    double lng=req.value(KK_LNG).toDouble();
    QString geohash=req.value(KK_GEOHASH).toString();

    // ###司机更新位置
    /*

    客户端发送请求给服务器：位置更新请求
    服务器回应给客户端：位置更新响应

    请求报文
    –--json
    {
        cmd:”updatepos”
        username:”username”,
        lat:
        lng:
        geohash:
    }

    响应报文
    {
        cmd:”updatepos”
        result:”ok”
    }
    失败
    {
        cmd:”updatepos”
        result:”err”		//半个小时过后，没网
        reason:”session time out”
    }

    有订单
    json
    {
        cmd:”order”,
        tel:”1334324324324”
        lat:
        lng:
        orderid:”45234543254325”
    }

    已经有订单,乘客和司机实时更新位置给对方。
    ----json
    {
        cmd:”pp”,	//personal position
        lat:12.32,
        lng:12.32
    }
    */
    //////////////////////////////////////////////
    //乘客更新位置
    /*

乘客发送请求给服务器：位置更新请求
服务器回应给乘客：位置更新响应

请求报文
–--json
{
    cmd:”updatepos”
    username:”username”,
    lat:
    lng:
    type:”passenger”
}

响应报文
{
    cmd:”updatepos”
    lat:	//对应订单的司机的位置，表示已经到哪了
    lng:
    result:”ok”
}
失败
{
    cmd:”updatepos”
    result:”err”		//半个小时过后，没网
    reason:”session time out”
}
    */

    QJsonObject resp;
      resp.insert(KK_CMD,QString(KK_UPDATEPOS));
    resp.insert(KK_RESULT,QString(KK_ERR));
    resp.insert(KK_REASON,QString("session time out"));
    return resp;
}

/*司机/乘客注册
1请求报文
{
    cmd:”reg”
    username:”username”
    password:”password”
}

相应报文
{
    cmd:”reg”
    result:”ok”
}

{
    cmd:”reg”,
    result:”err”,
    reason:”xxxxxx”
}

*/
QJsonObject Server::Reg(QJsonObject &req)
{
    QString username=req.value(KK_USERNAME).toString();
    QString password=req.value(KK_PASSWORD).toString();


    //change to md5 128bit=16bytes
    QByteArray bb;
    bb=QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5);
    QString md5=bb.toHex();

    //execute the db
    //insert into db
    QString sql=QString("insert into user (username,password) values('%1','%2')").arg(username,md5);
    QSqlQuery query=m_db.exec(sql);
    QSqlError error=m_db.lastError();

    //response
    if(error.type()==QSqlError::NoError){
        QJsonObject resp;
        resp.insert(KK_RESULT,QString(KK_OK));
        return resp;
    }

    QJsonObject resp;
    resp.insert(KK_RESULT,QString(KK_ERR));
    resp.insert(KK_REASON,error.text());
    return resp;
}


/*
请求报文
{
    cmd:”login”,
    username:”username”,
    password:”password”,
    lat:”29.435345”,
    lng:”119.23432”,
    type:”driver”
}

相应报文
{
    cmd:”login”
    result:”ok”
}

{
    cmd:”login”,
    result:”err”,
    reason:”xxxxxx”
}
*/
typedef struct driver{
    bool isFree;
    double lat;
    double lng;
    uint64_t Geohash;
    QString tel;
    QString carNo;
    QString sessionTime;

}driver;

int Server::saveDriverStatus(bool isFree,double lat,double lng,uint64_t geoh,QString tel,QString carNo,QString sessionTime){
	

}

 int Server::savePassengerStatus(double lat,double lng,uint64_t geoh,QString tel,QString sessionTime){


 }

QJsonObject Server::Login(QJsonObject &req)
{
    QString username=req.value(KK_USERNAME).toString();
    QString password=req.value(KK_PASSWORD).toString();
    double lat=req.value(KK_LAT).toDouble();
    double lng=req.value(KK_LNG).toDouble();
    QString type=req.value(KK_TYPE).toString();

    if(!type.compare("driver") ){//driver
        //save driver status in redis


/*
服务器的处理：
需要在内存中（redis）保存司机的状态，

状态量|	意义
是否空闲	初始化“闲”
纬度	初始化时登录携带的位置信息
经度
GeoHash值（可以简化服务器的压力）
手机号码		约车用
车牌			约车用
session更新时间
*/
        uint64_t tmp=geohash(lat,lng,20);
        //read from db
        //手机号码		约车用
        QString tel="13512312312";
        //车牌			约车用
        QString carNo="232323";
        QString sessionTime="2016-12-22";

        saveDriverStatus(true,lat,lng,tmp,tel,carNo,sessionTime);//using redis

    }else if(!type.compare("passenger") ){//passenger

/*
服务器的处理：
需要在内存中（redis）保存乘客的状态，

状态量|	意义
纬度	初始化时登录携带的位置信息
经度
GeoHash值（可以简化服务器的压力）
手机号码	约车用
session更新时间
*/
        uint64_t tmp=geohash(lat,lng,20);
        //read from db
        //手机号码		约车用
        QString tel="13512312312";
               QString sessionTime="2016-12-22";

        savePassengerStatus(lat,lng,tmp,tel,sessionTime);//using redis
    }






    QJsonObject resp;
    resp.insert(KK_RESULT,QString(KK_OK));
    return resp;
}

void Server::slotRequestReady(HttpServerRequest &request, HttpServerResponse &response)
{
    //不是获取post数据，只是收到请求行和头

    //emit signal end when post data has transmit
    connect(&request,&HttpServerRequest::end,[&](){
        QByteArray buf=request.readBody();
        QByteArray resp=handle(buf);

#if 0
        qDebug()<<buf;
        //响应客户端
#endif
        response.writeHead(HttpResponseStatus::OK);  //200 ok
        response.end("i have receive your request.");  //content
    });
}
