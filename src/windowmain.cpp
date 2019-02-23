/*==================Kirin Label Assistant================/

    file            windowmain.cpp
    brief           定义QtGUI主界面
    description	    继承QMainWindow
                    关联windowmain.ui,为主窗口显示的具体实现
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "windowmain.h"     
#include "ui_windowmain.h"


/////////====== Public Functions

///------------ 类基本函数

/*--------------WindowMain::WindowMain(QWidget *parent)
    brief       主界面类构造函数
    param       QWidget *parent:该部件的父部件
*/
WindowMain::WindowMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowMain)
{
    ui->setupUi(this);

    settingsWindow = new QSettings("KirinSettings.ini", QSettings::IniFormat);

    //Load Icon
    qssTheme = settingsWindow->value("theme",0).toInt();
    QFile file(QString(":/qss/themes/Default-").append("%1").arg(qssTheme).append(".qss"));
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    //Setting
    setContextMenuPolicy(Qt::PreventContextMenu);
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint| Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    resize(settingsWindow->value("size", QSize(1440, 900)).toSize());
    move(settingsWindow->value("pos", QPoint(240, 90)).toPoint());
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setBlurRadius(10);
    ui->mainToolBar->setGraphicsEffect(shadowEffect);
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setBlurRadius(10);
    ui->statusBar->setGraphicsEffect(shadowEffect);   
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->comboSettingSize->setItemDelegate(itemDelegate);

    //Header Initialize
    connect(ui->buttonExit,SIGNAL(clicked(bool)),this,SLOT(slotCloseEvent()));
    connect(ui->buttonMinimum,SIGNAL(clicked(bool)),this,SLOT(slotMinimumEvent()));
    connect(ui->buttonSizeContorol,SIGNAL(clicked(bool)),this,SLOT(slotSizeControlEvent(bool)));
    connect(ui->buttonAbout,SIGNAL(clicked(bool)),this,SLOT(slotAbout()));
    connect(ui->buttonChangeTheme,SIGNAL(clicked(bool)),this,SLOT(slotThemeChange()));
    connect(ui->frameHeader,SIGNAL(windowMove(QPoint)),this,SLOT(slotWindowMove(QPoint)));

    //Action group
    actionGroupZoom = new QActionGroup(this);
    actionGroupZoom->setEnabled(false);
    actionGroupZoom->addAction(ui->actionZoomIn);
    actionGroupZoom->addAction(ui->actionZoomOut);
    actionGroupZoom->addAction(ui->actionBestFit);
    actionGroupZoom->addAction(ui->actionOriginalSize);

    actionGroupLabel = new QActionGroup(this);
    actionGroupLabel->setEnabled(false);
    actionGroupLabel->addAction(ui->actionNewLabel);
    actionGroupLabel->addAction(ui->actionDeleteLabel);
    actionGroupLabel->addAction(ui->actionClearLabels);
    actionGroupLabel->addAction(ui->actionUpdateLabels);

    actionGroupFile = new QActionGroup(this);
    actionGroupFile->setEnabled(false);
    actionGroupFile->addAction(ui->actionLastImage);
    actionGroupFile->addAction(ui->actionNextImage);
    actionGroupFile->addAction(ui->actionSaveNext);

    actionGroupCapture = new QActionGroup(this);
    actionGroupCapture->setEnabled(false);
    actionGroupCapture->setVisible(false);
    actionGroupCapture->addAction(ui->actionCapture);
    actionGroupCapture->addAction(ui->actionSaveCapture);

    ui->actionOpen->setEnabled(false);

    //Status Bar

    //Dial Initialize
    ui->dialZoom->setNotchesVisible(true);
    ui->dialZoom->setWrapping(false);
    ui->dialZoom->setSingleStep(5);
    ui->dialZoom->setPageStep(20);
    ui->dialZoom->setMinimum(10);
    ui->dialZoom->setMaximum(500);
    ui->dialZoom->setRange(10,500);
    ui->dialZoom->setValue(100);
    connect(ui->dialZoom,SIGNAL(valueChanged(int)),ui->graphicsView,SLOT(zoomAbsolute(int)));
    ui->dialZoom->setEnabled(false);

    //FileList Initialize
    ui->textBrowserPath->append("/");
    dirFile = new QDir(ui->textBrowserPath->toPlainText());
    dirFile->setFilter(QDir::Files);
    connect(ui->actionOpen,&QAction::triggered,this,&WindowMain::slotOpen);
    connect(ui->actionLoad,&QAction::triggered,this,&WindowMain::slotLoad);
    connect(ui->actionLastImage,&QAction::triggered,this,&WindowMain::slotChooseLastImage);
    connect(ui->actionNextImage,&QAction::triggered,this,&WindowMain::slotChooseNextImage);
    connect(ui->actionSaveNext,&QAction::triggered,this,&WindowMain::slotSaveLabel);

    ui->tableFileList->setColumnCount(2);
    ui->tableFileList->setHorizontalHeaderLabels(QStringList() << tr("FileName") << tr("Labeled"));
    ui->tableFileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableFileList->horizontalHeader()->setStretchLastSection(true);
    ui->tableFileList->setColumnWidth(0,210);
    ui->tableFileList->horizontalHeader()->setSectionsClickable(false);
    ui->tableFileList->verticalHeader()->setSectionsClickable(false);
    ui->tableFileList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    fileInfoListFile = new QFileInfoList();
    ui->boxFile->setEnabled(false);

    //Coordlist Initialize
    ui->tableCoordinateDisplay->setColumnCount(1);
    ui->tableCoordinateDisplay->setHorizontalHeaderLabels(QStringList() << tr("Coordinate"));
    ui->tableCoordinateDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCoordinateDisplay->horizontalHeader()->setStretchLastSection(true);
    ui->tableCoordinateDisplay->horizontalHeader()->setSectionsClickable(false);
    ui->tableCoordinateDisplay->verticalHeader()->setSectionsClickable(false);
    connect(ui->tableCoordinateDisplay,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(slotRectSelect(int)));
    connect(ui->actionNewLabel,&QAction::triggered,this,&WindowMain::slotAddLabel);
    connect(ui->actionDeleteLabel,&QAction::triggered,this,&WindowMain::slotDeleteLabel);
    connect(ui->actionClearLabels,&QAction::triggered,this,&WindowMain::slotClearLabels);
    connect(ui->actionUpdateLabels,&QAction::triggered,this,&WindowMain::slotUpdateLabels);
    connect(ui->graphicsView,SIGNAL(zoomRatioChanged(double)),this,SLOT(slotZoomScaleDisplay(double)));
    ui->boxCoordinateList->setVisible(false);

    //Spinbox Initizlize
    connect(ui->spinCoordUpLeftX,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordUpLeftY,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordUpRightX,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordUpRightY,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordDownLeftX,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordDownLeftY,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordDownRightX,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    connect(ui->spinCoordDownRightY,SIGNAL(valueChanged(int)),this,SLOT(slotRectChange()));
    ui->boxCoordinateSet->setEnabled(false);
    ui->boxCoordinateSet->setVisible(false);

    //Scene for file mode Initialize
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    currentRect = nullptr;
    imagePix = new QPixmap();
    imageScene = new QGraphicsScene();
    ui->graphicsView->setScene(imageScene);
    connect(ui->actionZoomIn,&QAction::triggered,ui->graphicsView,&WidgetGraphicsView::zoomIn);
    connect(ui->actionZoomOut,&QAction::triggered,ui->graphicsView,&WidgetGraphicsView::zoomOut);
    connect(ui->actionOriginalSize,&QAction::triggered,ui->graphicsView,&WidgetGraphicsView::zoomOriginal);
    connect(ui->actionBestFit,&QAction::triggered,ui->graphicsView,&WidgetGraphicsView::zoomBestFit);
    ui->labelDisplayZoom->setVisible(false);

    //Scene for capture mode Initialize
    captureScene = new QGraphicsScene();
    captureCamera = new cv::VideoCapture();
    capturePen = new QPen(QColor(216,32,32,192));
    captureBrush = new QBrush(QColor(224,224,24,64));
    captureTimer = new QTimer();
    captureTimer->setInterval(50);
    connect(captureTimer,SIGNAL(timeout()),this,SLOT(slotCaptureRefresh()));
    connect(ui->actionCapture,&QAction::triggered,this,&WindowMain::slotCapture);
    connect(ui->actionSaveCapture,&QAction::triggered,this,&WindowMain::slotSaveCapture);
    connect(ui->actionDiscard,&QAction::triggered,this,&WindowMain::slotDiscardCapture);
    dirCapture = new QDir(ui->textBrowserPath->toPlainText());
    ui->boxOperation->setVisible(false);

    //Linkbutton Initialize
    connect(ui->buttonFileMode,SIGNAL(clicked(bool)),this,SLOT(slotModeChange()));
    connect(ui->buttonCameraMode,SIGNAL(clicked(bool)),this,SLOT(slotModeChange()));

    //Label Detector Setting Initialize
    ui->comboSettingSize->setCurrentIndex(4);
    ui->sliderSettingConf->setRange(-1000,600);
    ui->sliderSettingConf->setValue(0);
    ui->sliderSettingNms->setRange(-80,240);
    ui->sliderSettingNms->setValue(0);
    connect(ui->sliderSettingConf,SIGNAL(valueChanged(int)),this,SLOT(slotEASTSettingChange(int)));
    connect(ui->sliderSettingNms,SIGNAL(valueChanged(int)),this,SLOT(slotEASTSettingChange(int)));
    connect(ui->comboSettingSize,SIGNAL(currentIndexChanged(int)),this,SLOT(slotEASTSettingChange(int)));
    connect(ui->buttonEASTSet,SIGNAL(clicked(bool)),this,SLOT(slotEASTSettingApply()));
    connect(ui->buttonDefaultSet,SIGNAL(clicked(bool)),this,SLOT(slotEASTSettingDefault()));
    connect(ui->buttonEASTPath,SIGNAL(clicked(bool)),this,SLOT(slotEASTPathChange()));
    ui->labelSettingConf->setText(tr("EAST Thershold: ").append("%1").arg(6+static_cast<double>(ui->sliderSettingConf->value())/200,0,'g',1));
    ui->labelSettingNms->setText(tr("NMS Thershold: ").append("%1").arg(3+static_cast<double>(ui->sliderSettingNms->value())/40,0,'g',1));
    connect(ui->actionEASTSet,&QAction::triggered,this,&WindowMain::slotEASTSettingDisplay);
    ui->boxEast->setVisible(false);

    //Maxmize Setting
    if (settingsWindow->value("maximized", false).toBool()){
        showMaximized();
        ui->buttonSizeContorol->setChecked(true);
    }
    else {
        showNormal();
        ui->buttonSizeContorol->setChecked(false);
    }

    //Label Detector Model Initialize
    modelFile = new QFileInfo(settingsWindow->value("modelPath").toString());
    labelDetector = new CVEast(modelFile->filePath().toLocal8Bit().toStdString());
    while(!labelDetector->isAvailable()){
        QMessageBox::critical(nullptr, tr("Error"), tr("EAST Model is unavailable"), QMessageBox::Yes);
        modelFile->setFile(QFileDialog::getOpenFileName(this, tr("Choose EAST model"),
                                                 modelFile->filePath(),
                                                 tr("Tensorflow Frozen Model (*.pb)")));
        labelDetector->setNet(modelFile->filePath().toLocal8Bit().toStdString());
        if(!modelFile->isFile()){
            QTimer::singleShot(0, qApp, SLOT(quit()));
            break;
        }
    }
}

/*--------------WindowMain::~WindowMain(QWidget *parent)
    brief       主界面类析构函数
    param       QWidget *parent:该部件的父部件

*/
WindowMain::~WindowMain()
{
    delete ui;
}

/////////====== Private Slot Functions

///============ 文件处理槽函数

/*--------------WindowMain::slotOpen()
    brief       文件打开槽函数
    param       void
    return      void
    description  弹出路径选择对话框，选择路径后将图片列表显示在界面中
*/
void WindowMain::slotOpen()
{
    dirFile->setPath(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                           ui->textBrowserPath->toPlainText(),
                                                           QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks));
    if(dirFile->isAbsolute()){
        ui->textBrowserPath->clear();
        ui->textBrowserPath->append(dirFile->path());
    }
    else{
        dirFile->setPath(ui->textBrowserPath->toPlainText());
    }
    *fileInfoListFile = dirFile->entryInfoList(
                QStringList() << "*.jpg" << "*.png" << "*.bmp",
                QDir::Files|QDir::NoDotAndDotDot);
    ui->tableFileList->clearContents();
    ui->tableFileList->setRowCount(fileInfoListFile->count());
    for(int cnt = 0; cnt < fileInfoListFile->count(); cnt++){
        QTableWidgetItem* nameItem = new QTableWidgetItem(fileInfoListFile->value(cnt).fileName());
        bool labelExists = dirFile->exists(fileInfoListFile->value(cnt).baseName().append(".txt"));
        QTableWidgetItem* labelItem = new QTableWidgetItem(labelExists ? "Labeled":" ");
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        nameItem->setTextAlignment(Qt::AlignCenter);
        labelItem->setFlags(Qt::NoItemFlags);
        labelItem->setTextAlignment(Qt::AlignCenter);
        ui->tableFileList->setItem(cnt,0,nameItem);
        ui->tableFileList->setItem(cnt,1,labelItem);
    }
    ui->labelFileInfo->setText(QString("%1 ").arg(QString::number(fileInfoListFile->count(), 10))
                               .append(tr("Files   ")));
    ui->boxCoordinateList->setVisible(true);
    ui->boxCoordinateSet->setVisible(true);
    ui->boxOperation->setVisible(true);
}

