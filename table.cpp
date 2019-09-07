#include "table.h"
#include "graphics.hpp"
#include <math.h>
#include <iostream>

using namespace genv;

table::table(Application * parent,int x, int y, int sx, int sy)
    : Widget(parent,x,y,sx,sy)
{
    _focusedElementX = -1;
    _focusedElementY = -1;
    _first18Step = 0;
    _player = false;    //elso jatekos (zold) false, masodik jatekos (piros) true
    _text = _firstPlayer;
    _text += " (green) starts";
    _endGame = false;   //jatek vegen ne lehessen babut mozgatni
}

//kor rajzolas 1, ha piros, 10, ha piros es kijelolt, 2, ha zold, 20, ha zold es kijelolt kulonben nem rajzol kort
void circle(int x_pos, int y_pos, int colour)
{
    if (colour>0)
    {
        for (int i=-30;i<=30;i++)
        {
            for (int j=-30;j<=30;j++)
            {
                if ((pow(i,2)+pow(j,2))<pow(25,2)){
                    gout << move_to(x_pos-i, y_pos-j) << color(100,100,100) << dot;
                }

                int innerDiameter;

                if (colour == 10 || colour == 20) innerDiameter = 17;
                else innerDiameter = 20;

                if ((pow(i,2)+pow(j,2))<pow(innerDiameter,2)){
                    gout << move_to(x_pos-i, y_pos-j);
                    if (colour==1 || colour == 10){
                        gout << color(255,0,0);
                    }
                    else if (colour==2 || colour == 20){
                        gout << color(0,255,0);
                    }
                    gout << dot;
                }
            }
        }
    }
}

void table::draw()
{
    //belso negyzetek
    gout << move_to(_x-5, _y-5) << color(255,255,255) << box(_size_x+10, _size_y+10);
    gout << move_to(_x+5, _y+5) << color(0,0,0) << box(_size_x-10, _size_y-10);
    gout << move_to(_x-5+_size_x/6,_y-5+_size_y/6) << color(255,255,255) << box(_size_x*4/6+10,_size_y*4/6+10);
    gout << move_to(_x+5+_size_x/6, _y+5+_size_y/6) << color(0,0,0) << box(_size_x*4/6-10, _size_y*4/6-10);
    gout << move_to(_x-5+_size_x/6*2,_y-5+_size_y/6*2) << color(255,255,255) << box(_size_x*2/6+10,_size_y*2/6+10);
    gout << move_to(_x+5+_size_x/6*2, _y+5+_size_y/6*2) << color(0,0,0) << box(_size_x*2/6-10, _size_y*2/6-10);

    //belso vonalak
    gout << move_to(_x, _y+_size_y/2-5) << color(255,255,255) << box(_size_x*2/6,10);
    gout << move(_size_x*2/6,-10) << box(_size_x*2/6,10);
    gout << move_to(_x+_size_x/2-5, _y) << color(255,255,255) << box(10,_size_y*2/6);
    gout << move(-10,_size_y*2/6) << box(10,_size_y*2/6);

    for (int i=0;i<7;i++)
    {
        for (int j=0;j<7;j++)
        {
            circle(_x+i*(_size_x)/6,_y+j*(_size_y)/6,_matrix[j][i]);
        }
    }
}

bool neighbourhood(int newX, int newY, int focusedElementX, int focusedElementY)
{
    if (newX != focusedElementX && newY != focusedElementY)
    {
        return false;
    }

    if ((newX == 0 || newX == 6 || newY == 0 || newY == 6) && abs((newX + newY) - (focusedElementX + focusedElementY)) == 3)
    {
        return true;
    }

    if ((newX == 1 || newX == 5 || newY == 1 || newY == 5) && abs((newX + newY) - (focusedElementX + focusedElementY)) == 2)
    {
        return true;
    }

    if ((newX == 2 || newX == 3 || newX == 4 || newY == 2 || newY == 3 || newY == 4) && abs((newX + newY) - (focusedElementX + focusedElementY)) == 1)
    {
        return true;
    }

    return false;
}

//szin szerint a tablan levo babuk szamat megmondja
void figureNumber(std::vector<std::vector<int> > _matrix, int &sumRed, int &sumGreen)
{
    sumRed = 0, sumGreen = 0;
    for (int i=0;i<7;i++)
    {
        for (int j=0;j<7;j++)
        {
            if (_matrix[j][i] == 1 || _matrix[j][i] == 10){
                sumRed++;
            }
            else if(_matrix[j][i] == 2 || _matrix[j][i] == 20){
                sumGreen++;
            }
        }
    }
}

