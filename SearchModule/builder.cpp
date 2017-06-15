#include "builder.h"

Builder::Builder()
{
    queue = new QQueue<QString>();
    mutex = new QMutex;
    initQueue();
    parentDir = QDir::currentPath();
}

Builder::~Builder()
{
}

FileCrawler *Builder::build()
{
    FileCrawler *crawler = new FileCrawler(handler, queue, mutex);

    QObject::connect(handler, &SearchModule::startCrawlers, crawler, &FileCrawler::start, Qt::QueuedConnection);
    return crawler;
}

void Builder::setHandler(SearchModule *handler)
{
    this->handler = handler;
}

void Builder::setQueue(QQueue<QString> *queue)
{
    this->queue = queue;
}

QQueue<QString> *Builder::getQueue()
{
    return queue;
}

void Builder::initQueue()
{
    queue->clear();
    queue->append(parentDir);
}

void Builder::setParentDir(QString parentDir)
{
    this->parentDir = parentDir;
}
