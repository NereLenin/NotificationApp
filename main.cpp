#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringListModel>
#include <QQmlContext>
#include "src/notification.h"
#include "src/notificationnetworkmanager.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/MainNotification.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

   NotificationNetworkManager mainManager("http://notifications.ru");
   QList<Notification> notificationList;
   
   bool isAvailible = mainManager.isServerAccessible()
   
    mainManager.readAllNotifications(notificationList);


	engine.rootContext()->setContextProperty("notificationModel", isAvailible);
    engine.rootContext()->setContextProperty("notificationModel", QVariant::fromValue(notificationsList));
    engine.load(url);


    return app.exec();
}
