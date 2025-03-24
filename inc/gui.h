#ifndef GUI_H
#define GUI_H
#include <QWidget>

namespace Ui
{
class gui;
}
QT_END_NAMESPACE

class Gui : public QWidget
{
	Q_OBJECT

  public:
	Gui(QWidget *parent = nullptr);
	~Gui();

  private slots:
	void on_pushButton_clicked();

  private:
	Ui::gui *ui;
	void loadTxtFile();
};
#endif // GUI_H
