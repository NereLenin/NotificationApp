#ifndef NOTIFICATIONNETWORKMANAGER_H
#define NOTIFICATIONNETWORKMANAGER_H
#include <QtNetwork>
#include "notification.h"
class NotificationNetworkManager: public QObject 
{
	Q_OBJECT
private:
    QString serverUrl;
    QNetworkAccessManager manager;
    QNetworkCookieJar cookieJar;
    QNetworkRequest request;
    QUrl CurrentUrl;


    QTimer ResponseTimer;

    QEventLoop ResponseLoop;


    bool CreateGetWaitiedResponse(QString url, QByteArray *GetReply=nullptr);
    bool CreatePostWaitiedResponse(QString url,QString data, QByteArray *PostReply=nullptr);

    bool ReadJsonAllNotifications(const QByteArray &pars,QList<Notification> &NotificationsList);
    void ReadJsonNotification(QJsonValue notification,Notification& WriteTo);
public:
    NotificationNetworkManager(QString ServerUrl);

    bool isServerAccessible();
    void readAllNotifications(QList<Notification> &notificationsList);
};

#endif // NOTIFICATIONNETWORKMANAGER_H
