#ifndef ABSTRACTCRAWLER_H
#define ABSTRACTCRAWLER_H

#include "crawlerinterface.h"
#include "conditionsinterface.h"
#include "dataevent.h"

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QtCore>

class SearchModule;

template<class T>
class AbstractCrawler: public CrawlerInteface, public QObject
{    
public:
    AbstractCrawler(SearchModule *handler, QQueue<T> *queue, QMutex *mutex);
    virtual ~AbstractCrawler();
    QThread* getThread();
    void setConditions(ConditionsInterface *conditions);
    void setQueue(QQueue<T> *queue);
    QList<QVariant> getResults();
public slots:
    void start();
    void stop();

protected:
    virtual void run() final;
    ConditionsInterface *getConditons();
    virtual bool checkConditions(T &obj) = 0;
    virtual QList<T> getChildern(T &obj) = 0;

private:
    void notifyHandler(T data);
    void notifyHandlerProcessFinished();
    void notifyHandlerProcessStarted();

    QList<QVariant> results;
    QQueue<T> *queue;
    QMutex *mutex;
    SearchModule *handler;
    QThread *thread;
    ConditionsInterface *conditions;
    bool stopped;
};

template<class T>
AbstractCrawler<T>::AbstractCrawler(SearchModule *handler, QQueue<T> *queue, QMutex *mutex)
{
    this->handler = handler;
    this->queue = queue;
    this->mutex = mutex;
    stopped = false;
    thread = new QThread();
    thread->setObjectName("0");
    moveToThread(thread);
    thread->start();
}

template<class T>
AbstractCrawler<T>::~AbstractCrawler()
{
}

template<class T>
void AbstractCrawler<T>::start()
{
    stopped = false;
    results.clear();
    notifyHandlerProcessStarted();
    run();
}

template<class T>
void AbstractCrawler<T>::stop()
{
    stopped = true;
}

template<class T>
void AbstractCrawler<T>::run()
{
    int msec = thread->objectName().toInt()*100;
    QThread::currentThread()->msleep(msec);

    while(!stopped)
    {
        mutex->lock();
        if(queue->isEmpty())
        {
            mutex->unlock();
            break;
        }
        T obj = queue->dequeue();
        mutex->unlock();

        QList<T> children = getChildern(obj);

        mutex->lock();
        foreach(T child, children)
        {
            queue->enqueue(child);
        }
        mutex->unlock();

        if(checkConditions(obj))
            notifyHandler(obj);
    }
    notifyHandlerProcessFinished();
}

template<class T>
ConditionsInterface* AbstractCrawler<T>::getConditons()
{
    return conditions;
}

template<class T>
void AbstractCrawler<T>::notifyHandler(T data)
{
    results.append(QVariant::fromValue(data));
    QCoreApplication::postEvent(static_cast<QObject*>(handler), new DataEvent(QVariant::fromValue(data)));
}

template<class T>
void AbstractCrawler<T>::notifyHandlerProcessFinished()
{
    QMetaObject::invokeMethod(static_cast<QObject*>(handler), "processFinished", Qt::QueuedConnection);
    stopped = true;
}

template<class T>
void AbstractCrawler<T>::notifyHandlerProcessStarted()
{
    QMetaObject::invokeMethod(static_cast<QObject*>(handler), "processStarted", Qt::QueuedConnection);
}

template<class T>
QThread* AbstractCrawler<T>::getThread()
{
    return thread;
}

template<class T>
void AbstractCrawler<T>::setConditions(ConditionsInterface *conditions)
{
    this->conditions = conditions;
}

template<class T>
void AbstractCrawler<T>::setQueue(QQueue<T> *queue)
{
    this->queue = queue;
}

template<class T>
QList<QVariant> AbstractCrawler<T>::getResults()
{
    return results;
}

#endif // ABSTRACTCRAWLER_H
