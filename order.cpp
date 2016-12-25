#include "order.h"
#include<stddef.h>
Order::Order()
{

}

Order *Order::instance()
{
   static Order* one=NULL;
    if(one)return one;
    one=new Order;
    return one;
}
