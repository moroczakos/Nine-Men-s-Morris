#include "choose.h"
#include "graphics.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace genv;

choose::choose(Application * parent, int x, int y, int sx, int minimum, std::string fileName)
    : Widget(parent,x,y,sx,_sy)
{
    _min=minimum;                           //megjelenitheto opciok szama, deklaracional megadott
    _sy=gout.cascent()+8;                   //egy "szovegdoboz" magassaga
    _fileName=fileName;                     //ahonnet beolvassa az opciokat
    text();                                 //feltolti a vektort az opciokkal
    _chosenElement=0;                       //a kivalsztott elem, ami mindig latszik, alabol default az erteke
    _pressedArrow=false;                    //nyilra kattintva ha elso akkor true, tehát a legordules megjelenik, masodik kattra hamis, eltunik a menu
    _pressedScroll=false;                   //a gorgo felett kattintottam-e az egerrel (addig tart mig el nem engedem)
    _size_y=_sy;                            //az egesz widget magassaga (esemeny hatarterulete), változik a legordulestol fuggoen
    setElementParam();
    _row=0;                                 //hanyadik sortol jelenitse meg az elemeket a legordulo menuben
    _checkFormer=_chosenElement;            //ellenorzeshez megnezi hogy az elozo azonos-e a jelenlegivel
    _focusedElement=0;                      //fokuszban levo elem
}

void choose::setElementParam()
{
    if (_textout.size()>0){
        _elementLimit=_min;
        if (_textout.size()<size_t(_min)){              //_elementLimit valtozot allitja be: ha a beolvasott opciok merete kisebb mint a megadott akkor a beolvasott erteket veszi fel kulonben a megadottat (_min)
            _elementLimit=_textout.size();
        }
        _visibleElementNumber=_textout.size()-_elementLimit;    //lathato elemek szama, kod egyszerusitese miatt kell, tul hosszu lenne ha mindig kiszamolnam
        _chosenElement%=_textout.size();
    }
    else{
       _elementLimit=0;
       _visibleElementNumber=0;
       _chosenElement=0;
    }

}

//filebol beolvassa az opciokat, plussz hozzaad egy defaultot
void choose::text()
{
    _textout.push_back("default");
    std::ifstream f(_fileName);
    std::string temp;
    while(f.good())
    {
        std::getline(f,temp);
        _textout.push_back(temp);
    }

    f.close();
}

std::string choose::value()
{
    if (_textout.size()>0){
        return _textout[_chosenElement];
    }
    return "";
}

bool choose::changedValue()
{
    if(_chosenElement!=_checkFormer)
    {
        _checkFormer=_chosenElement;
        return true;
    }
    return false;
}

//megmodja, hogy adott hataron belul van-e az eger
bool choose::over(event ev, int x0, int x, int y0, int y)
{
    if(ev.pos_x<=x && ev.pos_x>=x0 && ev.pos_y>=y0 && ev.pos_y<=y){
        return true;
    }
    return false;
}

//az adott elemet jeleniti meg a legordulo menuben, a  hatterszin megadhato
void choose::openText(int changeColor)
{
    gout << move_to(_x,_y+_sy*(_elementNumber-_row + 1)) << color(255,255,255) << box(_size_x+2-20, _sy);
    gout << move_to(_x+1,_y+_sy*(_elementNumber-_row + 1)+1) << color(0+changeColor,0+changeColor,0+changeColor) << box(_size_x-20, _sy-2);
    gout << move_to(_x+3,_y+_sy*(_elementNumber-_row + 1)+gout.cascent()+4) << color(255,255,255);
    if (_textout.size()>0 && gout.twidth(_textout[_elementNumber])>_size_x-30){
        gout << genv::text(_textout[_elementNumber].substr(0,(_size_x-30)/gout.twidth("a")));
    }
    else if (_textout.size()>0){
        gout << genv::text(_textout[_elementNumber]);
    }
}

//gorgot rajzolja ki a megfelelo helyen
void choose::scrollDraw()
{
    gout << move_to(_x+_size_x,_y+_sy) << color(255,255,255) << box(-20,_size_y-_sy);
    gout << move_to(_x+_size_x-1,_y+_sy+1) << color(0,0,0) << box(-18,_size_y-_sy-2);
    gout << move_to(_x+_size_x,_y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*_row) << color(255,255,255) << box(-20,(_size_y-_sy)/(_visibleElementNumber+1));
}

void choose::draw()
{
    //ha nincs focusban a widget akkor ne lehessen latni a legordulo opciokat
    if (!_focused) _pressedArrow=false;

    //a kivalasztott elemet jeleniti meg, ami mindig lathato
    gout << move_to(_x, _y) << color(255,255-_focused*200,255-_focused*200) << box(_size_x, _sy);
    gout << move_to(_x+2, _y+2) << color(0,0,0) << box(_size_x-4, _sy-4);
    gout << move_to(_x+_size_x, _y) << color(255,255-_focused*200,255-_focused*200) << box(-20, _sy);
    gout << move_to(_x+_size_x-2, _y+2) << color(0,0,0) << box(-(20-4), _sy-4);
    gout << move_to(_x+4, _y+gout.cascent()+4) << color(255,255,255);
    //ha kinullazott a vector akkor nem ir ki semmit
    /*if(_textout.size()==0){
        gout << genv::text("");
    }*/
    /*else*/ if (gout.twidth(_textout[_chosenElement])>_size_x-30){
        gout << genv::text(_textout[_chosenElement].substr(0,(_size_x-30)/gout.twidth("a")));
    }
    else{
        gout << genv::text(_textout[_chosenElement]);
    }

    //ha eloszor a nyilra katt, akkor megjelenik a legordulo menu gorgo savja, masodszorra eltunik
    if (_pressedArrow){
        _size_y=_sy*(_elementLimit + 1);
        gout << move_to(_x+_size_x-4,_y+_sy-4) << color(255,255,255) << line(-(10-3),-(_sy-8)) << line(-(10-3),+(_sy-8));
        scrollDraw();
    }
    else if(!_pressedArrow || !_focused){
        _size_y=_sy;
        gout << move_to(_x+_size_x-4,_y+4) << color(255,255,255) << line(-(10-3),(_sy-8)) << line(-(10-3),-(_sy-8));
    }
}

