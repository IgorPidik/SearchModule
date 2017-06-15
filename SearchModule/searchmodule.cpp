#include "searchmodule.h"

SearchModule::SearchModule(BuilderInterface *builder, int numberOfThreads, QObject *parent) : QObject(parent)
{
    this->builder = builder;
    this->numberOfThreads = numberOfThreads;
    runningThreadsCounter = 0;
    currentConditions = 0;

    installEventFilter(this);
    builder->setHandler(this);
    moveToSeparatedThread();
    createCrawlers();
}

SearchModule::~SearchModule()
{
    delete builder;
    foreach (CrawlerInteface *crawler, crawlerHolder)
    {
        crawler->stop();
        crawler->getThread()->quit();
    }
    thread->quit();
}

void SearchModule::startSearch(ConditionsInterface *conditions)
{
    stopSearch();
    updateConditions(conditions);
    builder->initQueue();

    emit startCrawlers();
}

void SearchModule::updateConditions(ConditionsInterface *conditions)
{
    currentConditions = conditions;
    foreach(CrawlerInteface *crawler, crawlerHolder)
    {
        crawler->setConditions(conditions);
    }
}

void SearchModule::updateSearch(ConditionsInterface *conditions)
{
    stopSearch();
    startSearch(conditions);
}

void SearchModule::processFinished()
{
    runningThreadsCounter--;
    if(runningThreadsCounter == 0)
    {
        emit searchFinished();
    }
}

void SearchModule::processStarted()
{
    runningThreadsCounter++;
}

void SearchModule::stopSearch()
{
    emit stopWriting();
    foreach(CrawlerInteface *crawler, crawlerHolder)
    {
        crawler->stop();
    }
    QThread::currentThread()->msleep(200);
    QCoreApplication::removePostedEvents(this, QEvent::User+1);
}

void SearchModule::createCrawlers()
{
    for(int i = 0; i < numberOfThreads; i++)
    {
        CrawlerInteface *crawler = builder->build();
        crawler->getThread()->setObjectName(QString::number(i));
        crawlerHolder.append(crawler);
    }
}

void SearchModule::moveToSeparatedThread()
{
    thread = new QThread();
    this->moveToThread(thread);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

QList<QVariant> SearchModule::getResults()
{
    QList<QVariant> results;
    foreach(CrawlerInteface *crawler, crawlerHolder)
    {
        results.append(crawler->getResults());
    }
    return results;
}

bool SearchModule::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    if(event->type() == QEvent::User+1)
    {
        DataEvent *dateEvent = static_cast<DataEvent*>(event);
        emit searchDataReceived(dateEvent->getData());
        QThread::currentThread()->msleep(10);
        return true;
    }
    return false;
}

bool SearchModule::stringContains(QString first, QString second)
{
    if(first == "")
        return false;
    if(second == "")
        return true;
    bool equals = false;
    for(int i = 0; i < first.length(); i++)
    {
        // search for first match
        if(first.at(i) == second.at(0))
        {
            equals = true;
            // second string must be longer or equal to the length of the rest of the first string otherwise they won't match
            if(first.length() - i >= second.length())
            {
                for(int j = 1; j < second.length(); j++)
                {
                    if(first.at(i+j) != second.at(j))
                    {
                        // characters doesn't match, continue search for another match
                        equals = false;
                        break;
                    }
                }
            }
            else
            {
                return false;
            }
            // if all characters match, return true
            if(equals)
                return true;
        }
    }
    return false;
}

BuilderInterface *SearchModule::getBuilder()
{
    return builder;
}

ConditionsInterface *SearchModule::getCurrentConditions()
{
    return currentConditions;
}
