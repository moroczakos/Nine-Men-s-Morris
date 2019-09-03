#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include "graphics.hpp"
#include "widgets.hpp"
#include <vector>

class table : public Widget {
protected:
    std::vector<std::vector<int> > _matrix{ {0,-1,-1,0,-1,-1,0},
                                 {-1,0,-1,0,-1,0,-1},
                                 {-1,-1,0,0,0,-1,-1},
                                 {0,0,0,-1,0,0,0},
                                 {-1,-1,0,0,0,-1,-1},
                                 {-1,0,-1,0,-1,0,-1},
                                 {0,-1,-1,0,-1,-1,0} };
    int _focusedElementX, _focusedElementY, _first18Step;
    bool _player;
    std::string _text, _firstPlayer, _secondPlayer;
public:
    table(Application * parent,int x, int y, int sx, int sy, std::string firstPlyer, std::string secondPlayer);
    virtual void draw();
    virtual void handle(genv::event ev);
    virtual bool changedValue();
    virtual std::string value();
    virtual void getMatrix(std::vector<std::vector<int> > matrix);
    virtual void newGame();
    virtual void addName(std::string firstPlayer, std::string secondPlayer);
};


#endif // TABLE_H_INCLUDED
