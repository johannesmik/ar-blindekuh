#include "highscorewidget.h"

#include<fstream>


highscorewidget::highscorewidget(QWidget *parent) :
    QListWidget(parent)
{

    this->addItem("Blinde_Kuh\t\t32 Sekunden");
    this->addItem("Some1\t\t60 Sekunden");
    this->addItem("ArDemo\t\t120 Sekunden");

}
