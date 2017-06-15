#ifndef SEARCHMODULE_H
#define SEARCHMODULE_H

#include <QObject>
#include "builderinterface.h"
#include "abstractcrawler.h"

class BuilderInterface;
class ConditionsInterface;

class SearchModule : public QObject
{
    Q_OBJECT
public:
    explicit SearchModule(BuilderInterface *builder, int numberOfThreads, QObject *parent = nullptr);
    virtual ~SearchModule();
    static bool stringContains(QString first, QString second);
    QList<QVariant> getResults();
    BuilderInterface* getBuilder();
    ConditionsInterface* getCurrentConditions();


protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void updateConditions(ConditionsInterface *conditions);

signals:
    void startCrawlers();
    void stopWriting();
    void searchFinished();
    void searchDataReceived(QVariant data);

public slots:
    virtual void startSearch(ConditionsInterface *conditions);
    virtual void stopSearch();
    virtual void updateSearch(ConditionsInterface *conditions);
    virtual void processFinished();
    virtual void processStarted();

private:
    void createCrawlers();
    void moveToSeparatedThread();

    QList<CrawlerInteface*> crawlerHolder;
    BuilderInterface *builder;
    int numberOfThreads;
    int runningThreadsCounter;
    QThread *thread;
    ConditionsInterface *currentConditions;



};

#endif // SEARCHMODULE_H
