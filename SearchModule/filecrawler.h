#ifndef FILECRAWLER_H
#define FILECRAWLER_H

#include "abstractcrawler.h"
#include <QDebug>
#include "patternconditions.h"

class FileCrawler : public AbstractCrawler<QString>
{
public:
    FileCrawler(SearchModule *handler, QQueue<QString> *queue, QMutex *mutex);

protected:
    bool checkConditions(QString &obj);
    QList<QString> getChildern(QString &obj);
private:
    bool checkNamePattern(QFileInfo &info, QString &namePattern);
    bool checkContextPattern(QString &obj, QString &contextPattern);
};

#endif // FILECRAWLER_H
