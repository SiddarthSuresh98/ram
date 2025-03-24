#ifndef GUI_H
#define GUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GUI;
}
QT_END_NAMESPACE

class GUI : public QWidget
{
    Q_OBJECT

public:
    GUI(QWidget *parent = nullptr);
    ~GUI();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GUI *ui;
    void loadTxtFile();
};
#endif // GUI_H
