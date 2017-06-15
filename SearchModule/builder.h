#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>
#include "builderinterface.h"
#include "filecrawler.h"

class Builder : public BuilderInterface
{
public:
    Builder();
    virtual ~Builder();
    FileCrawler *build();
    void setHandler(SearchModule *handler);
    void setQueue(QQueue<QString>* queue);
    QQueue<QString>* getQueue();
    void initQueue();
    void setParentDir(QString parentDir);

private:
    QQueue<QString> *queue;
    QMutex *mutex;
    SearchModule *handler;
    QString parentDir;

};

#endif // BUILDER_H
