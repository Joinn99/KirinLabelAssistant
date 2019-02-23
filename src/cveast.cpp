/*==================Kirin Label Assistant================/

    file            cveast.cpp
    brief           EAST模型对象
    description	    EAST模型使用opencv加载的对象，可进行参数调整、路径设置等选项，进行图片文字检测的操作
                    参考自OPencv官方开源示例: https://github.com/opencv/opencv/blob/master/samples/dnn/text_detection.cpp
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "cveast.h"

/////////====== Public Functions

/*--------------CVEast::CVEast(String modelPath)
    brief       类构造函数
    param       String modelPath:模型路径
*/
CVEast::CVEast(String modelPath)
{
    net = dnn::readNetFromTensorflow(modelPath);

    outs = std::vector<Mat>();

    outNames = std::vector<String>(2);
    outNames[CVEast::Score] = "feature_fusion/Conv_7/Sigmoid";
    outNames[CVEast::Geometry] = "feature_fusion/concat_3";

    sizeParam = std::vector<int>(2);
    sizeParam[CVEast::InputWidth] = 320;
    sizeParam[CVEast::InputHeight] = 320;

    thresParam = std::vector<float>(2);
    thresParam[CVEast::EASTThreshold] = 0.5f;
    thresParam[CVEast::NMSThreshold] = 0.4f;
    this->eastPath = modelPath;
}

/*--------------CVEast::detect(String imagePath)
    brief       文件标注检测函数
    param       String imagePath:图片路径
    return      QStringList:输出坐标字符串列表
    Description	根据输入图片路径和输入参数，返回对应标注框的坐标字符串列表
*/
QStringList CVEast::detect(String imagePath)
{
    Mat image = imread(imagePath);
    Mat blob;
    if(image.empty()) {return QStringList();}
    dnn::blobFromImage(image, blob, 1.0,
                  Size(sizeParam[CVEast::InputWidth], sizeParam[CVEast::InputHeight]),
                  Scalar(123.68, 116.78, 103.94), true, false);
    net.setInput(blob);
    net.forward(outs, outNames);

    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
    decode(outs[CVEast::Score], outs[CVEast::Geometry],
            thresParam[CVEast::EASTThreshold], boxes, confidences);

    std::vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, thresParam[CVEast::EASTThreshold],
            thresParam[CVEast::NMSThreshold], indices);

    Point2f ratio(static_cast<float>(image.cols) / sizeParam[CVEast::InputWidth],
                  static_cast<float>(image.rows) / sizeParam[CVEast::InputHeight]);

    QStringList result;

    for (size_t i = 0; i < indices.size(); ++i)
    {
        RotatedRect& box = boxes[static_cast<unsigned long long>(indices[i])];
        Point2f vertices[4];
        box.points(vertices);
        for (int j = 0; j < 4; ++j)
        {
            vertices[j].x *= ratio.x;
            vertices[j].y *= ratio.y;
        }
        result.append(QString("%1,%2,%3,%4,%5,%6,%7,%8").arg(int(vertices[1].x)).arg(int(vertices[1].y)).
                arg(int(vertices[2].x)).arg(int(vertices[2].y)).arg(int(vertices[3].x)).
                arg(int(vertices[3].y)).arg(int(vertices[0].x)).arg(int(vertices[0].y)));
    }

    return result;
}

/*--------------CVEast::detectCapture(cv::Mat image)
    brief       实时检测检测函数
    param       cv::Mat image:图片对应矩阵
    return      QVector<QPolygonF>:输出标注框对应多边形序列
    Description	根据输入图片的矩阵，返回标注框的多边形序列
*/
QVector<QPolygonF> CVEast::detectCapture(cv::Mat image)
{
    Mat blob;
    if(image.empty()) {return QVector<QPolygonF>();}
    dnn::blobFromImage(image, blob, 1.0,
                  Size(sizeParam[CVEast::InputWidth], sizeParam[CVEast::InputHeight]),
                  Scalar(123.68, 116.78, 103.94), true, false);
    net.setInput(blob);
    net.forward(outs, outNames);

    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
    decode(outs[CVEast::Score], outs[CVEast::Geometry],
            thresParam[CVEast::EASTThreshold], boxes, confidences);

    std::vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, thresParam[CVEast::EASTThreshold],
            thresParam[CVEast::NMSThreshold], indices);

    Point2f ratio(static_cast<float>(image.cols) / sizeParam[CVEast::InputWidth],
                  static_cast<float>(image.rows) / sizeParam[CVEast::InputHeight]);

    QVector<QPolygonF> result;

    for (size_t i = 0; i < indices.size(); ++i){
        RotatedRect& box = boxes[static_cast<unsigned long long>(indices[i])];
        Point2f vertices[4];
        box.points(vertices);
        QPolygonF resultPolygon;
        for (int j = 0; j < 4; ++j)
        {
            vertices[j].x *= ratio.x;
            vertices[j].y *= ratio.y;
            resultPolygon << QPointF(double(vertices[j].x),double(vertices[j].y));
        }
        result.append(resultPolygon);
    }
    return result;
}

