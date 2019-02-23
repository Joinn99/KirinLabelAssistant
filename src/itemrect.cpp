/*==================Kirin Label Assistant================/

    file            itemrect.cpp
    brief           定义标注框对象
    description	    继承QGraphicsPolygonItem
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "itemrect.h"

/////////====== Public Functions

/*--------------ItemRect(QString coordRect,QGraphicsScene* windowScene)
    brief       类构造函数
    param       QString coordRect:顶点坐标字符串  QGraphicsScene* windowScene:场景父对象
*/
ItemRect::ItemRect(QString coordRect,QGraphicsScene* windowScene)
{
    recPen[0] = new QPen(QColor(16,16,255,168));
    recPen[0]->setWidth(3);
    recPen[1] = new QPen(QColor(224,16,16,224));
    recPen[1]->setWidth(5);
    recBrush[0] = new QBrush(QColor(16,192,16,32));
    recBrush[1] = new QBrush(QColor(128,16,16,64));
    setPen(*recPen[ItemRect::RecUnselected]);
    setBrush(*recBrush[ItemRect::RecUnselected]);

    QStringList coordList = coordRect.split(",");

    for(int cnt = 0; cnt<RECPOINT; cnt++){
        recPoint[cnt] = new QPointF(coordList.value(2*cnt).toDouble(),coordList.value(2*cnt+1).toDouble());
        recPointRect[cnt] = new ItemPoint(*recPoint[cnt]);
        connect(recPointRect[cnt],SIGNAL(pointMoved()),this,SLOT(recUpdate()));
        connect(this,SIGNAL(recMoved(QPointF)),recPointRect[cnt],SLOT(pointUpdate(QPointF)));
        connect(recPointRect[cnt],SIGNAL(pointSelectionChanged()),this,SLOT(recSelectionChange()));
    }
    recCursor = new QPointF(0,0);

    QPolygonF recPolygon;
    recPolygon << *recPoint[ItemRect::TopLeft] << *recPoint[ItemRect::TopRight] <<
                *recPoint[ItemRect::BottomRight] << *recPoint[ItemRect::BottomLeft];                                                                ;
    setPolygon(recPolygon);

    recTable = new QTableWidgetItem(QString("(%1,%2) , (%3,%4) , (%5,%6) , (%7,%8)").
                  arg(int(recPoint[ItemRect::TopLeft]->x())).arg(int(recPoint[ItemRect::TopLeft]->y())).
                  arg(int(recPoint[ItemRect::TopRight]->x())).arg(int(recPoint[ItemRect::TopRight]->y())).
                  arg(int(recPoint[ItemRect::BottomRight]->x())).arg(int(recPoint[ItemRect::BottomRight]->y())).
                  arg(int(recPoint[ItemRect::BottomLeft]->x())).arg(int(recPoint[ItemRect::BottomLeft]->y())));
    recTable->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    recTable->setTextAlignment(Qt::AlignCenter);
    setFlags(ItemRect::ItemIsMovable|ItemRect::ItemIsSelectable);
    windowScene->addItem(this);
    windowScene->addItem(recPointRect[ItemRect::TopLeft]);
    windowScene->addItem(recPointRect[ItemRect::TopRight]);
    windowScene->addItem(recPointRect[ItemRect::BottomRight]);
    windowScene->addItem(recPointRect[ItemRect::BottomLeft]);
}

/*--------------ItemRect::recPointValue(int pointIndex)
    brief       坐标顶点函数
    param       int pointIndex:顶点坐标序号
    return      QPointF:顶点坐标
    description 返回对应顶点坐标
*/
QPointF ItemRect::recPointValue(int pointIndex)
{
    return recPointRect[pointIndex]->pos();
}

/*--------------ItemRect::recPointValue(int pointIndex)
    brief       坐标顶点设置函数
    param       int pointIndex:顶点坐标序号	QPointF value:坐标值
    return      void
    description 设置对应顶点坐标
*/
void ItemRect::setRecPointValue(int pointIndex, QPointF value)
{
    recPointRect[pointIndex]->setPoint(value);
}

/*--------------ItemRect::recIsSelected()
    brief       选中状态函数
    param       void
    return      bool:是否选中
    description 返回标注框选中状态
*/
bool ItemRect::recIsSelected()
{
    return this->isSelected() ||
                this->recPointRect[0]->isSelected() ||
                this->recPointRect[1]->isSelected() ||
                this->recPointRect[2]->isSelected() ||
                this->recPointRect[3]->isSelected();
}

