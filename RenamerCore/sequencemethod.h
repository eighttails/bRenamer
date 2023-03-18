#ifndef SEQUENCEMETHOD_H
#define SEQUENCEMETHOD_H
#include "renamemethod.h"

// 連番を付加するメソッド。
// <s[桁数],[オリジン]>の書式で指定する。
// 指定した桁数に満たない数は0でパディングされる。
// <s2>は01,02,03...
// <s3,100>は100,101,102...
// と置換される。
// 桁数を省略した場合は1,オリジンを省略した場合は1が適用される。
class SequenceMethod : public RenameMethod
{
    Q_OBJECT
public:
    explicit SequenceMethod(QObject *parent = 0);
	virtual QString rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString) override;
	virtual QList<RenameAssistant *> getRenameAssistants() override;
	virtual void reset() override;

signals:

public slots:
private:
    int64_t origin_;        // 連番の起点
    int64_t digits_;        // 連番の桁数
    int64_t currentNum_;    // 現在の番号
};

#endif // SEQUENCEMETHOD_H
