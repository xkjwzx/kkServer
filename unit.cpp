#include"unit.h"
#include"def.h"
//bits迭代次数，20次就40位
uint64_t geohash(double lat,double lng,int bits){
    uint64_t lngLatBits = 0;

    double lngMin = -180;
    double lngMax = 180;
    double latMin = -90;
    double latMax = 90;


    for(int i=0; i<bits; ++i)
    {
        lngLatBits<<=1;

        double lngMid = (lngMax + lngMin)/2;
        if(lng > lngMid)
        {
            lngLatBits += 1;
            lngMin = lngMid;
        }
        else
        {
            lngMax = lngMid;
        }

        lngLatBits <<=1;

        double latMid = (latMax + latMin)/2;
        if(lat > latMid)
        {
            lngLatBits += 1;
            latMin = latMid;
        }
        else
        {
            latMax = latMid;
        }
    }
    return lngLatBits;
}

int q2i(QString &str)
{
    if(!str.compare(KK_REG)){
        return MKK_REG;
    }
    if(!str.compare(KK_LOGIN)){
        return MKK_LOGIN;
    }
    if(!str.compare(KK_UPDATEPOS)){
        return MKK_UPDATEPOS;
    }
    if(!str.compare(KK_ORDERREQUEST)){
        return MKK_ORDERREQUEST;
    }
    if(!str.compare(KK_UPDATEORDER)){
        return MKK_UPDATEORDER;
    }
    if(!str.compare(KK_ORDER)){
        return MKK_ORDER;
    }
    if(!str.compare(KK_PP)){
        return MKK_PP;
    }
    if(!str.compare(KK_UPDATESTATUS)){
        return MKK_UPDATESTATUS;
    }
    return -1;

}
