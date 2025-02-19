QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TEMPLATE = app
RC_ICONS = icon.ico
DESTDIR = ./bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    applyfrienditem.cpp \
    applyfriendlist.cpp \
    applyfriendpage.cpp \
    authenfriend.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatitembase.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    chatview.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    clickedoncelabel.cpp \
    contactuserlist.cpp \
    conuseritem.cpp \
    customizededit.cpp \
    findfaildlg.cpp \
    findsuccessdlg.cpp \
    friendinfopage.cpp \
    friendlabel.cpp \
    global.cpp \
    grouptipitem.cpp \
    httpmgr.cpp \
    invaliditem.cpp \
    lineitem.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagetextedit.cpp \
    picturebubble.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    searchlist.cpp \
    statelabel.cpp \
    statewidget.cpp \
    tcpmgr.cpp \
    textbubble.cpp \
    timerbtn.cpp \
    userdata.cpp \
    usermgr.cpp

HEADERS += \
    adduseritem.h \
    applyfriend.h \
    applyfrienditem.h \
    applyfriendlist.h \
    applyfriendpage.h \
    authenfriend.h \
    bubbleframe.h \
    chatdialog.h \
    chatitembase.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    chatview.h \
    clickedbtn.h \
    clickedlabel.h \
    clickedoncelabel.h \
    contactuserlist.h \
    conuseritem.h \
    customizededit.h \
    findfaildlg.h \
    findsuccessdlg.h \
    friendinfopage.h \
    friendlabel.h \
    global.h \
    grouptipitem.h \
    httpmgr.h \
    invaliditem.h \
    lineitem.h \
    listitembase.h \
    loadingdlg.h \
    logindialog.h \
    mainwindow.h \
    messagetextedit.h \
    picturebubble.h \
    registerdialog.h \
    resetdialog.h \
    searchlist.h \
    singleton.h \
    statelabel.h \
    statewidget.h \
    tcpmgr.h \
    textbubble.h \
    timerbtn.h \
    userdata.h \
    usermgr.h

FORMS += \
    adduseritem.ui \
    applyfriend.ui \
    applyfrienditem.ui \
    applyfriendpage.ui \
    authenfriend.ui \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    conuseritem.ui \
    findfaildlg.ui \
    findsuccessdlg.ui \
    friendinfopage.ui \
    friendlabel.ui \
    grouptipitem.ui \
    lineitem.ui \
    loadingdlg.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc

DISTFILES += \
    config.ini


win32:CONFIG(debug, debug | release)
{
    TargetConfig = $${PWD}/config.ini
    TargetConfig = $$replace(TargetConfig, /, \\)

    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)

    # copy config.ini into bin folder
    QMAKE_POST_LINK += cp \"$$TargetConfig\" \"$$OutputDir\"

    StaticDir = $${PWD}/static
    StaticDir = $$replace(StaticDir, /, \\)

    # copy static folder into bin folder
    QMAKE_POST_LINK += mkdir \"$$OutputDir\\static\" &
    QMAKE_POST_LINK +=xcopy /E /I /Y \"$$StaticDir\" \"$$OutputDir\\static\\\"
}

# WIN32::CONFIG(debug, debug | release){
#     message("debug mode")
#     TargetConfig = $${PWD}/config.ini
#     TargetConfig = $$replace(TargetConfig, /, \\)

#     OutputDir =  $${OUT_PWD}/$${DESTDIR}
#     OutputDir = $$replace(OutputDir, /, \\)

#     # copy config.ini
#     QMAKE_POST_LINK += mkdir \"$$OutputDir\" &
#     QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"&

#     StaticDir = $${PWD}/static
#     StaticDir = $$replace(StaticDir, /, \\)

#     # copy config.ini into bin folder
#     QMAKE_POST_LINK += mkdir \"$$OutputDir\\static\" &
#     QMAKE_POST_LINK += copy \"$$StaticDir\" \"$$OutputDir\\static\\\"

# }
# # else{

# #     message("release mode")
# #     TargetConfig = $${PWD}/config.ini
# #     TargetConfig = $$replace(TargetConfig, /, \\)

# #     OutputDir =  $${OUT_PWD}/$${DESTDIR}
# #     OutputDir = $$replace(OutputDir, /, \\)

# #     QMAKE_POST_LINK += cmd /C mkdir \"$$OutputDir\" &
# #     QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\" &

# #     StaticDir = $${PWD}/static
# #     StaticDir = $$replace(StaticDir, /, \\)

# #     # copy config.ini into bin folder
# #     QMAKE_POST_LINK += mkdir \"$$OutputDir\\static\" &
# #     QMAKE_POST_LINK += xcopy /E /I /Y \"$$StaticDir\" \"$$OutputDir\\static\\\"
# # }

# win32-msvc*:QMAKE_CXXFLAGS += /wd4819 /utf-8
