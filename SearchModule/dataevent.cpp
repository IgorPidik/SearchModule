#include "dataevent.h"

DataEvent::DataEvent(QVariant data):QEvent((QEvent::Type)(QEvent::User+1))
{
    this->data = data;
}

QVariant DataEvent::getData()
{
    return data;
}
