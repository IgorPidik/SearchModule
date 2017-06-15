#ifndef CRAWLERINTERFACE_H
#define CRAWLERINTERFACE_H

#include <QThread>
#include <QList>
#include "searchmodule.h"
#include "conditionsinterface.h"

class SearchModule;

class CrawlerInteface
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual QThread* getThread() = 0;
    virtual void setConditions(ConditionsInterface *condicions) = 0;
    virtual QList<QVariant> getResults() = 0;

protected:
    virtual void run() = 0;

};

#endif // CRAWLERINTERFACE_H
