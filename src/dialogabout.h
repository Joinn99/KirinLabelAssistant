/*==================Kirin Label Assistant================/

    file            dialogabout.h
    brief           定义关于对话框对象
    description	    继承QDialog
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

/////////====== Includes

#include <QDialog>
#include <QTextStream>
#include <QFile>
#include <QString>

/////////====== NameSpace

namespace Ui {


class DialogAbout;
}

/////////====== Class DialogAbout

class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAbout(QWidget *parent = nullptr, int theme = 0);
    ~DialogAbout();

public slots:
    void slotWindowMove(QPoint pos);

private:
    Ui::DialogAbout *ui;                                        //UI对象
};

#endif // DIALOGABOUT_H