/*--------------WindowMain::slotLoad()
    \brief	    文件加载槽函数
    \param      void
    \return     void
    \descript   加载选中图片，根据标注文件是否存在，选择从文件加载标注文件或用模型生成标注，显示在界面中
*/
void WindowMain::slotLoad()
{
    if(ui->tableFileList->currentItem() == nullptr){
        return;
    }
    qDeleteAll(listRect);
    listRect.clear();
    imageScene->clear();
    ui->tableCoordinateDisplay->blockSignals(true);
    ui->tableCoordinateDisplay->clearContents();
    ui->tableCoordinateDisplay->blockSignals(false);

    imagePix->load(dirFile->path().append("/").append(
                       ui->tableFileList->currentItem()->text()));
    spinRangeChange(imagePix->rect());
    imageScene->addPixmap(*imagePix);
    imageScene->setSceneRect(imagePix->rect());
    ui->graphicsView->setSceneRect(imagePix->rect());
    ui->graphicsView->zoomBestFit();

    QFile fileRead(dirFile->path().append("/").append(
                       ui->tableFileList->currentItem()->text().split(".").first().append(".txt")));
    if(fileRead.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream fileStream(&fileRead);
        while(!fileStream.atEnd()){
            listRect.append(new ItemRect(fileStream.readLine(),imageScene));
            connect(listRect.last(),SIGNAL(recSelected(ItemRect*)),this,SLOT(slotRectSelect(ItemRect*)));
            connect(listRect.last(),SIGNAL(recMoved(ItemRect*)),this,SLOT(slotSpinValueChange(ItemRect*)));
        }
        fileRead.close();
    }
    else{
        fileRead.close();
        appTimer.restart();
        QStringList listLabel = labelDetector->detect(dirFile->path().append("/").append(
                                    ui->tableFileList->currentItem()->text()).toLocal8Bit().toStdString());
        ui->labelDisplayTime->setText(QString("%1 ms   | ").arg(appTimer.elapsed()));
        for(int cnt = 0; cnt < listLabel.count(); cnt++){
            listRect.append(new ItemRect(listLabel.value(cnt).remove("-"),imageScene));
            connect(listRect.last(),SIGNAL(recSelected(ItemRect*)),this,SLOT(slotRectSelect(ItemRect*)));
            connect(listRect.last(),SIGNAL(recMoved(ItemRect*)),this,SLOT(slotSpinValueChange(ItemRect*)));
        }
    }
    ui->tableCoordinateDisplay->setRowCount(listRect.count());
    for(int cnt = 0; cnt < listRect.count(); cnt++){
        ui->tableCoordinateDisplay->setItem(cnt,0,listRect.value(cnt)->recTable);
    }

    ui->labelDisplayInfo->setText(QString(" %1x%2  |  %3 kB").arg(imagePix->width()).arg(imagePix->height()).
            arg(fileInfoListFile->value(ui->tableFileList->currentRow()).size()/1024));
    ui->labelCoordinateList->setText(QString("%1 ").arg(ui->tableCoordinateDisplay->rowCount()).append("Labels  "));
    imageScene->update(imageScene->sceneRect());
    actionGroupFile->setEnabled(true);
    actionGroupZoom->setEnabled(true);
    actionGroupLabel->setEnabled(true);
    ui->actionBestFit->setEnabled(false);
    ui->boxCoordinateList->setEnabled(true);
    ui->boxCoordinateSet->setEnabled(false);
    ui->buttonDeleteLabel->setEnabled(false);
    ui->actionDeleteLabel->setEnabled(false);
    ui->labelDisplayInfo->setVisible(true);
    ui->labelDisplayZoom->setVisible(true);
    ui->dialZoom->setEnabled(true);
    ui->buttonSaveNext->setEnabled(true);
    if(ui->tableFileList->currentRow() == ui->tableFileList->rowCount()-1){
        ui->buttonNextImage->setEnabled(false);
        ui->actionNextImage->setEnabled(false);
    }
    else if (ui->tableFileList->currentRow() == 0){
        ui->buttonLastImage->setEnabled(false);
        ui->actionLastImage->setEnabled(false);
    }
    else{
        ui->buttonNextImage->setEnabled(true);
        ui->actionNextImage->setEnabled(true);
        ui->buttonLastImage->setEnabled(true);
        ui->actionLastImage->setEnabled(true);
    }
}