/*--------------CVEast::decode(const Mat& scores, const Mat& geometry, float scoreThresh,
                    std::vector<RotatedRect>& detections, std::vector<float>& confidences)
    brief       模型输出解析函数
    param       const Mat& scores: 输出置信度矩阵	const Mat& geometry:输出形状矩阵	float scoreThresh:置信度门限值
                std::vector<RotatedRect>& detections	标注旋转矩形序列	std::vector<float>& confidences:输出置信度序列
    return      void
    Description	将模型输出的节点值转换为标注框序列
*/
void CVEast::decode(const Mat& scores, const Mat& geometry, float scoreThresh,
    std::vector<RotatedRect>& detections, std::vector<float>& confidences)
{
    detections.clear();
    CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
    CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
    CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

    const int height = scores.size[2];
    const int width = scores.size[3];
    for (int y = 0; y < height; ++y)
    {
        const float* scoresData = scores.ptr<float>(0, 0, y);
        const float* x0_data = geometry.ptr<float>(0, 0, y);
        const float* x1_data = geometry.ptr<float>(0, 1, y);
        const float* x2_data = geometry.ptr<float>(0, 2, y);
        const float* x3_data = geometry.ptr<float>(0, 3, y);
        const float* anglesData = geometry.ptr<float>(0, 4, y);
        for (int x = 0; x < width; ++x)
        {
            float score = scoresData[x];
            if (score < scoreThresh){
                continue;
            }
            // Decode a prediction.
            // Multiple by 4 because feature maps are 4 time less than input image.
            float offsetX = x * 4.0f, offsetY = y * 4.0f;
            float angle = anglesData[x];
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            float h = x0_data[x] + x2_data[x];
            float w = x1_data[x] + x3_data[x];

            Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            Point2f p1 = Point2f(-sinA * h, -cosA * h) + offset;
            Point2f p3 = Point2f(-cosA * w, sinA * w) + offset;
            RotatedRect r(0.5f * (p1 + p3), Size2f(w, h), -angle * 180.0f / PI);
            detections.push_back(r);
            confidences.push_back(score);
        }
    }
}

/*--------------CVEast::setNet(String modelPath)
    brief       模型路径设置函数
    param       String modelPath:模型路径
    return      void
    Description	更改模型路径
*/
void CVEast::setNet(String modelPath)
{
    net = dnn::readNetFromTensorflow(modelPath);
    this->eastPath = modelPath;
}

/*--------------CVEast::setParam(float eastThers, float nmsThres, int inputWidth, int inputHeight)
    brief       模型参数设置函数
    param       float eastThers:置信度门限值	nmsThres:聚合门限值	   int inputWidth:输入宽度	 int inputHeight:输入高度   
    return      void
    Description	更改模型路径
*/
void CVEast::setParam(float eastThers, float nmsThres,
                      int inputWidth, int inputHeight)
{
    thresParam[CVEast::EASTThreshold] = eastThers;
    thresParam[CVEast::NMSThreshold] = nmsThres;

    sizeParam[CVEast::InputWidth] = inputWidth;
    sizeParam[CVEast::InputHeight] = inputHeight;
    net = dnn::readNet(eastPath);
}

/*--------------CVEast::resetParam()
    brief       模型参数重置函数
    param       void
    return      void
    Description	更改模型路径
*/
void CVEast::resetParam()
{
    thresParam[CVEast::EASTThreshold] = 0.5f;
    thresParam[CVEast::NMSThreshold] = 0.4f;

    sizeParam[CVEast::InputWidth] = 320;
    sizeParam[CVEast::InputHeight] = 320;

    net = dnn::readNet(eastPath);
}

/*--------------CVEast::EASTThres()
    brief       置信度门限值函数
    param       void
    return      float:置信度门限值
    Description	返回置信度门限值
*/
float CVEast::EASTThres()
{
    return thresParam[CVEast::EASTThreshold];
}

/*--------------CVEast::NMSThres()
    brief       聚合门限值函数
    param       void
    return      float:聚合门限值
    Description	返回聚合门限值
*/
float CVEast::NMSThres()
{
    return thresParam[CVEast::NMSThreshold];
}

/*--------------CVEast::InputW()
    brief       输入宽度函数
    param       void
    return      int:输入宽度
    Description	返回输入宽度
*/
int CVEast::InputW()
{
    return sizeParam[CVEast::InputWidth];
}

/*--------------CVEast::InputH()
    brief       输入高度函数
    param       void
    return      int:输入高度
    Description	返回输入高度
*/
int CVEast::InputH()
{
    return sizeParam[CVEast::InputHeight];
}

/*--------------CVEast::isAvailable()
    brief       模型可用函数
    param       void
    return      bool:模型是否可用
    Description	返回模型是否可用
*/
bool CVEast::isAvailable()
{
    return !net.empty();
}