<img src="https://raw.githubusercontent.com/Joinn99/RepositoryResource/master/KLA/icon.png" width=96 height=96 />

# Kirin Label Assistant
### A scene text detection label helper

![GitHub](https://img.shields.io/github/license/Joinn99/KirinLabelAssistant.svg?label=License) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Joinn99/KirinLabelAssistant.svg) ![OpenCV](https://img.shields.io/badge/OpenCV-4.0.1-red.svg) ![Qt](https://img.shields.io/badge/Qt-5.11.2-brightgreen.svg)
---
Kirin Label Assistant is a deskop cumputer program which helps creating training set for scene text detection. Users can use the program to label their own images efficiently with the help of the automatic label function, which using [EAST](https://arxiv.org/abs/1704.03155v2) neural network to generate labels automatically. Users only need to correct model's wrong label results. 

It is programmed in C++ with [Qt](https://www.qt.io/) framework. The EAST model applied in the program by using [OpenCV](https://opencv.org/) Library.

## Features
The program has two mode: File mode and Camera mode. You can choose the mode when you open the program, or change it any time. 

![](https://raw.githubusercontent.com/Joinn99/RepositoryResource/master/KLA/main.jpg)

When you run the program for the first time, you need to set the EAST model path. This project does not include the model. You can download it on [Google Drive](https://drive.google.com/open?id=14quvrzJ211VePBqFO0oc6vSj5Lg6nAqM) or [BaiduYun](https://pan.baidu.com/s/19BXxaF5WaJLmLrIBMVxkPw), or use your own EAST model.

### File Mode
In file mode, first you should choose a directory which contains several images(support *.jpg, *.png, *.bmp). You will see all images name in the list. If there is a *.txt file has the same name with a image, it will be marked as "Labeled".

Click an image and it will show in the center zone. If it is labeled, KLA will load the label file, or it will generate labels using EAST model.

![](https://raw.githubusercontent.com/Joinn99/RepositoryResource/master/KLA/label.jpg)

The label rectangle will show on the image, their coordinate will list right. You can drag the rectangle to move it, or drag the corner to reshape. You can save the label by clicking "Kirin Label!" or pressing SPACE.

### Camera Mode
In camera mode, program will use webcam to capture the image and do real-time text detection. You can save the image with label files.

![](https://raw.githubusercontent.com/Joinn99/RepositoryResource/master/KLA/camera.jpg)

In both two modes, you can set EAST model parameters. **Input Size** will influence the model's detection speed and precision. **EAST Thershold** and **NMS Thershold** influence the precision. 

![](https://raw.githubusercontent.com/Joinn99/RepositoryResource/master/KLA/set.jpg)

#### Language
This program support English and Simplified Chinese, you can set the language by modifing ```KirinSettings.ini```
```
language=en #English
language=ch_zn #Simplified Chinese
```
## Compiling the project
The project is a C++ project, you can cuild the project using [CMake](https://cmake.org/) and then compile and run it when you solve the dependencies problems.

### Dependencies
KLA needs two main dependencies: OpenCV and Qt Frameforks. You have to make sure these two dependencies are installed correctly on your system. You can compile the project on Linux, Windows and MacOS.

### Versions
+ **Qt:**  5.11.2(Recommanded)  5.4.0(Minimum)
+ **OpenCV:**   4.0.1(Recommanded)  3.4.0(Minimum)

### Compiling on Windows
+ Installing Qt: Compiling the program only need **Qt5Core.dll** and **Qt5Widget.dll**, make sure they are included in your PATH.

+ Installing OpenCV: If you haven't install OpenCV, follow this [page](https://docs.opencv.org/4.0.1/d3/d52/tutorial_windows_install.html) to install it.

Build the project using ```cmake``` or ```cmake-gui``` in ```<KirinLabelAssistant>/build``` directory. If you use ```NMake Makefiles``` as generator, you can compile it with ```nmake``` and find the program in ```<KirinLabelAssistant>/bin```.
### Compiling on Linux
+ Installing Qt: **Qt5Core.dll** and **Qt5Widget.dll** are included in ```Qtbase5-dev```.Install it with
```
sudo apt-get install qtbase5-dev
```
+ Installing OpenCV: KLA uses dnn module in ```opencv_contrib```, which is not available in ```libopencv-dev```. This means you have to compile the OpenCV librarys by yourself. Following [this](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) to compile and install OpenCV plus **OpenCV contrib**.

Once you finish these two dependencies installation, you can compile the project with
```
$ cd <KirinLabelAssistant>/build>
$ cmake ..
$ make
```
And run the program with
```
$ cd <KirinLabelAssistant>/bin>
$ ./KirinLabelAssistant
```
## License
Kirin Label Assistant uses GNU GPL v3.0 License.
Icons are from [Google Material Design Icons](https://github.com/google/material-design-icons) and [iconshock](https://www.iconshock.com/).
