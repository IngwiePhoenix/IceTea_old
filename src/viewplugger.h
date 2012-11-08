#ifndef VIEW_PLUGGER_H
#define VIEW_PLUGGER_H

#include "view.h"
#include <bu/plugger.h>
#include <bu/singleton.h>

class ViewPlugger : public Bu::Plugger<View>, public Bu::Singleton<ViewPlugger>
{
    friend class Bu::Singleton<ViewPlugger>;
private:
    ViewPlugger();

public:
    virtual ~ViewPlugger();

private:
};

#endif
