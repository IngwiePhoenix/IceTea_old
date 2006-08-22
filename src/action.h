#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>

class Action
{
public:
	enum eAction
	{
		actCheck,
		actClean
	};
public:
	Action();
	virtual ~Action();

private:

};

#endif
