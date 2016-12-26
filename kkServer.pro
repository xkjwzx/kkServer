SOURCES += \
    main.cpp \
    server.cpp \
    log.cpp \
    order.cpp \
    dbop.cpp \
    unit.cpp

HEADERS += \
    server.h \
    def.h \
    unit.h \
    log.h \
    order.h \
    dbop.h

LIBS += \
    -ltufao1 \
    -lhiredis
QT += network \
    sql
QMAKE_CXXFLAGS+= -std=c++11
