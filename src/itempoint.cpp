/*==================Kirin Label Assistant================/

    file            itempoint.cpp
    brief           定义标注框顶点圆形
    description	    继承QGraphicsEllipseItem
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "itempoint.h"

/////////====== Public Functions

/*--------------ItemPoint::ItemPoint(QPointF coord)
    brief       类构造函数
    param       QPointF coord:顶点初始坐标
*/
ItemPoint::ItemPoint(QPointF coord)
{

    pointCursor = new QPointF(coord);
    pointPen = new QPen(QColor(64,64,64,224));
    pointBrush = new QBrush(QColor(240,240,240,168));
    setPen(*pointPen);
    setBrush(*pointBrush);
    setRect(QRectF(-RECSIZE/2,-RECSIZE/2,RECSIZE,RECSIZE));
    setPos(coord);
    setFlags(QGraphicsPolygonItem::ItemIsMovable | QGraphicsPolygonItem::ItemIsSelectable |
             QGraphicsPolygonItem::ItemIgnoresTransformations);
}

/*--------------ItemPoint::setPoint(QPointF pointCoord)
    brief       坐标设定函数
    param       QPointF pointCoord:顶点初始坐标
    return	    void
    description 设定顶点坐标

*/
void ItemPoint::setPoint(QPointF pointCoord)
{
    this->setPos(pointCoord);
    emit pointMoved();
}

/////////====== Public Slot Functions
/*--------------ItemPoint::pointUpdate(QPointF pointCoord)
    brief       坐标更新槽函数
    param       QPointF pointCoord:顶点坐标
    return      void
    description 更新坐标顶点
*/
void ItemPoint::pointUpdate(QPointF pointCoord)
{
    this->setPos(scenePos() + pointCoord);
}

/////////====== Protected Functions

/*--------------ItemPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QGraphicsItem::mousePressEventEvent）
    param       QGraphicsSceneMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标点击顶点后的事件
*/
void ItemPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        QGraphicsItem::mousePressEvent(event);
        *pointCursor = event->scenePos() - scenePos();
    }

}

/*--------------ItemPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QGraphicsItem::mouseMoveEventEvent）
    param       QGraphicsSceneMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标拖动顶点后的事件
*/
void ItemPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    emit pointMoved();
}

/*--------------ItemPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    brief       绘制槽函数（覆写QGraphicsItem::paint）
    param       QPainter *painter:绘制器	const QStyleOptionGraphicsItem *option:绘制选项	    QWidget *widget：父对象
    return      void
    description 更新绘制对象
*/
void ItemPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if (option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    QGraphicsEllipseItem::paint(painter, &op, widget);
}

/*--------------ItemPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    brief       对象选中状态改变槽函数（覆写QGraphicsItem::itemChange）
    param       QGraphicsItem::GraphicsItemChange change:变化对象	QVariant &value:变化值
    return      QVarient	变化值
    description 更新选中状态
*/
QVariant ItemPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged){
        emit pointSelectionChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}
