SOURCES += \
    main.cpp \
    server.cpp

HEADERS += \
    server.h \
    def.h \
    unit.h

LIBS += \
    -ltufao1 \
    -lhiredis
QT += network \
    sql
QMAKE_CXXFLAGS+= -std=c++11
