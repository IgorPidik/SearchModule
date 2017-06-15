#include "filecrawler.h"

FileCrawler::FileCrawler(SearchModule *handler, QQueue<QString> *queue, QMutex *mutex):AbstractCrawler(handler, queue, mutex)
{

}

bool FileCrawler::checkConditions(QString &obj)
{
    PatternConditions *patternConditions = static_cast<PatternConditions*>(getConditons());
    QFileInfo info(obj);
    if(info.isFile() && checkNamePattern(info, patternConditions->namePattern)
            && checkContextPattern(obj, patternConditions->contextPattern))
        return true;
    return false;
}

bool FileCrawler::checkNamePattern(QFileInfo &info, QString &namePattern)
{
    return SearchModule::stringContains(info.baseName(), namePattern);
}

bool FileCrawler::checkContextPattern(QString &obj, QString &contextPattern)
{
    QFile file(obj);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(SearchModule::stringContains(line, contextPattern))
            return true;
    }
    return false;
}

QList<QString> FileCrawler::getChildern(QString &obj)
{
    QList<QString> children;
    QFileInfo info(obj);
    if(info.isDir())
    {
        QDir directory(obj);
        foreach (QFileInfo info, directory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
        {
            children.append(info.absoluteFilePath());
        }
    }
    return children;
}