//a lenyilt esetben az opciokat jeleniti meg, amelyik folott az eger van annak hatterszine mas
void choose::optionListDraw(event ev, int from)
{
    for (int i=from;i<_elementLimit+from;i++){
        _elementNumber=i;
        if (_pressedArrow && over(ev,_x+1, _x+1+_size_x-20, _y+_sy*(i-from + 1)+1, _y+_sy*(i-from + 1)+1+_sy-2)){
            _focusedElement=i;
            openText(100);
            if (ev.button==btn_left){
                _chosenElement=i;
                _pressedArrow=!_pressedArrow;
            }
        }
        else if (_pressedArrow){
            openText(0);
        }
    }
}

void choose::handle(event ev)
{
    //nyil feletti kattintasra valtozik (ha nem nulla a vectorom), hogy eloszor vagy masodszor kattintottunk (valt a lenyilt és nem lenyilt opcio kozott)
    if (_textout.size()!=0 && over(ev,_x+_size_x-20,_x+_size_x,_y,_y+_sy) && ev.button==btn_left){
        _pressedArrow=!_pressedArrow;
    }

    //a lenyilt esetben az opciokat jeleniti meg, amelyik folott az eger van annak hatterszine mas
    optionListDraw(ev,_row);

    //amelyik opcio felett van az eger az fokuszba kerul es marad (hatterszin valtozik), ha nincs felette akkor az alapertelmezettet helyezi fokuszba
    if (!over(ev,_x+1, _x+1+_size_x-20, _y+_sy, _y+_size_y) && _pressedArrow){
        _elementNumber=_focusedElement;
        openText(100);
    }


    //gomb, illetve eger esemenyek
    if (_row<_visibleElementNumber && ev.button==btn_wheeldown){
        _row++;
        _focusedElement++;
    }
    else if (_row>0 && ev.button==btn_wheelup){
        _row--;
        _focusedElement--;
    }

    switch (ev.keycode){
        case key_right : _pressedArrow=true;
        break;
        case key_left : _pressedArrow=false;
        break;
        case key_pgup : _focusedElement=0; _row=0;
        break;
        case key_pgdn : _focusedElement=_textout.size()-1; _row=_visibleElementNumber;
        break;
        case key_down : if (_focusedElement<_elementLimit+_row-1) _focusedElement++;                //fokuszalt elem ne menjen ki
                        else if (size_t(_focusedElement)<_textout.size()-1) {_row++; _focusedElement++;}    //fokuszalt elem ne menjen ki
        break;
        case key_up : if (_focusedElement>0) _focusedElement--;                                     //fokuszalt elem ne menjen ki
                      if (_focusedElement<_row && _focusedElement>=0) _row--;                       //fokuszalt elem ne menjen ki
        break;
        case key_enter : _chosenElement=_focusedElement;
        break;
    }

    //ha az eger a gorgo felett kattintva van akkor "megfogja" a gorgot ameddig nem engedi el a lenyomast
    if (over(ev, _x+_size_x-20, _x+_size_x, _y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*_row, _y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*_row+(_size_y-_sy)/(_visibleElementNumber+1)) && ev.button==btn_left){
        _pressedScroll=true;
    }
    else if(ev.button==-btn_left){
        _pressedScroll=false;
    }

    //a lenyomott bal eger helyzete szerint mozog a gorgo, ha az eger a gorgo felett volt akitvalva vagy a gorgore kattintott reszre ugrik a csuszka
    if (_pressedScroll || (ev.button==btn_left && ev.pos_x<=_x+_size_x && ev.pos_x>=_x+_size_x-20)){
        if (ev.pos_y<=_y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)) _row=0;
        else if (ev.pos_y>=_y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*(_visibleElementNumber)) _row=_visibleElementNumber;
        for (int i=1;i<_visibleElementNumber;i++)
        {
            if (ev.pos_y>=_y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*i && ev.pos_y<=_y+_sy+(_size_y-_sy)/(_visibleElementNumber+1)*(i+1)){
                _row=i;
            }
            _focusedElement=_row;
        }
    }
}

//elemet hozzaad
void choose::addElement(std::string add)
{
    if (add!=""){
        _textout.push_back(add);
        setElementParam();
        if (_textout[0]==""){
            _textout.erase(_textout.begin());
        }
    }
}

void choose::addVector(std::vector<std::string> inVector)
{
    _textout=inVector;
}

//elemet eltavolit, ha nem nulla a vector
void choose::removeElement(std::string reMove)
{
    if (_textout.size()>0){
        _textout.erase(find(_textout.begin(),_textout.end(),reMove));
        setElementParam();
    }
}

//kitorli a vektort es visszaadja az aktualisan kijelolt elem indexet
int choose::clearElement()
{
    int index=_chosenElement%_textout.size();
    _textout.clear();
    _textout.push_back("");
    setElementParam();
    return index;
}

//meghatarozza a _chosenElement erteket
void choose::getIndex(int index)
{
    _chosenElement=index%_textout.size();
}