/*--------------WindowMain::slotChooseLastImage()
    brief       图片切换（上一张）槽函数
    param       void
    return      void
    description 切换上一张图片，并执行加载图片槽函数
*/
void WindowMain::slotChooseLastImage()
{
    if(ui->tableFileList->currentRow() == 0){
        return;
    }
    ui->tableFileList->clearSelection();
    ui->tableFileList->setCurrentCell(ui->tableFileList->currentRow()-1,0);
    ui->tableFileList->currentItem()->setSelected(true);
    slotLoad();
}

/*--------------WindowMain::slotChooseNextImage()
    brief       图片切换（下一张）槽函数
    param       void
    return      void
    description 切换下一张图片，并执行加载图片槽函数
*/
void WindowMain::slotChooseNextImage()
{
    if(ui->tableFileList->currentRow() == ui->tableFileList->rowCount()-1){
        return;
    }
    ui->tableFileList->clearSelection();
    ui->tableFileList->setCurrentCell(ui->tableFileList->currentRow()+1,0);
    ui->tableFileList->currentItem()->setSelected(true);
    slotLoad();
}

///============ 标注框操作响应槽函数

/*--------------WindowMain::slotRectSelect(int recSelected)
    brief       标注框选中响应槽函数（重载）
    param       int recSelected:选中标注框的序号
    return      void
    description 本函数为重载函数，用于响应标注框列表内的选中操作。在列表内进行选择操作时，同时改变其它关联部件选中的标注框。
*/
void WindowMain::slotRectSelect(int recSelected)
{
    if(listRect.count() > recSelected){
        if(!listRect.value(recSelected)->recIsSelected()){
            imageScene->clearSelection();
            listRect.value(recSelected)->blockSignals(true);
            ui->tableCoordinateDisplay->clearSelection();
            listRect.value(recSelected)->setSelected(true);
            listRect.value(recSelected)->blockSignals(false);
            slotSpinValueChange(listRect.value(recSelected));
            currentRect = listRect.value(recSelected);
            ui->boxCoordinateSet->setEnabled(true);
            ui->buttonDeleteLabel->setEnabled(true);
            ui->actionDeleteLabel->setEnabled(true);
        }
    }
}

