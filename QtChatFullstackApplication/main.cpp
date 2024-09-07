#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include "global.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFile qss(":/style/stylesheet.qss");
    if (qss.open(QFile::ReadOnly)) {
        qDebug("Open success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    } else {
        qDebug("Open failed");
    }

    QString fileName = "config.ini";
    QString appPath = QCoreApplication::applicationDirPath();
    QString configPath = QDir::toNativeSeparators(appPath + QDir::separator() + fileName);

    QSettings settings(configPath, QSettings::IniFormat);
    QString gateHost = settings.value("GateServer/host").toString();
    QString gatePort = settings.value("GateServer/port").toString();

    gate_url_prefix = "http://" + gateHost + ":" + gatePort;

    qDebug() << gate_url_prefix;

    MainWindow w;
    w.show();
    return a.exec();
}
