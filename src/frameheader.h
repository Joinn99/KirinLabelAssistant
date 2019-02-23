/*==================Kirin Label Assistant================/

    file            frameheader.h
    brief           定义标题栏对象
    description	    继承QFrame
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef FRAMEHEADER_H
#define FRAMEHEADER_H

/////////====== Includes

#include <QFrame>
#include <QMouseEvent>

/////////====== Class FrameHeader

class FrameHeader : public QFrame
{
    Q_OBJECT
public:
    explicit FrameHeader(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

signals:
    void windowMove(QPoint pos);

private:
    QPoint dragPos;                                         //拖动坐标
};

#endif // FRAMEHEADER_H