/*--------------WindowMain::slotRectSelect(ItemRect* recSelected)
    brief       标注框选中响应槽函数（重载）
    param       ItemRect* recSelected:选中标注框对象
    return      void
    description 本函数为重载函数，用于响应显示区域内的选中操作。在显示区域进行选择操作时，同时改变其它关联部件选中的标注框。
*/
void WindowMain::slotRectSelect(ItemRect* recSelected)
{
    ui->tableCoordinateDisplay->blockSignals(true);
    ui->tableCoordinateDisplay->clearSelection();
    ui->tableCoordinateDisplay->setCurrentItem(recSelected->recTable);
    recSelected->recTable->setSelected(true);
    ui->tableCoordinateDisplay->blockSignals(false);
    slotSpinValueChange(recSelected);
    currentRect = recSelected;
    ui->boxCoordinateSet->setEnabled(true);
    ui->buttonDeleteLabel->setEnabled(true);
    ui->actionDeleteLabel->setEnabled(true);
}

/*--------------WindowMain::slotRectChange()
    brief       标注框变形响应函数
    param       void
    return      void
    description 本函数用于响应举行框的变形操作，使更新后的标注框坐标实时显示在关联部件上
*/
void WindowMain::slotRectChange()
{
    if(currentRect != nullptr){
        currentRect->blockSignals(true);
        if(sender()==ui->spinCoordUpLeftX || sender()==ui->spinCoordUpLeftY){
            currentRect->setRecPointValue(WindowMain::TopLeft,QPointF(
                        ui->spinCoordUpLeftX->value(),ui->spinCoordUpLeftY->value()));
        }
        else if(sender()==ui->spinCoordUpRightX || sender()==ui->spinCoordUpRightY){
            currentRect->setRecPointValue(WindowMain::TopRight,QPointF(
                        ui->spinCoordUpRightX->value(),ui->spinCoordUpRightY->value()));
        }
        else if(sender()==ui->spinCoordDownLeftX || sender()==ui->spinCoordDownLeftY){
            currentRect->setRecPointValue(WindowMain::BottomLeft,QPointF(
                        ui->spinCoordDownLeftX->value(),ui->spinCoordDownLeftY->value()));
        }
        else if(sender()==ui->spinCoordDownRightX || sender()==ui->spinCoordDownRightY){
            currentRect->setRecPointValue(WindowMain::BottomRight,QPointF(
                        ui->spinCoordDownRightX->value(),ui->spinCoordDownRightY->value()));
        }
        currentRect->blockSignals(false);
    }
}

/*--------------WindowMain::slotSpinValueChange(ItemRect* current)
    brief       精确控制数值调整槽函数
    param       ItemRect*current:当前选中标注框对象
    return      void
    description 本函数用于在标注框坐标变化时，实时更新关联部件上的坐标
*/
void WindowMain::slotSpinValueChange(ItemRect* current)
{
    ui->spinCoordUpLeftX->blockSignals(true);
    ui->spinCoordUpLeftY->blockSignals(true);
    ui->spinCoordUpRightX->blockSignals(true);
    ui->spinCoordUpRightY->blockSignals(true);
    ui->spinCoordDownLeftX->blockSignals(true);
    ui->spinCoordDownLeftY->blockSignals(true);
    ui->spinCoordDownRightX->blockSignals(true);
    ui->spinCoordDownRightY->blockSignals(true);
    ui->spinCoordUpLeftX->setValue(int(current->recPointValue(WindowMain::TopLeft).x()));
    ui->spinCoordUpRightX->setValue(int(current->recPointValue(WindowMain::TopRight).x()));
    ui->spinCoordDownLeftX->setValue(int(current->recPointValue(WindowMain::BottomLeft).x()));
    ui->spinCoordDownRightX->setValue(int(current->recPointValue(WindowMain::BottomRight).x()));
    ui->spinCoordUpLeftY->setValue(int(current->recPointValue(WindowMain::TopLeft).y()));
    ui->spinCoordUpRightY->setValue(int(current->recPointValue(WindowMain::TopRight).y()));
    ui->spinCoordDownLeftY->setValue(int(current->recPointValue(WindowMain::BottomLeft).y()));
    ui->spinCoordDownRightY->setValue(int(current->recPointValue(WindowMain::BottomRight).y()));
    ui->spinCoordUpLeftX->blockSignals(false);
    ui->spinCoordUpLeftY->blockSignals(false);
    ui->spinCoordUpRightX->blockSignals(false);
    ui->spinCoordUpRightY->blockSignals(false);
    ui->spinCoordDownLeftX->blockSignals(false);
    ui->spinCoordDownLeftY->blockSignals(false);
    ui->spinCoordDownRightX->blockSignals(false);
    ui->spinCoordDownRightY->blockSignals(false);
}

