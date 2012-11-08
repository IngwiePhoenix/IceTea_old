#ifndef RULE_H
#define RULE_H

#include "types.h"
#include <bu/formatter.h>

class Rule
{
    friend Bu::Formatter &operator<<( Bu::Formatter &f, const Rule &t );
public:
    Rule( const Bu::String &sName );
    virtual ~Rule();

    const Bu::String &getName() const;

    void setInput( const AstBranch *pNewInput );
    const AstBranch *getInput() const;

    bool hasOutputs() const;

    void addOutput( const AstBranch *pNewOutput );
    void addProfile( const AstBranch *pProfile );

    void prepTarget( class Target *pTarget );
    class Target *createTarget( class Runner &r, const Bu::String &sInput,
            class Target *pParent );
    bool ruleMatches( class Runner &r, const Bu::String &sInput );

    void addTag( const Bu::String &sTag );
    const StrList &getTagList() const;

    void setDisplay( const Bu::String &sStr );
    const Bu::String &getDisplay() const;

    void addRequires( const AstBranch *pBr );

private:
    Bu::String sName;
    Bu::String sDisplay;
    const AstBranch *pInput;
    AstBranchList lOutput;
    ProfileHash hProfiles;
    StrList lsTags;
    AstBranchList lRequires;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Rule &t );

namespace Bu
{
    template<> Bu::Formatter &operator<< <Rule>( Bu::Formatter &f, const Rule *t );
};

#endif
