#include "notificationnetworkmanager.h"

NotificationNetworkManager::NotificationNetworkManager(QString serverUrl)
{
    this->serverUrl = serverUrl;
    ResponseTimer.setInterval(5000);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    manager.setCookieJar(&cookieJar);
}


bool NotificationNetworkManager::CreateGetWaitiedResponse(QString url,QByteArray *GetReply)
{
    CurrentUrl.setUrl(url);
    request.setUrl(CurrentUrl);


    QObject::connect(&manager, &QNetworkAccessManager::finished,
            &ResponseLoop, &QEventLoop::quit);

    QObject::connect(&ResponseTimer, &QTimer::timeout,
           &ResponseLoop, &QEventLoop::quit);


    ResponseTimer.start();

    QObject::connect(&ResponseTimer, &QTimer::timeout,
           &ResponseTimer, &QTimer::stop);

    QNetworkReply *current_reply = manager.get(request);

    ResponseLoop.exec();//waiting

    if(ResponseTimer.remainingTime() <= 0)
    {
        qDebug() << "Ошибка сети. Ошибка периода ожидания ответа на запрос.";

        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                &ResponseLoop, &QEventLoop::quit);

        QObject::disconnect(&ResponseTimer, &QTimer::timeout,
               &ResponseTimer, &QTimer::stop);

        QObject::disconnect(&ResponseTimer, &QTimer::timeout,
               &ResponseLoop, &QEventLoop::quit);


        return false;
    }

    if(current_reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Ошибка подключения к сети";
        delete current_reply;

        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                   &ResponseLoop, &QEventLoop::quit);
        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                &ResponseLoop, &QEventLoop::quit);

        QObject::disconnect(&ResponseTimer, &QTimer::timeout,
               &ResponseTimer, &QTimer::stop);

        return false;
    }


    if(GetReply != nullptr)
        *GetReply = current_reply->readAll();


    if(current_reply!=nullptr) delete current_reply;


    QObject::disconnect(&manager, &QNetworkAccessManager::finished,
               &ResponseLoop, &QEventLoop::quit);
    QObject::disconnect(&manager, &QNetworkAccessManager::finished,
            &ResponseLoop, &QEventLoop::quit);

    QObject::disconnect(&ResponseTimer, &QTimer::timeout,
           &ResponseTimer, &QTimer::stop);

    return true;
}


bool NotificationNetworkManager::CreatePostWaitiedResponse(QString url,QString data,QByteArray *PostReply)
{
    CurrentUrl.setUrl(url);
    request.setUrl(CurrentUrl);


    QObject::connect(&manager, &QNetworkAccessManager::finished,
            &ResponseLoop, &QEventLoop::quit);

    QObject::connect(&ResponseTimer, &QTimer::timeout,
           &ResponseLoop, &QEventLoop::quit);

    ResponseTimer.start();

    QObject::connect(&ResponseTimer, &QTimer::timeout,
           &ResponseTimer, &QTimer::stop);

    QByteArray PostData = QByteArray::fromStdString(data.toStdString());
    QNetworkReply *current_reply = manager.post(request,PostData);

    ResponseLoop.exec();//waiting

    if(ResponseTimer.remainingTime() <= 0)
    {
        qDebug() << "Ошибка сети. Ошибка периода ожидания ответа на запрос.";
        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                   &ResponseLoop, &QEventLoop::quit);
        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                &ResponseLoop, &QEventLoop::quit);

        QObject::disconnect(&ResponseTimer, &QTimer::timeout,
               &ResponseTimer, &QTimer::stop);

        return false;
    }
    if(current_reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Ошибка подключения к сети";
        delete current_reply;

        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                   &ResponseLoop, &QEventLoop::quit);
        QObject::disconnect(&manager, &QNetworkAccessManager::finished,
                &ResponseLoop, &QEventLoop::quit);

        QObject::disconnect(&ResponseTimer, &QTimer::timeout,
               &ResponseTimer, &QTimer::stop);

        return false;
    }

    if(PostReply != nullptr)
        *PostReply = current_reply->readAll();

    if(current_reply!= nullptr) delete current_reply;

    QObject::disconnect(&manager, &QNetworkAccessManager::finished,
               &ResponseLoop, &QEventLoop::quit);
    QObject::disconnect(&manager, &QNetworkAccessManager::finished,
            &ResponseLoop, &QEventLoop::quit);

    QObject::disconnect(&ResponseTimer, &QTimer::timeout,
           &ResponseTimer, &QTimer::stop);

    return true;
}

bool NotificationNetworkManager::isServerAccessible()
{
    return CreateGetWaitiedResponse(serverUrl);
}

void NotificationNetworkManager::ReadJsonNotification(QJsonValue notification, Notification& newNotification)
{
    if(notification.isNull())
        {
           qDebug() << "Пустое уведомление";
           return;
        }

    QJsonObject tempJsonNotification = notification.toObject();


    newNotification.setId(tempJsonNotification.value("id").toString().toInt());
    newNotification.setText(tempJsonNotification.value("text").toString());
    newNotification.setTime(QDateTime::fromString((tempJsonNotification.value("time").toString()), Qt::ISODate));

}

bool NotificationNetworkManager::ReadJsonAllNotifications(const QByteArray &pars, QList<Notification> &notificationsList)
{
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(pars,&error);

        if(error.error != QJsonParseError::NoError)
        {
            qDebug() << "Ошибка чтения ответа";
            qDebug() << error.errorString();
            return false;
        }

        QJsonArray jsonNotifications = doc.array();

        if(jsonNotifications.isEmpty())
        {
            qDebug() << "Список уведомлений пуст" << endl;
            return true;
        }

        for(int i=0; i< jsonNotifications.size();i++)
            {
                Notification *newNotification = new Notification;
                ReadJsonNotification(jsonNotifications[i],*newNotification);
                notificationsList.append(*newNotification);
            }
         return true;
}

void NotificationNetworkManager::readAllNotifications(QList<Notification> &notificationsList)
{
    QByteArray responseData;

    QString requestUrl = QString("%1/NotificationApi/allNotifications").arg(serverUrl);

    if(!CreateGetWaitiedResponse(requestUrl,&responseData))
        {
            qDebug() << "Ошибка получения диалогов. Проверте подключение к сети";
            return;
        }

    notificationsList.clear();


    if(!ReadJsonAllNotifications(responseData,notificationsList))
        {
            qDebug() << "Ошибка парсинга диалогов";
            return;
        }
}