/*--------------WindowMain::slotAddLabel()
    brief       添加标注框槽函数
    param       void
    return      void
    description 本函数用于在显示界面添加一个标注框，并在关联部件上更新信息
*/
void WindowMain::slotAddLabel()
{
    QPointF viewCenter = ui->graphicsView->mapToScene(QPoint(ui->graphicsView->width()/2,
                                                             ui->graphicsView->height()/2));
    listRect.append(new ItemRect(QString("%1,%2,%3,%2,%3,%4,%1,%4")
                                 .arg(0.8*viewCenter.x()).arg(0.8*viewCenter.y())
                                 .arg(1.2*viewCenter.x()).arg(1.2*viewCenter.y()),imageScene));
    connect(listRect.last(),SIGNAL(recSelected(ItemRect*)),this,SLOT(slotRectSelect(ItemRect*)));
    connect(listRect.last(),SIGNAL(recMoved(ItemRect*)),this,SLOT(slotSpinValueChange(ItemRect*)));
    ui->tableCoordinateDisplay->blockSignals(true);
    ui->tableCoordinateDisplay->setRowCount(ui->tableCoordinateDisplay->rowCount()+1);
    ui->tableCoordinateDisplay->setItem(ui->tableCoordinateDisplay->rowCount()-1,0,listRect.last()->recTable);
    ui->tableCoordinateDisplay->blockSignals(false);
    ui->labelCoordinateList->setText(QString("%1 ").arg(ui->tableCoordinateDisplay->rowCount()).append("Labels  "));
}

/*--------------WindowMain::slotDeleteLabel()
    brief       删除标注框槽函数
    param       void
    return      void
    description 本函数用于在显示界面删除一个标注框，并在关联部件上更新信息
*/
void WindowMain::slotDeleteLabel()
{
    if(listRect.count() > ui->tableCoordinateDisplay->currentRow() &&
            ui->tableCoordinateDisplay->currentRow() >= 0){
        int rectIndex = ui->tableCoordinateDisplay->currentRow();
        ui->tableCoordinateDisplay->blockSignals(true);
        listRect.value(rectIndex)->blockSignals(true);
        listRect.value(rectIndex)->recRemove(imageScene);
        delete listRect.value(rectIndex);
        listRect.removeAt(rectIndex);
        ui->tableCoordinateDisplay->removeRow(rectIndex);
        ui->tableCoordinateDisplay->blockSignals(false);
    }
        ui->labelCoordinateList->setText(QString("%1 ").arg(ui->tableCoordinateDisplay->rowCount()).append("Labels  "));
        ui->boxCoordinateSet->setEnabled(false);
        ui->buttonDeleteLabel->setEnabled(false);
        ui->actionDeleteLabel->setEnabled(false);
}

/*--------------WindowMain::slotClearLabels()
    brief       清空标注框槽函数
    param       void
    return      void
    description 本函数用于清空显示界面的标注框，并在关联部件上更新信息
*/
void WindowMain::slotClearLabels()
{
    qDeleteAll(listRect);
    listRect.clear();
    imageScene->clear();
    ui->tableCoordinateDisplay->blockSignals(true);
    ui->tableCoordinateDisplay->clearContents();
    ui->tableCoordinateDisplay->setRowCount(0);
    ui->tableCoordinateDisplay->blockSignals(false);
    imageScene->addPixmap(*imagePix);
    ui->labelCoordinateList->setText(QString("%1 ").arg(ui->tableCoordinateDisplay->rowCount()).append("Labels  "));
    ui->boxCoordinateSet->setEnabled(false);
}

/*--------------WindowMain::slotAddLabel()
    brief       更新标注框槽函数
    param       void
    return      void
    description 本函数使用EAST模型生成标注，更新显示界面上的标注框，并在关联部件上更新信息
*/
void WindowMain::slotUpdateLabels()
{
    slotClearLabels();
    appTimer.restart();
    QStringList listLabel = labelDetector->detect(dirFile->path().append("/").append(
                                ui->tableFileList->currentItem()->text()).toLocal8Bit().toStdString());
    ui->labelDisplayTime->setText(QString("%1 ms  | ").arg(appTimer.elapsed()));
    for(int cnt = 0; cnt < listLabel.count(); cnt++){
        listRect.append(new ItemRect(listLabel.value(cnt).remove("-"),imageScene));
        connect(listRect.last(),SIGNAL(recSelected(ItemRect*)),this,SLOT(slotRectSelect(ItemRect*)));
        connect(listRect.last(),SIGNAL(recMoved(ItemRect*)),this,SLOT(slotSpinValueChange(ItemRect*)));
    }
    ui->tableCoordinateDisplay->setRowCount(listRect.count());
    for(int cnt = 0; cnt < listRect.count(); cnt++){
        ui->tableCoordinateDisplay->setItem(cnt,0,listRect.value(cnt)->recTable);
    }
    ui->labelCoordinateList->setText(QString("%1 ").arg(ui->tableCoordinateDisplay->rowCount()).append("Labels  "));
    imageScene->update(imageScene->sceneRect());
}

