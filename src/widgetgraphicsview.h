/*==================Kirin Label Assistant================/

    file            widgetgraphicsview.h
    brief           显示区域对象继承
    description	    加入缩放控制
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef WIDGETGRAPHICSVIEW_H
#define WIDGETGRAPHICSVIEW_H

/////////====== Includes

#include <QGraphicsView>
#include <QWheelEvent>

/////////====== Class WidgetGraphicsView

class WidgetGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit WidgetGraphicsView(QWidget *parent = nullptr);
    void setZoomable(bool value);

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

signals:
    /*--------------zoomRatioChanged(double zoomScale);
        brief	    缩放比例改变信号
        param       double zoomScale:缩放值
        return      void
        description 本信号在缩放比例改变后发出
    */
    void zoomRatioChanged(double zoomScale);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomOriginal();
    void zoomBestFit();
    void zoomAbsolute(int scaleSize);

private:
    void zoom(double scaleFactor);                              
    bool zoomable;                                                  //是否允许缩放

};

#endif // WIDGETGRAPHICSVIEW_H
