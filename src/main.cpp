#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtQml>
#include <QDebug>

#include "backend.h"

extern "C"
{
#include <gst/gst.h>
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    char appid[] = "org.webosbrew.sample.ndl-directmedia";

    // Seems useful on 4K TVs
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qmlRegisterType<Backend>("Sample", 1, 0, "Backend");

    QGuiApplication app(argc, argv);
    app.setApplicationName(appid);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    
    return app.exec();
}