void figureMove(std::vector<std::vector<int> > &_matrix, int i, int j, int &focusedElementX, int &focusedElementY, bool &player)
{
    //fokusz beallitasa
    if (_matrix[j][i] % 10 == 0 && _matrix[j][i] > 0){
        _matrix[j][i]/=10;
    }
    else if (_matrix[j][i] % 10 != 0 && _matrix[j][i] > 0){
       if (focusedElementX >= 0 && _matrix[focusedElementX][focusedElementY] >= 10) _matrix[focusedElementX][focusedElementY]/=10;  //emiatt csak egy lehet kijelolve egyszerre
        _matrix[j][i]*=10;
        focusedElementX=j;
        focusedElementY=i;
    }

    //3 babu eseten lehessen ugralni
    int sumRed, sumGreen;
    bool jump = false;

    figureNumber(_matrix, sumRed, sumGreen);

    if (player && sumRed < 4){
        jump = true;
    }
    else if (!player && sumGreen < 4){
        jump = true;
    }
    else{
        jump = false;
    }

    //kijelolt elemet az uj helyre teszi at kattintasra, ha szomszedos (erteket cserel a ket pozicio)
    if (_matrix[j][i] == 0 && focusedElementX >= 0 && _matrix[focusedElementX][focusedElementY]/10 > 0){
        if(neighbourhood(j, i, focusedElementX, focusedElementY) || jump){
            _matrix[j][i] = _matrix[focusedElementX][focusedElementY];
            _matrix[focusedElementX][focusedElementY] = 0;
            _matrix[j][i]/=10;  //kijeloles megszuntetese
            focusedElementX = j;
            focusedElementY = i;
            player = !player;
        }
    }
}

void addFigure(std::vector<std::vector<int> > &_matrix, int i, int j, bool &player, int &first18Step)
{
    if (_matrix[j][i] == 0 && !player)
    _matrix[j][i] = 2;
    if (_matrix[j][i] == 0 && player)
    _matrix[j][i] = 1;
    player = !player;
    first18Step++;
}

int nineMen_s_Morris_Check_Help(std::vector<std::vector<int> > _matrix, int from, int to, int coordFix, int focusedElement, int focusedElementX, int focusedElementY)
{
    int sum = 0;
    for (int i=from;i<to;i++){
        if (coordFix == 1){
            if (_matrix[focusedElement][i] == _matrix[focusedElementX][focusedElementY]){
                sum += _matrix[focusedElement][i];
            }
        }
        else{
            if (_matrix[i][focusedElement] == _matrix[focusedElementX][focusedElementY]){
                sum += _matrix[i][focusedElement];
            }
        }

    }
    if (sum % 3 == 0){
        return 1;
    }

    return 0;
}

bool nineMen_s_Morris_Check(std::vector<std::vector<int> > _matrix, int focusedElementX, int focusedElementY)
{
    int boolCount = 0;
    if (focusedElementX >= 0 && focusedElementX != 3 && focusedElementY != 3){
        boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 7, 1, focusedElementX, focusedElementX, focusedElementY);
        boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 7, 2, focusedElementY, focusedElementX, focusedElementY);
    }

    if (focusedElementX == 3){
        if (focusedElementY < 3){
            boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 3, 1, focusedElementX, focusedElementX, focusedElementY);
        }
        else{
            boolCount += nineMen_s_Morris_Check_Help(_matrix, 4, 7, 1, focusedElementX, focusedElementX, focusedElementY);
        }
        boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 7, 2, focusedElementY, focusedElementX, focusedElementY);
    }

    if (focusedElementY == 3){
        if (focusedElementX < 4){
            boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 3, 2, focusedElementY, focusedElementX, focusedElementY);
        }
        else{
            boolCount += nineMen_s_Morris_Check_Help(_matrix, 4, 7, 2, focusedElementY, focusedElementX, focusedElementY);
        }
        boolCount += nineMen_s_Morris_Check_Help(_matrix, 0, 7, 1, focusedElementX, focusedElementX, focusedElementY);
    }

    if (boolCount > 0) return true;
    return false;
}

bool removeFigure(std::vector<std::vector<int> > &_matrix, int coordX, int coordY, bool player)
{
    if (!player && _matrix[coordX][coordY] == 2){
        _matrix[coordX][coordY] = 0;
        return true;
    }
    else if (player && _matrix[coordX][coordY] == 1){
        _matrix[coordX][coordY] = 0;
        return true;
    }

    return false;
}

