#include "widget.h"
#include <QApplication>
#include "searchmodule.h"
#include "builder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Builder *builder = new Builder();
    Widget w(builder);
    SearchModule searchModule(builder, 2);

    QObject::connect(&w, &Widget::startSearch, &searchModule, &SearchModule::startSearch, Qt::DirectConnection);
    QObject::connect(&w, &Widget::stopSearch, &searchModule, &SearchModule::stopSearch, Qt::DirectConnection);

    QObject::connect(&searchModule, &SearchModule::startCrawlers, &w, &Widget::startWriting, Qt::QueuedConnection);
    QObject::connect(&searchModule, &SearchModule::stopWriting, &w, &Widget::stopWriting, Qt::DirectConnection);

    QObject::connect(&searchModule, &SearchModule::searchFinished, &w, &Widget::searchFinished, Qt::QueuedConnection);
    QObject::connect(&searchModule, &SearchModule::searchDataReceived, &w, &Widget::searchDataReceived, Qt::QueuedConnection);

    w.show();

    return a.exec();
}
