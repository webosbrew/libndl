#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtQml>
#include <QDebug>

#include "backend.h"

extern "C"
{
#include <NDL_directmedia.h>
#include <gst/gst.h>
}

static void ndlInitCb(char *data)
{
    qDebug("NDLInitCallback(%s)", data);
}

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);

    char appid[] = "org.webosbrew.sample.ndl-directmedia";

    if (NDL_DirectMediaInit(appid, ndlInitCb)) {
        qDebug() << NDL_DirectMediaGetError();
        return -1;
    }
    // Seems useful on 4K TVs
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qmlRegisterType<Backend>("Sample", 1, 0, "Backend");

    QGuiApplication app(argc, argv);
    app.setApplicationName(appid);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    
    int ret = app.exec();
    NDL_DirectMediaQuit();
    return ret;
}