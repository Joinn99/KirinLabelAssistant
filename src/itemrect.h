/*==================Kirin Label Assistant================/

    file            itemrect.h
    brief           定义标注框对象
    description	    继承QGraphicsPolygonItem
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef ITEMRECT_H
#define ITEMRECT_H

/////////====== Includes

#include <Qt>
#include <QObject>
#include <QVector>
#include <QGraphicsScene>
#include <QTableWidgetItem>
#include <QGraphicsPolygonItem>
#include <QPainter>
#include "itempoint.h"

/////////====== Define

#define RECPOINT 4	                                            //标注框顶点数

/////////====== Class ItemRect

class ItemRect : public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    explicit ItemRect(QString coordRect, QGraphicsScene* windowScene);
    QTableWidgetItem* recTable;                                 //坐标数值显示对象
    QPointF recPointValue(int pointIndex);
    void setRecPointValue(int pointIndex, QPointF value);
    bool recIsSelected();
    void recRemove(QGraphicsScene* windowScene);
signals:
    void recMoved(QPointF pointCoord);
    void recMoved(ItemRect* rect);
    void recSelected(ItemRect* rect);

public slots:
    void recUpdate();
    void recSelectionChange();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
private:

    enum {TopLeft,TopRight,BottomRight,BottomLeft};             //枚举：顶点位置
    enum {RecUnselected,RecSelected};                           //枚举：选中状态
    QPointF* recCursor;                                         //鼠标坐标
    QPointF* recPoint[RECPOINT];                                //标注框顶点坐标
    ItemPoint* recPointRect[RECPOINT];                          //标注框顶点对象

    QPen* recPen[2];                                            //标注框边界绘制
    QBrush* recBrush[2];                                        //标注框区域绘制
    void recUpdateTable();                                      
};

#endif // WIDGETRECTANGLE_H
