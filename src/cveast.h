/*==================Kirin Label Assistant================/

    file            cveast.cpp
    brief           EAST模型对象
    description	    EAST模型使用opencv加载的对象，可进行参数调整、路径设置等选项，进行图片文字检测的操作
                    参考自OPencv官方开源示例: https://github.com/opencv/opencv/blob/master/samples/dnn/text_detection.cpp
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

#ifndef CVEAST_H
#define CVEAST_H

/////////====== Includes

#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QStringList>
#include <QPolygonF>

/////////====== Define

#define PI 3.141593f

/////////====== NameSpace

using namespace cv;

/////////====== Class CVEast

class CVEast
{
public:
    CVEast(String modelPath);
    QStringList detect(String imagePath);
    QVector<QPolygonF> detectCapture(cv::Mat image);
    void setNet(String modelPath);
    void setParam(float eastThers, float nmsThres, int inputWidth, int inputHeight);
    void resetParam();
    float EASTThres();
    float NMSThres();
    int InputW();
    int InputH();
    bool isAvailable();
  private:
    enum{Score,Geometry};                                   //枚举：模型输出向量含义
    enum{InputWidth, InputHeight};                          //枚举：输入尺寸向量
    enum{EASTThreshold, NMSThreshold};                      //枚举：门限值参数向量

    void decode(const Mat& scores, const Mat& geometry, float scoreThresh,
        std::vector<RotatedRect>& detections, std::vector<float>& confidences);

    std::vector<Mat> outs;                                  //模型输出
    std::vector<String> outNames;                           //模型输出节点名称
    std::vector<int> sizeParam;                             //模型输入尺寸
    std::vector<float> thresParam;                          //模型门限值
    dnn::Net net;                                           //模型网络
    String eastPath;                                        //模型文件路径
};

#endif // CVEAST_H
