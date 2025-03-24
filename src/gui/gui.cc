#include "gui.h"
#include "ui_gui.h"
#include <QFile>
#include <QTextStream>

Gui::Gui(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gui)
{
    ui->setupUi(this);
    loadTxtFile();
}

Gui::~Gui()
{
    delete ui;
}

void Gui::loadTxtFile(){
    QFile inputFile(":/resources/input.txt");
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->textEdit->setPlainText(line);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
}

void Gui::on_pushButton_clicked()
{
    QString searchString = ui->lineEdit->text();
    ui->textEdit->find(searchString, QTextDocument::FindWholeWords);
}



