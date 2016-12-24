SOURCES += \
    main.cpp \
    server.cpp \
    log.cpp

HEADERS += \
    server.h \
    def.h \
    unit.h \
    log.h

LIBS += \
    -ltufao1 \
    -lhiredis
QT += network \
    sql
QMAKE_CXXFLAGS+= -std=c++11
