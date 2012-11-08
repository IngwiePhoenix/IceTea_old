#ifndef FILE_MGR_H
#define FILE_MGR_H

#include <bu/singleton.h>
#include <bu/hash.h>
#include <bu/file.h>

class FileMgr : public Bu::Singleton<FileMgr>
{
friend class Bu::Singleton<FileMgr>;
private:
    FileMgr();
    virtual ~FileMgr();

public:
    int open( const Bu::String &sPath, int iMode );
    Bu::File &get( int iId );
    void close( int iId );

private:
    typedef Bu::Hash<int, Bu::File *> FileHash;
    FileHash hFile;
    int iNextId;
};

#endif
