#include "graphics.hpp"
#include "widgets.hpp"
#include <vector>
#include <sstream>
#include "application.hpp"
#include <iostream>
#include <fstream>
#include "counter.h"
#include "choose.h"
#include "button.h"
#include "scroll.hpp"
#include "staticText.hpp"
#include "dinamicText.hpp"
#include "table.h"

using namespace std;
using namespace genv;

int XX=800, YY=500;

struct Tetel{
    string nev;
    int ertek;
};

class MyApplication : public Application
{
protected:
    table * table1;
    staticText * staticText1;
    button * newGameButton;
    button * exitButton;

public:
    MyApplication(){
        table1 = new table(this,40,40,400,400,"George","Michael");
        staticText1 = new staticText(this,500,40,200,10,"Green starts");
        newGameButton = new button(this,500,100,90,30," New Game");
        exitButton = new button(this,600,100,90,30,"   Exit");
    }




    virtual int action()
    {
        staticText1->addElement(table1->value());
        if (newGameButton->changedValue()) {table1->newGame();}
        if (exitButton->changedValue()) {return 0;}
    }
};

int main()
{
    gout.open(XX, YY);
    MyApplication app;
    app.event_loop(XX, YY);
    return 0;
}

