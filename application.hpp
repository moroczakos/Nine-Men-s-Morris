#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include "widgets.hpp"
#include <vector>
#include <string>
#include <fstream>

class Application
{
protected:
    std::vector<Widget*> widgets;
    std::ofstream _log;
public:
    Application();
    virtual ~Application();
    void event_loop(int XX, int YY);
    void registerWidget(Widget *w){
        widgets.push_back(w);
    }
    virtual void action(){};
};

#endif // APPLICATION_HPP_INCLUDED
