/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "function.h"

Function::Function() :
    pContext( NULL )
{
}

Function::~Function()
{
}

void Function::setContext( class Context *p )
{
    pContext = p;
}

