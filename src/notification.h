#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <QString>
#include <QDateTime>
#include <QObject>

class Notification : public QObject 
{
	Q_OBJECT
private:
    int id;
    QString text;
    QDateTime time;
    bool readed;
public:
    Notification();

    void setId(int id);
    void setText(const QString& text);
    void setTime(const QDateTime& time);
    void setReaded(const bool& readed);

    const int& getId();
    const QString& getText();
    const QDateTime getTime();
    const bool& getReaded();
};

#endif // NOTIFICATION_H
