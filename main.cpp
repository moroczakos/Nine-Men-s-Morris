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
    staticText * staticText2;
    staticText * staticText3;
    staticText * staticText4;
    button * newGameButton;
    button * exitButton;
    button * startButton;
    dinamicText * dinamicTextFirstPlayerName;
    dinamicText * dinamicTextSecondPlayerName;
    scroll * scrollDescription;

    bool _game;

public:
    MyApplication(){
        staticText2 = new staticText(this,300,15,200,10,"   Nine Men's Morris");
        staticText3 = new staticText(this,195,70,200,10," First player's name:");
        staticText4 = new staticText(this,405,70,200,10," Second player's name:");
        dinamicTextFirstPlayerName = new dinamicText(this,195,110,190,30);
        dinamicTextSecondPlayerName = new dinamicText(this,405,110,190,30);
        startButton = new button(this,350,400,100,30,"   Start");
        scrollDescription = new scroll(this,20,180,760,200,"rule.txt");

        table1 = new table(this,40,70,400,400);
        staticText1 = new staticText(this,500,70,200,10,"Green starts");
        newGameButton = new button(this,500,100,90,30," New Game");
        exitButton = new button(this,600,100,90,30,"   Exit");

        _game = false;
    }




    virtual void action()
    {
        if (_game){
            staticText1->addElement(table1->value());
            if (newGameButton->changedValue()) {table1->newGame();}
            if (exitButton->changedValue()) {table1->newGame(); _game = !_game;}

            table1->visible();
            staticText1->visible();
            newGameButton->visible();
            exitButton->visible();

            staticText3->invisible();
            staticText4->invisible();
            startButton->invisible();
            dinamicTextFirstPlayerName->invisible();
            dinamicTextSecondPlayerName->invisible();
            scrollDescription->invisible();
        }
        else{
            table1->invisible();
            staticText1->invisible();
            newGameButton->invisible();
            exitButton->invisible();

            if (startButton->changedValue()) {_game = !_game;}

            staticText3->visible();
            staticText4->visible();
            startButton->visible();
            dinamicTextFirstPlayerName->visible();
            dinamicTextSecondPlayerName->visible();
            scrollDescription->visible();
            table1->addName(dinamicTextFirstPlayerName->value(),dinamicTextSecondPlayerName->value());
        }

    }
};

int main()
{
    gout.open(XX, YY);
    MyApplication app;
    app.event_loop(XX, YY);
    return 0;
}

