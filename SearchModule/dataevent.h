#ifndef DATAEVENT_H
#define DATAEVENT_H

#include <QObject>
#include <QEvent>
#include <QVariant>

class DataEvent : public QEvent
{
public:
    DataEvent(QVariant data);
    QVariant getData();

private:
    QVariant data;

};

#endif // DATAEVENT_H
