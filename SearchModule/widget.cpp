#include "widget.h"
#include "ui_widget.h"
#include "searchmodule.h"
#include "builder.h"

Widget::Widget(Builder *builder, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("SearchModule Example");
    this->builder = builder;
    ui->pathEdit->setPlaceholderText("Directory path [Default=Current directory]");
    writing = false;
    counter = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::searchFinished()
{
    if(writing)
        ui->resultArea->append("Search Finished!\n"+ QString::number(counter)+" matches!");
}

void Widget::searchDataReceived(QVariant data)
{
    if(writing)
    {
        counter++;
        ui->resultArea->append(data.toString());
    }
}

void Widget::stopWriting()
{
    writing = false;
}

void Widget::startWriting()
{
    writing = true;
    counter = 0;
}

void Widget::on_startButton_clicked()
{
    setParentDir();
    emit startSearch(new PatternConditions(ui->nameEdit->text(), ui->contextEdit->text()));
    ui->resultArea->setText("Searching...");
}

void Widget::on_stopButton_clicked()
{
    emit stopSearch();
}

void Widget::setParentDir()
{
    if(ui->pathEdit->text() != "")
        builder->setParentDir(ui->pathEdit->text());
    else
        builder->setParentDir(QDir::currentPath());
}

void Widget::on_toolButton_clicked()
{
    QString parentDir = QFileDialog::getExistingDirectory(this, "Select folder", QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->pathEdit->setText(parentDir);
}
