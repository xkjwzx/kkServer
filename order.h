#ifndef ORDER_H
#define ORDER_H


class Order
{
public:
    Order();
    static Order* instance();
    bool checkOrder();
};

#endif // ORDER_H
