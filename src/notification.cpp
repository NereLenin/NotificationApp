#include "notification.h"

Notification::Notification()
{
    readed = false;
    id=0;
}

void Notification::setId(int id){ this->id = id; }
void Notification::setText(const QString& text){ this->text = text; }
void Notification::setTime(const QDateTime& time){ this->time = time; }
void Notification::setReaded(const bool& readed){ this->readed = readed; }

const int& Notification::getId(){ return id; }
const QString& Notification::getText(){ return text; }
const QDateTime Notification::getTime(){ return time;}
const bool& Notification::getReaded(){ return readed; }
