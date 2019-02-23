/*==================Kirin Label Assistant================/

    file            windowmain.h
    brief           定义QtGUI主界面
    description	    继承QMainWindow
                    关联windowmain.ui,为主窗口显示的具体实现
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

/////////====== Includes

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <QPainter>
#include <QtDebug>
#include <opencv2/video.hpp>

#include "itemrect.h"
#include "cveast.h"
#include "frameheader.h"
#include "dialogabout.h"

/////////====== Namespace

namespace Ui {
class WindowMain;
}

/////////====== Class WindowMain

class WindowMain : public QMainWindow
{
    Q_OBJECT

public:
///------------ 类基本函数

    explicit WindowMain(QWidget *parent = nullptr);
    ~WindowMain() Q_DECL_OVERRIDE;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    /*--------------signalRectSelectionChanged(ItemRect* current);
        brief	    标注框选择改变信号
        param       ItemRect* current:当前选中标注框对象
        return      void
        description 本信号在选中标注框改变后发出
    */
    void signalRectSelectionChanged(ItemRect* current);

private slots:

///============ 文件处理槽函数
    void slotOpen();
    void slotLoad();
    void slotChooseLastImage();
    void slotChooseNextImage();
///============ 标注框操作响应槽函数
    void slotRectSelect(int currentRect);
    void slotRectSelect(ItemRect* recSelected);
    void slotRectChange();
    void slotSpinValueChange(ItemRect* current);
    void slotAddLabel();
    void slotDeleteLabel();
    void slotClearLabels();
    void slotUpdateLabels();
    void slotSaveLabel();
///============ 显示界面操作响应槽函数
    void slotZoomScaleDisplay(double zoomScale);
///============ 实时标注操作响应槽函数
    void slotCaptureRefresh();
    void slotCapture();
    void slotSaveCapture();
    void slotDiscardCapture();
///============ EAST模型设置响应槽函数
    void slotEASTSettingChange(int value);
    void slotEASTSettingApply();
    void slotEASTSettingDefault();
    void slotEASTSettingDisplay();
    void slotEASTPathChange();
///============ 应用软件操作槽函数
    void slotModeChange();
    void slotAbout();
    void slotCloseEvent();
    void slotMinimumEvent();
    void slotSizeControlEvent(bool checked);
    void slotWindowMove(QPoint pos);
    void slotThemeChange();
    void slotSaveSettings();

private:
    enum {TopLeft,TopRight,BottomRight,BottomLeft};         //枚举：标注框顶点位置
    Ui::WindowMain *ui;                                     //UI对象
    int qssTheme;                                           //当前主题序号

    QSettings* settingsWindow;                              //用户设置对象
    QActionGroup* actionGroupZoom;                          //缩放工具组
    QActionGroup* actionGroupLabel;                         //标注工具组
    QActionGroup* actionGroupFile;                          //文件工具组
    QActionGroup* actionGroupCapture;                       //拍照工具组

    QDir* dirFile;                                          //文件夹路径
    QFileInfoList* fileInfoListFile;                        //文件列表

    QList<ItemRect*> listRect;                              //标注框列表
    QGraphicsScene* imageScene;                             //文件标注显示场景
    QPixmap* imagePix;                                      //显示图片对象
    ItemRect* currentRect;                                  //当前标注框对象

    QDir* dirCapture;                                       //照片保存路径
    QGraphicsScene* captureScene;                           //实时检测显示场景
    cv::VideoCapture* captureCamera;                        //摄像头对象
    cv::Mat captureFrame;                                   //摄像头帧对象
    QTimer* captureTimer;                                   //实时监测定时器
    QPen* capturePen;                                       //标注框边界绘制
    QBrush* captureBrush;                                   //标注框区域绘制

    CVEast* labelDetector;                                  //EAST模型对象
    QFileInfo* modelFile;                                   //当前文件信息
    QTime appTimer;                                         //计时器

    void spinRangeChange(QRect range);
    double sigmoid(double x);
};

#endif // WINDOWMAIN_H
