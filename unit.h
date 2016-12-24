#ifndef UNIT_H
#define UNIT_H

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

#endif // UNIT_H
