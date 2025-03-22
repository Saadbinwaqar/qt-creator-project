QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminlogin1.cpp \
    adminmenu.cpp \
    cv.cpp \
    description.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    recruiterlogin.cpp \
    recruitermenu.cpp \
    signup.cpp \
    skillswindow.cpp \
    studentmenu.cpp \
    studentmenu1.cpp

HEADERS += \
    adminlogin1.h \
    adminmenu.h \
    cv.h \
    description.h \
    login.h \
    mainwindow.h \
    recruiterlogin.h \
    recruitermenu.h \
    signup.h \
    skillswindow.h \
    studentmenu.h \
    studentmenu1.h

FORMS += \
    adminlogin1.ui \
    adminmenu.ui \
    cv.ui \
    description.ui \
    login.ui \
    mainwindow.ui \
    recruiterlogin.ui \
    recruitermenu.ui \
    signup.ui \
    skillswindow.ui \
    studentmenu.ui \
    studentmenu1.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
