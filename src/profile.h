#ifndef PROFILE_H
#define PROFILE_H

#include <bu/string.h>

class Profile
{
public:
    Profile( const class AstBranch *pRoot );
    Profile( const Profile &rSrc );
    virtual ~Profile();

    const Bu::String &getName() const;
    const class AstBranch *getRoot() const;
    const class Condition *getCond() const;
    bool shouldExec( class Runner &r, class Target &rTarget ) const;

    static Profile *genDefaultClean();

private:
    void setCondition();

private:
    Bu::String sName;
    const class AstBranch *pRoot;
    class Condition *pCond;
    class Ast *pAst;
};

#endif
