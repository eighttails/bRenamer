#ifndef DUMMYMETHOD_H
#define DUMMYMETHOD_H
#include "renamemethod.h"

class DummyMethod : public RenameMethod
{
    Q_OBJECT
public:
    explicit DummyMethod(QObject *parent = 0);
    virtual QString rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString);
};

#endif // DUMMYMETHOD_H