/*--------------WindowMain::slotSaveLabel()
    brief       保存标注文件槽函数
    param       void
    return      void
    description 本函数用于将当前显示的标注框生成标注文件并保存
*/
void WindowMain::slotSaveLabel()
{
    QFile fileWrite(dirFile->path().append("/").append(
                       ui->tableFileList->currentItem()->text().split(".").first().append(".txt")));
    if(fileWrite.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream labelOutput(&fileWrite);
        for(int cnt = 0; cnt < ui->tableCoordinateDisplay->rowCount(); cnt++){
            labelOutput << ui->tableCoordinateDisplay->item(cnt,0)->
                           text().remove(" ").remove("(").remove(")") << endl;
        }
        fileWrite.close();
        ui->tableFileList->item(ui->tableFileList->currentRow(),1)->setText("Labeled");
    }
    slotChooseNextImage();
}

///============ 显示界面操作响应槽函数

/*--------------WindowMain::slotZoomScaleDisplay(double zoomScale)
    brief       缩放比例显示槽函数
    param       double zoomScale:当前缩放比例
    return      void
    description 本函数用于在显示界面缩放比例更新时，显示新的缩放比例信息
*/
void WindowMain::slotZoomScaleDisplay(double zoomScale)
{
    int zoomValue = int(zoomScale*100);
    ui->labelDisplayZoom->setText(QString("%1 %").arg(zoomValue));
    ui->dialZoom->blockSignals(true);
    ui->dialZoom->setValue(zoomValue);
    ui->dialZoom->blockSignals(false);
    ui->actionZoomIn->setEnabled(zoomValue == 500 ? false : true);
    ui->actionZoomOut->setEnabled(zoomValue == 10 ? false : true);
    ui->actionOriginalSize->setEnabled(zoomValue == 100 ? false : true);
    ui->actionBestFit->setEnabled(true);
}

///============ 实时标注操作响应槽函数

/*--------------WindowMain::slotCaptureRefresh()
    brief       实时显示界面刷新槽函数
    param       void
    return      void
    description 本函数根据定时器设定，定时刷新显示界面
*/
void WindowMain::slotCaptureRefresh()
{
    *captureCamera >> captureFrame;
    if (!captureFrame.empty())
    {
        appTimer.restart();
        QImage captureImage;
        QVector<QPolygonF> polygonCapture;
        polygonCapture = labelDetector->detectCapture(captureFrame);
        cv::cvtColor(captureFrame,captureFrame,cv::ColorConversionCodes::COLOR_BGR2RGB);
        captureImage = QImage(static_cast<const unsigned char*>(captureFrame.data),
                    captureFrame.cols, captureFrame.rows,
                    captureFrame.cols*captureFrame.channels(),
                    QImage::Format_RGB888);

        captureScene->clear();
        captureScene->addPixmap(QPixmap().fromImage(captureImage));
        for(int cnt = 0; cnt < polygonCapture.count(); cnt++){
            captureScene->addPolygon(polygonCapture.value(cnt),*capturePen,*captureBrush);
        }
        captureScene->update();
        ui->labelDisplayTime->setText(QString("FPS: %1  ").arg(1000/double(appTimer.elapsed()),0,'g',2));
    }
}

/*--------------WindowMain::slotCapture()
    brief       拍照操作槽函数
    param       void
    return      void
    description 本函数执行拍照操作
*/
void WindowMain::slotCapture()
{
    *captureCamera >> captureFrame;
    captureTimer->stop();
    ui->buttonCapture->setVisible(false);
    ui->buttonFinish->setVisible(true);
    ui->buttonDiscard->setVisible(true);
    ui->actionSaveCapture->setEnabled(true);
    ui->actionCapture->setEnabled(false);
    ui->labelDisplayTime->setText("");
}

