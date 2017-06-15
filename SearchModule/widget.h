#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "searchmodule.h"
#include "patternconditions.h"
#include "builder.h"
#include <QFileDialog>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(Builder *builder, QWidget *parent = 0);
    ~Widget();
signals:
    void startSearch(ConditionsInterface *conditions);
    void updateSearch(ConditionsInterface *conditions);
    void stopSearch();

public slots:
    void searchFinished();
    void searchDataReceived(QVariant data);
    void stopWriting();
    void startWriting();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_toolButton_clicked();

private:
    void setParentDir();

    Ui::Widget *ui;
    Builder *builder;
    bool writing;
    long long counter;
};

#endif // WIDGET_H
