#ifndef PARENTFOLDERMETHOD_H
#define PARENTFOLDERMETHOD_H

#include "renamemethod.h"

// 親フォルダ名を付加するメソッド。
// <p[階層数]>の書式で指定する。
// 階層数を省略した場合は1が適用される。

class ParentFolderMethod : public RenameMethod
{
    Q_OBJECT
public:
    explicit ParentFolderMethod(QObject *parent = 0);
	virtual QString rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString) override;
	virtual QList<RenameAssistant *> getRenameAssistants() override;
};

#endif // PARENTFOLDERMETHOD_H