/*--------------WindowMain::slotSaveCapture()
    brief       拍照保存槽函数
    param       void
    return      void
    description 本函数保存拍照操作得到的图片及图片对应的标注文件
*/
void WindowMain::slotSaveCapture()
{

    dirCapture->setPath(QFileDialog::getExistingDirectory(this, tr("Choose Save Directory"),
                                                           dirCapture->path(),
                                                           QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks));

    if(dirCapture->isAbsolute()){
        if (!captureFrame.empty())
        {
            QImage captureImage;
            QVector<QPolygonF> polygonCapture;
            polygonCapture = labelDetector->detectCapture(captureFrame);
            cv::cvtColor(captureFrame,captureFrame,cv::ColorConversionCodes::COLOR_BGR2RGB);
            captureImage = QImage(static_cast<const unsigned char*>(captureFrame.data),
                        captureFrame.cols, captureFrame.rows,
                        captureFrame.cols*captureFrame.channels(),
                        QImage::Format_RGB888);
            QString fileName = dirCapture->path().append("/capture_").append(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
            captureImage.save(fileName + ".jpg");
            QStringList recList = labelDetector->detect((fileName + ".jpg").toLocal8Bit().toStdString());
            QFile fileWrite(fileName + ".txt");
            if(fileWrite.open(QIODevice::WriteOnly|QIODevice::Text)){
                QTextStream labelOutput(&fileWrite);
                for(int cnt = 0; cnt < recList.count(); cnt++){
                    labelOutput << recList.value(cnt) << endl;
                }
                fileWrite.close();
            }
        }
    }
    captureTimer->start();
    ui->buttonCapture->setVisible(true);
    ui->buttonFinish->setVisible(false);
    ui->buttonDiscard->setVisible(false);
    ui->actionSaveCapture->setEnabled(false);
    ui->actionCapture->setEnabled(true);

}

/*--------------WindowMain::slotDiscardCapture()
    brief       拍照放弃槽函数
    param       void
    return      void
    description 本函数放弃已拍摄照片，继续显示实时标注界面
*/
void WindowMain::slotDiscardCapture()
{
    captureTimer->start();
    ui->buttonCapture->setVisible(true);
    ui->buttonFinish->setVisible(false);
    ui->buttonDiscard->setVisible(false);
    ui->actionSaveCapture->setEnabled(false);
    ui->actionCapture->setEnabled(true);

}

///============ EAST模型设置响应槽函数

/*--------------WindowMain::slotEASTSettingChange(int value)
    brief       EAST设置更改槽函数
    param       int value:更改后的对应数值
    return      void
    description 本函数在EAST设置更改后，将更改值显示在界面上（在未选择“应用”之前，模型的参数并未更改）
*/
void WindowMain::slotEASTSettingChange(int value)
{
    ui->buttonEASTSet->setEnabled(true);
    ui->buttonDefaultSet->setEnabled(true);
    if(sender() == ui->sliderSettingConf){
        ui->labelSettingConf->setText(tr("EAST Thershold: ").append("%1").arg(6+static_cast<double>(value)/200,0,'g',1));
    }
    else if(sender() == ui->sliderSettingNms){
        ui->labelSettingNms->setText(tr("NMS Thershold: ").append("%1").arg(3+static_cast<double>(value)/40,0,'g',1));
    }
}

/*--------------WindowMain::slotEASTSettingApply()
    brief       EAST设置应用槽函数
    param       void
    return      void
    description 本函数将EAST参数更改为当前执行更改操作后的新数值
*/
void WindowMain::slotEASTSettingApply()
{
    ui->buttonEASTSet->setEnabled(false);
    ui->boxEast->setVisible(false);
    labelDetector->setParam(static_cast<float>(sigmoid(ui->sliderSettingConf->value()/100)),
                                static_cast<float>(sigmoid(ui->sliderSettingNms->value()/70)),
                                64 * (ui->comboSettingSize->currentIndex()+1),
                                64 * (ui->comboSettingSize->currentIndex()+1));
}

/*--------------WindowMain::slotEASTSettingDefault()
    brief       EAST默认设置槽函数
    param       void
    return      void
    description 本函数将界面显示的EAST参数更改为默认值（在未选择“应用”之前，模型的参数并未更改）
*/
void WindowMain::slotEASTSettingDefault()
{
    ui->sliderSettingConf->setValue(0);
    ui->sliderSettingNms->setValue(0);
    ui->comboSettingSize->setCurrentIndex(4);
    ui->buttonDefaultSet->setEnabled(false);
    ui->buttonEASTSet->setEnabled(true);
}

/*--------------WindowMain::slotEASTSettingDisplay()
    brief       EAST设置区域显示槽函数
    param       void
    return      void
    description 本函数控制EAST设置区域是否显示在界面上。若已显示则隐藏；若未显示则显示
*/
void WindowMain::slotEASTSettingDisplay()
{
    ui->boxEast->setVisible(ui->boxEast->isVisible() ? false : true);
}

/*--------------WindowMain::slotEASTPathChange()
    brief       EAST模型路径设置槽函数
    param       void
    return      void
    description 本函数打开文件选择对话框，将模型更新为新选中的的模型文件
*/
void WindowMain::slotEASTPathChange()
{
    QString oldModel = modelFile->filePath();
    modelFile->setFile(QFileDialog::getOpenFileName(this, tr("Choose EAST model"),
                                             modelFile->filePath(),
                                             tr("Tensorflow Frozen Model (*.pb)")));
    if(modelFile->isFile()){
        labelDetector->setNet(modelFile->filePath().toLocal8Bit().toStdString());
        while(!labelDetector->isAvailable()){
            QMessageBox::critical(nullptr, tr("Error"), tr("EAST Model is unavailable"), QMessageBox::Yes);
            modelFile->setFile(QFileDialog::getOpenFileName(this, tr("Choose EAST model"),
                                                     modelFile->filePath(),
                                                     tr("Tensorflow Frozen Model (*.pb)")));
            if(!modelFile->isFile()){
                modelFile->setFile(oldModel);
                break;
            }
            labelDetector->setNet(modelFile->filePath().toLocal8Bit().toStdString());
        }
    }
    else{
        modelFile->setFile(oldModel);
    }
}

///============ 应用软件操作槽函数

/*--------------WindowMain::slotModeChange()
    brief       模式选择槽函数
    param       void
    return      void
    description 本函数实现文件标注模式和实时检测模式之间的转换
*/
void WindowMain::slotModeChange()
{
    captureTimer->stop();
    if (captureCamera->isOpened())
        captureCamera->release();

    if(sender() == ui->buttonFileMode){
        ui->boxFile->setEnabled(true);
        ui->boxOperation->setVisible(false);
        ui->labelDisplayTime->setText("");
        ui->actionOpen->setEnabled(true);
        actionGroupCapture->setEnabled(false);
        actionGroupCapture->setVisible(false);
        actionGroupFile->setVisible(true);
        ui->stackedOperation->setCurrentWidget(ui->pageFileMode);
        ui->graphicsView->setScene(imageScene);
        ui->graphicsView->setSceneRect(imageScene->sceneRect());
        ui->graphicsView->setZoomable(true);
        slotEASTSettingDefault();
        slotEASTSettingApply();
        slotOpen();
    }
    else if(sender() == ui->buttonCameraMode){
        if(!captureCamera->open(0)){
            QMessageBox::critical(nullptr, tr("Critical"), tr("No camera."), QMessageBox::Yes);
            return;
        }
        ui->boxCoordinateList->setVisible(false);
        ui->boxCoordinateSet->setVisible(false);
        ui->boxFile->setEnabled(false);
        ui->boxOperation->setVisible(true);

        ui->buttonCapture->setVisible(true);
        ui->buttonFinish->setVisible(false);
        ui->buttonDiscard->setVisible(false);
        ui->buttonSaveNext->setEnabled(false);
        ui->labelDisplayInfo->setVisible(false);
        ui->labelDisplayZoom->setVisible(false);
        ui->labelDisplayTime->setText("");
        ui->labelFileInfo->setText("");
        ui->actionOpen->setEnabled(false);
        ui->actionSaveCapture->setEnabled(false);
        ui->dialZoom->setEnabled(false);

        actionGroupFile->setEnabled(false);
        actionGroupFile->setVisible(false);
        actionGroupCapture->setEnabled(true);
        actionGroupCapture->setVisible(true);
        actionGroupLabel->setEnabled(false);
        actionGroupZoom->setEnabled(false);

        qDeleteAll(listRect);
        listRect.clear();
        imageScene->clear();
        ui->tableCoordinateDisplay->blockSignals(true);
        ui->tableCoordinateDisplay->clearContents();
        ui->tableCoordinateDisplay->setRowCount(0);
        ui->tableCoordinateDisplay->blockSignals(false);
        ui->tableFileList->blockSignals(true);
        ui->tableFileList->clearContents();
        ui->tableFileList->setRowCount(0);
        ui->tableFileList->blockSignals(false);
        ui->stackedOperation->setCurrentWidget(ui->pageCameraMode);
        ui->comboSettingSize->setCurrentIndex(2);
        slotEASTSettingApply();

        cv::Mat frame;
        *captureCamera >> frame;
        captureScene->setSceneRect(0,0,frame.cols, frame.rows);
        ui->graphicsView->setScene(captureScene);
        ui->graphicsView->setSceneRect(0,0,captureScene->width(),captureScene->height());
        ui->graphicsView->setZoomable(false);
        captureTimer->start();
    }
    ui->boxCoordinateSet->setEnabled(false);
    ui->boxCoordinateList->setEnabled(false);
    ui->graphicsView->zoomBestFit();
    QMainWindow::resizeEvent(nullptr);
}

/*--------------WindowMain::slotAbout()
    brief       关于界面显示槽函数
    param       void
    return      void
    description 本函数显示软件的关于界面
*/
void WindowMain::slotAbout()
{
    new DialogAbout(this,qssTheme);
}

/*--------------WindowMain::slotCloseEvent() 
    brief       关闭窗口事件槽函数
    param       void
    return      void
    description 本函数用于执行关闭窗口的操作
*/
void WindowMain::slotCloseEvent() 
{
    slotSaveSettings();
    close();
}

/*--------------WindowMain::slotMinimumEvent() 
    brief       最小化窗口事件槽函数
    param       void
    return      void
    description 本函数用于执行最小化窗口的操作
*/
void WindowMain::slotMinimumEvent() 
{
    showMinimized();
}

/*--------------WindowMain::slotSizeControlEvent(bool checked) 
    brief       窗口大小更改事件槽函数
    param       bool checked:最大化/正常大小按钮的当前状态
    return      void
    description 本函数用于执行最大化窗口和正常大小窗口直接切换的操作
*/
void WindowMain::slotSizeControlEvent(bool checked) 
{
    if(checked){
        showMaximized();
    }
    else{
        showNormal();
    }
}

/*--------------WindowMain::slotWindowMove(QPoint pos)
    brief       窗口拖动槽函数
    param       QPoint pos:拖动目标坐标
    return      void
    description 本函数用于执行拖动窗口操作
*/
void WindowMain::slotWindowMove(QPoint pos)
{
    move(this->pos() + pos);
}


/*--------------WindowMain::slotThemeChange()
    brief       主题切换槽函数
    param       void
    return      void
    description 本函数用于执行切换窗口主题操作
*/
void WindowMain::slotThemeChange()
{
    qssTheme = (qssTheme + 1) % 7;
    QFile file(QString(":/qss/themes/Default-").append("%1").arg(qssTheme).append(".qss"));
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

/*--------------WindowMain::slotSaveSettinsg()
    brief       用户设置保存槽函数
    param       void
    return      void
    description 本函数用于保存当前用户设置
*/
void WindowMain::slotSaveSettings() 
{
    settingsWindow->setValue("maximized", isMaximized());
    if (!isMaximized()) {
        settingsWindow->setValue("pos", pos());
        settingsWindow->setValue("size", size());

    }
    settingsWindow->setValue("modelPath", modelFile->filePath());
    settingsWindow->setValue("theme", qssTheme);
    QString language = settingsWindow->value("language","en").toString();
    settingsWindow->setValue("language", language);
}

/////////====== Protected Functions

/*-------------WindowMain::resizeEvent(QResizeEvent *event)
    brief       窗口对象变形事件处理函数（覆写QMainWindow::resizeEvent）
    param       QResizeEvent *eventevent:触发的窗口对象变形事件
    return      void
    description 调整显示区域内的缩放比例，使显示对象在调整窗口大小后仍适应屏幕
*/
void WindowMain::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(ui->graphicsView != nullptr){
        ui->graphicsView->zoomBestFit();
    }
}

/////////====== Private Functions

/*--------------WindowMain::spinRangeChange(QRect range)
    brief       精确控制范围调整函数
    param       QRect rang:新图片的可控制矩形范围
    return      void
    description 本函数用于在加载新图片且图片大小改变时，调整精确控制框的调整范围，避免越界
*/
void WindowMain::spinRangeChange(QRect range)
{
    ui->spinCoordUpLeftX->blockSignals(true);
    ui->spinCoordUpLeftY->blockSignals(true);
    ui->spinCoordUpRightX->blockSignals(true);
    ui->spinCoordUpRightY->blockSignals(true);
    ui->spinCoordDownLeftX->blockSignals(true);
    ui->spinCoordDownLeftY->blockSignals(true);
    ui->spinCoordDownRightX->blockSignals(true);
    ui->spinCoordDownRightY->blockSignals(true);
    ui->spinCoordUpLeftX->setRange(0,range.width());
    ui->spinCoordUpRightX->setRange(0,range.width());
    ui->spinCoordDownLeftX->setRange(0,range.width());
    ui->spinCoordDownRightX->setRange(0,range.width());
    ui->spinCoordUpLeftY->setRange(0,range.height());
    ui->spinCoordUpRightY->setRange(0,range.height());
    ui->spinCoordDownLeftY->setRange(0,range.height());
    ui->spinCoordDownRightY->setRange(0,range.height());
    ui->spinCoordUpLeftX->blockSignals(false);
    ui->spinCoordUpLeftY->blockSignals(false);
    ui->spinCoordUpRightX->blockSignals(false);
    ui->spinCoordUpRightY->blockSignals(false);
    ui->spinCoordDownLeftX->blockSignals(false);
    ui->spinCoordDownLeftY->blockSignals(false);
    ui->spinCoordDownRightX->blockSignals(false);
    ui->spinCoordDownRightY->blockSignals(false);
}

/*--------------WindowMain::sigmoid(double x)
    brief       Sigmoid函数
    param       double x:输入值
    return      double: 输入值的sigmoid函数值
*/
double WindowMain::sigmoid(double x)
{
    return 1/(1+qExp(-x));
}