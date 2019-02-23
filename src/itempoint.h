/*==================Kirin Label Assistant================/

    file            itempoint.h
    brief           定义标注框顶点圆形
    description	    继承QGraphicsEllipseItem
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef ITEMPOINT_H
#define ITEMPOINT_H

/////////====== Includes

#include <QStyle>
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

/////////====== Define

#define RECSIZE 12	                                            //顶点圆形直径

/////////====== Class ItemPoint

class ItemPoint : public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit ItemPoint(QPointF coord = QPointF(0,0));
    void setPoint(QPointF pointCoord);
signals:

    /*--------------pointMoved();
        brief	    顶点移动信号
        param       void
        return      void
        description 本信号在顶点坐标改变时发出
    */
    void pointMoved();

    /*--------------pointSelectionChanged();
        brief	    顶点选中状态改变信号
        param       void
        return      void
        description 本信号在顶点选中状态改变时发出
    */
    void pointSelectionChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) Q_DECL_OVERRIDE;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

public slots:
    void pointUpdate(QPointF pointCoord);

private:
    enum {TopLeft,TopRight,BottomRight,BottomLeft};                 //枚举：顶点位置
    enum {RecUnselected,RecSelected};                               //枚举：选中状态
    QPointF* pointCursor;                                           //鼠标坐标
    QPen* pointPen;                                                 //顶点边界绘制
    QBrush* pointBrush;                                             //顶点区域绘制
};

#endif // ITEMPOINT_H
