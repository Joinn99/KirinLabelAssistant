/*==================Kirin Label Assistant================/

    file            frameheader.cpp
    brief           定义标题栏对象
    description	    继承QFrame
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "frameheader.h"

/////////====== Public Functions

/*--------------FrameHeader::FrameHeader(QWidget *parent): QFrame(parent)
    brief       标题栏对象构造函数
    param       QWidget *parent:该部件的父部件
*/
FrameHeader::FrameHeader(QWidget *parent): QFrame(parent)
{

}

/////////====== Protected Functions

/*--------------FrameHeader::mousePressEvent(QMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QFrame::mousePressEventEvent）
    param       QMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标点击标题栏后的事件
*/
void FrameHeader::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragPos = event->globalPos();

}

/*--------------FrameHeader::mouseMoveEvent(QMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QGraphicsItem::mouseMoveEventEvent）
    param       QMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标拖动标题栏后的事件
*/
void FrameHeader::mouseMoveEvent(QMouseEvent * event)
{
    if (!event->buttons().testFlag(Qt::LeftButton)) return;
    const QPoint position = event->globalPos() - dragPos;
    emit windowMove(position);
    dragPos = event->globalPos();

}
