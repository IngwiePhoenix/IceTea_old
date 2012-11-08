#ifndef CONDITION_H
#define CONDITION_H

class Condition
{
public:
    Condition();
    virtual ~Condition();

    virtual bool shouldExec( class Runner &r, class Target &rTarget )=0;
    virtual Condition *clone()=0;

private:
};

#endif
