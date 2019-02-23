/*==================Kirin Label Assistant================/

    file            widgetgraphicsview.cpp
    brief           显示区域对象继承
    description	    加入缩放控制
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "widgetgraphicsview.h"

/////////====== Public Functions

///------------ 类基本函数

/*--------------WidgetGraphicsView::WidgetGraphicsView(QWidget *parent) : QGraphicsView(parent)
    brief       类构造函数
    param       QWidget *parent:该部件的父部件
*/
WidgetGraphicsView::WidgetGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    zoomable = true;
}

/////////====== Protected Functions

/*-------------WidgetGraphicsView::wheelEvent(QWheelEvent *event)
    brief       滚轮事件处理函数（覆写QGraphicsView::wheelEvent）
    param       QWheelEvent *event:触发的滚轮事件
    return      void
    description 滚轮控制放大缩小
*/
void WidgetGraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);
    if(zoomable){
        QPoint scrollAmount = event->angleDelta();
        scrollAmount.y() > 0 ? zoomIn() : zoomOut();
        centerOn(mapToScene(event->posF().toPoint()));
    }
}

/////////====== Public Slot Functions

/*-------------WidgetGraphicsView::setZoomable(bool value)
    brief       缩放功能控制槽函数
    param       bool value:是否允许放大
    return      void
    description 滚轮控制放大缩小
*/
void WidgetGraphicsView::setZoomable(bool value)
{
    zoomable = value;
}

/*-------------WidgetGraphicsView::zoomIn()
    brief       放大槽函数
    param       void
    return      void
    description 放大显示区域1.1倍
*/
void WidgetGraphicsView::zoomIn()
{
    zoom(1.1);
}

/*-------------WidgetGraphicsView::zoomOut()
    brief       缩小槽函数
    param       void
    return      void
    description 缩小显示区域0.9倍
*/
void WidgetGraphicsView::zoomOut()
{
    zoom(0.9);
}

/*-------------WidgetGraphicsView::zoomOriginal()
    brief       原始大小槽函数
    param       void
    return      void
    description 显示区域显示原始大小
*/
void WidgetGraphicsView::zoomOriginal()
{
    zoom(1/matrix().m11());
}

/*-------------WidgetGraphicsView::zoomBestFit()
    brief       适应窗口大小槽函数
    param       void
    return      void
    description 使缩放值适合当前窗口大小
*/
void WidgetGraphicsView::zoomBestFit()
{
    if(scene()!= nullptr){
        qreal horizonScale;
        qreal verticalScale;
        horizonScale = width() / scene()->width();
        verticalScale = height() / scene()->height();
        zoom(horizonScale < verticalScale ? 0.98*horizonScale/matrix().m11() : 0.98*verticalScale/matrix().m11());
    }
}

/*-------------WidgetGraphicsView::zoomAbsolute(int scaleSize)
    brief       绝对缩放槽函数
    param       int scaleSize:缩放百分比
    return      void
    description 根据输入值进行绝对缩放
*/
void WidgetGraphicsView::zoomAbsolute(int scaleSize){
    zoom((static_cast<double>(scaleSize)/100)/matrix().m11());
}


/*-------------WidgetGraphicsView::zoom(double scaleFactor)
    brief       缩放执行槽函数
    param       double scaleFactor:缩放绝对比例
    return      void
    description 实现缩放动作
*/
void WidgetGraphicsView::zoom(double scaleFactor)
{
    scaleFactor *= matrix().m11();
    scaleFactor = scaleFactor < 0.1 ? 0.1 : scaleFactor;
    scaleFactor = scaleFactor > 5 ? 5 : scaleFactor;
    setMatrix(QMatrix(scaleFactor,0,0,scaleFactor,0,0));
    emit zoomRatioChanged(matrix().m11());
}