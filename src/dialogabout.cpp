/*==================Kirin Label Assistant================/

    file            dialogabout.cpp
    brief           定义关于对话框对象
    description	    继承QDialog
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Include

#include "dialogabout.h"
#include "ui_dialogabout.h"

/////////====== Public Functions
/*--------------DialogAbout::DialogAbout(QWidget *parent, int theme)
    brief       类构造函数
    param       QWidget *parent:父对象	int theme:主题序号
*/
DialogAbout::DialogAbout(QWidget *parent, int theme) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QFile file(QString(":/qss/themes/Default-").append("%1").arg(theme % 6).append(".qss"));
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    setContextMenuPolicy(Qt::PreventContextMenu);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->dialogTextBrowser->setOpenExternalLinks(true);
    connect(ui->dialogFrame, SIGNAL(windowMove(QPoint)), this, SLOT(slotWindowMove(QPoint)));
    show();
}

/*--------------DialogAbout::DialogAbout(QWidget *parent, int theme)
    brief       类析构函数
    param       void
*/
DialogAbout::~DialogAbout()
{
    delete ui;
}

/////////====== Public Slot Functions

/*--------------DialogAbout::slotWindowMove(QPoint pos)
    brief       窗口移动槽函数
    param       QPoint pos: 移动坐标
    return      void
    description 移动窗口位置
*/
void DialogAbout::slotWindowMove(QPoint pos)
{
    move(this->pos() + pos);
}