/*--------------ItemRect::recRemove(QGraphicsScene* windowScene)
    brief       标注框移除函数
    param       QGraphicsScene* windowScene:父对象
    return      void
    description 移除标注框及对应顶点
*/
void ItemRect::recRemove(QGraphicsScene* windowScene)
{
    if(recPointRect[0] != nullptr){
        for(int cnt = 0; cnt < RECPOINT; cnt++){
            recPointRect[cnt]->blockSignals(true);
            windowScene->removeItem(recPointRect[cnt]);
        }
        windowScene->removeItem(this);
    }
}

/////////====== Public Slot Functions

/*--------------ItemRect::ItemRect::recUpdate()
    brief       标注框更新槽函数
    param       void
    return      void
    description 更新标注框坐标
*/
void ItemRect::recUpdate()
{
    *recPoint[ItemRect::TopLeft] = recPointRect[ItemRect::TopLeft]->pos();
    *recPoint[ItemRect::TopRight] = recPointRect[ItemRect::TopRight]->pos();
    *recPoint[ItemRect::BottomRight] = recPointRect[ItemRect::BottomRight]->pos();
    *recPoint[ItemRect::BottomLeft] = recPointRect[ItemRect::BottomLeft]->pos();
    QPolygonF recPolygon;
    recPolygon << *recPoint[ItemRect::TopLeft] << *recPoint[ItemRect::TopRight] <<
                *recPoint[ItemRect::BottomRight] << *recPoint[ItemRect::BottomLeft];                                                                ;
    prepareGeometryChange();
    setPos(0,0);
    setPolygon(recPolygon);
    recUpdateTable();
    emit recMoved(this);
}

/*--------------ItemRect::ItemRect::recUpdate()
    brief       标注框选中状态更新槽函数
    param       void
    return      void
    description 更新标注框选中状态
*/
void ItemRect::recSelectionChange()
{
    if(recIsSelected()){
        setPen(*recPen[ItemRect::RecSelected]);
        setBrush(*recBrush[ItemRect::RecSelected]);
        recUpdateTable();
        emit recSelected(this);
    }
    else{
        setPen(*recPen[ItemRect::RecUnselected]);
        setBrush(*recBrush[ItemRect::RecUnselected]);
    }
}

/////////====== Protected Functions

/*--------------ItemRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QGraphicsItem::mousePressEventEvent）
    param       QGraphicsSceneMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标点击标注框后的事件
*/
void ItemRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        QGraphicsItem::mousePressEvent(event);
        *recCursor = event->scenePos();
    }
}

/*--------------ItemRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    brief       鼠标点击处理槽函数（覆写QGraphicsItem::mouseMoveEventEvent）
    param       QGraphicsSceneMouseEvent *event: 鼠标响应事件
    return      void
    description 处理鼠标拖动标注框后的事件
*/
void ItemRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    emit recMoved(event->scenePos() - *recCursor);
    *recCursor = event->scenePos();
    recUpdateTable();
    emit recMoved(this);
}

/*--------------ItemRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    brief       绘制槽函数（覆写QGraphicsItem::paint）
    param       QPainter *painter:绘制器	const QStyleOptionGraphicsItem *option:绘制选项	    QWidget *widget：父对象
    return      void
    description 更新绘制对象
*/
void ItemRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if (option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    QGraphicsPolygonItem::paint(painter, &op, widget);
}

/*--------------ItemRect::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    brief       对象选中状态改变槽函数（覆写QGraphicsItem::itemChange）
    param       QGraphicsItem::GraphicsItemChange change:变化对象	QVariant &value:变化值
    return      QVarient	变化值
    description 更新选中状态
*/
QVariant ItemRect::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged){
        recSelectionChange();
    }
    return QGraphicsItem::itemChange(change, value);
}

/////////====== Private Functions

/*--------------ItemRect::recUpdateTable()
    brief       更新坐标数值显示函数
    param       void
    return      void
    description 更新坐标值数值
*/
void ItemRect::recUpdateTable()
{
    recTable->setText(QString("(%1,%2) , (%3,%4) , (%5,%6) , (%7,%8)").
                      arg(int(recPointRect[ItemRect::TopLeft]->pos().x())).arg(int(recPointRect[ItemRect::TopLeft]->pos().y())).
                      arg(int(recPointRect[ItemRect::TopRight]->pos().x())).arg(int(recPointRect[ItemRect::TopRight]->pos().y())).
                      arg(int(recPointRect[ItemRect::BottomRight]->pos().x())).arg(int(recPointRect[ItemRect::BottomRight]->pos().y())).
                      arg(int(recPointRect[ItemRect::BottomLeft]->pos().x())).arg(int(recPointRect[ItemRect::BottomLeft]->pos().y())));
    recTable->setSelected(true);
}