/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