void showTable(std::vector<std::vector<int> > _matrix, int focusedElementX, int focusedElementY)
{
    std::cout << std::endl;
    for (int i=0;i<7;i++)
    {
        for (int j=0;j<7;j++)
        {
            std::cout << _matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "Fokusz koordinatak: " << focusedElementX << " " << focusedElementY << "van-e malom " << nineMen_s_Morris_Check(_matrix, focusedElementX, focusedElementY) << std::endl;
}

int endGame(std::vector<std::vector<int> > _matrix)
{
    int sumRed = 0, sumGreen = 0;

    figureNumber(_matrix, sumRed, sumGreen);

    if (sumRed < 3){
        std::cout << "Zold nyert";
        return 2;
    }
    if (sumGreen < 3){
        std::cout << "Piros nyert";
        return 1;
    }

    return 0;
}

void printTurn(bool player, std::string &text, std::string firstPlayer, std::string secondPlayer)
{
    if (!player){
        text = firstPlayer;
        text += " (green) turn";
    }
    else{
        text = secondPlayer;
        text += " (red) turn";
    }
}

void printRemove(bool player, std::string &text)
{
    if (!player){
        text = "Remove a green figure!";
    }
    else{
        text = "Remove a red figure!";
    }
}

void table::handle(event ev)
{
    for (int i=0;i<7;i++)
    {
        for (int j=0;j<7;j++)
        {
            if (ev.pos_x >= _x+i*(_size_x)/6-20 && ev.pos_x <= _x+i*(_size_x)/6+20 && ev.pos_y >= _y+j*(_size_y)/6-20 && ev.pos_y <= _y+j*(_size_y)/6+20 && ev.button==btn_left &&  _matrix[j][i] >= 0){
                if (nineMen_s_Morris_Check(_matrix, _focusedElementX, _focusedElementY) && !_endGame){
                    //csak az ellenfel babujat lehessen levenni
                    if (removeFigure(_matrix, j, i, _player)){
                        _focusedElementX = -1;
                        _focusedElementY = -1;
                    }
                }
                else if (_first18Step > 17 && !_endGame){
                    if (_player && (_matrix[j][i] == 1 || _matrix[j][i] == 10 || _matrix[j][i] == 0)){
                        figureMove(_matrix, i, j, _focusedElementX, _focusedElementY, _player);
                    }
                    else if (!_player && (_matrix[j][i] == 2 || _matrix[j][i] == 20 || _matrix[j][i] == 0)){
                        figureMove(_matrix, i, j, _focusedElementX, _focusedElementY, _player);
                    }
                }
                else if (_first18Step < 18 && !_endGame){
                    addFigure(_matrix, i, j, _player, _first18Step);
                    _focusedElementX = j;
                    _focusedElementY = i;
                }

                printTurn(_player, _text, _firstPlayer, _secondPlayer);

                if (nineMen_s_Morris_Check(_matrix, _focusedElementX, _focusedElementY)){
                    printRemove(_player, _text);
                }

                if (_first18Step > 17){
                    switch (endGame(_matrix))
                    {
                        case 1 :    _text = _secondPlayer;
                                    _text += " (red) won";
                                    _endGame = true;
                                    break;
                        case 2 :    _text = _firstPlayer;
                                    _text += " (green) won";
                                    _endGame = true;
                                    break;
                    }
                }
            }
        }
    }

    //matrix kiiratasa k billentyu lenyomasara
    if (ev.keycode == 'k'){
         showTable(_matrix, _focusedElementX, _focusedElementY);
    }
}

std::string table::value()
{
    return _text;
}

bool table::changedValue()
{
    return false;
}

void table::getMatrix(std::vector<std::vector<int> > matrix)
{
    _matrix=matrix;
}

void table::newGame()
{
    _matrix = { {0,-1,-1,0,-1,-1,0},
                 {-1,0,-1,0,-1,0,-1},
                 {-1,-1,0,0,0,-1,-1},
                 {0,0,0,-1,0,0,0},
                 {-1,-1,0,0,0,-1,-1},
                 {-1,0,-1,0,-1,0,-1},
                 {0,-1,-1,0,-1,-1,0} };
    _focusedElementX = -1;
    _focusedElementY = -1;
    _first18Step = 0;
    _player = false;    //elso jatekos (zold) false, masodik jatekos (piroszü) true
    _text = _firstPlayer;
    _text += " (green) starts";
    _endGame = false;
}

void table::addName(std::string firstPlayer, std::string secondPlayer)
{
    _firstPlayer = firstPlayer;
    _secondPlayer = secondPlayer;
    _text = _firstPlayer;
    _text += " (green) starts";
}


