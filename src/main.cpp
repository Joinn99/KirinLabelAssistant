/*==================Kirin Label Assistant================/

    file            main.cpp
    brief           主函数
    description	    实现窗口显示，语言切换
    author          Joinn99	(https://github.com/Joinn99)
    version         V1.0.0 
    date            2019-1-14
*/

/////////====== Includes

#include "windowmain.h"
#include <QApplication>

/////////====== Main Function

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings settingsWindow("KirinSettings.ini", QSettings::IniFormat);
    QString language = settingsWindow.value("language", "en").toString();
    QTranslator translator;
    translator.load("kirinui_"+ language +".qm",":/translations/translations/");
    a.installTranslator(&translator);

    WindowMain w;
    w.show();

    return a.exec();
}
