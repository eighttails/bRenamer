/*--------------------------------------------------------------------
Copyright (c) 2011, Tadahito Yao
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------*/

#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QList>
#include <QRegularExpression>
#include <QMetaEnum>

#include "queryassistant.h"
#include "renameassistant.h"
#include "rename.h"
#include "renamemethod.h"

class Query : public QObject
{
    Q_OBJECT
    Q_FLAGS(RenameSubject)
    Q_PROPERTY(bool CaseSensitive READ isCaseSensitive WRITE setCaseSensitive)
    Q_PROPERTY(bool Recursive READ isRecursive WRITE setRecursive)
    Q_PROPERTY(RenameSubject RenameSubject READ subject WRITE setSubject)

public:
	// リネーム対象。フォルダー、ファイル、またはその両方
    enum RenameSubjectFlag {
		NONE = 0x0,
        FOLDER = 0x1,
        FILE = 0x2,
    };
    Q_DECLARE_FLAGS(RenameSubject, RenameSubjectFlag)

    explicit Query(QObject *parent, QList<RenameMethod*> methods);
    virtual ~Query();

	bool isCaseSensitive() const { return caseSensitive_; }
	bool isRecursive() const { return recursive_; }
	const RenameSubject subject() const {return subject_; }
	QList<QueryAssistant*>& queryAssistants() { return queryAssistants_; }
	QList<RenameAssistant*>& renameAssistants() { return renameAssistants_; }

signals:

public slots:
	// rootPath以下のフォルダからクエリにマッチするファイルを抽出し、
	// リネーム対象と、リネーム後のファイル名のリストを返す
    QList<Rename> getRenameList(QString rootPath, QString query, QString renameString);

    void setSubject(RenameSubject subject) { subject_ = subject; }
    void setCaseSensitive(bool caseSensitive) { caseSensitive_ = caseSensitive; }
    void setRecursive(bool recursive) { recursive_ = recursive; }
protected:
    void getRenameListInternal(QList<Rename>& list, QString parentPath, QString query, QString renameString);
	void createRenameAssistantsList(QList<RenameMethod*> methods);

	bool caseSensitive_; // 大文字小文字を区別する
	bool recursive_; // フォルダを再帰的に処理する
	RenameSubject subject_; // リネーム対象

	// クエリ書式に関するヘルプ文字列のリスト
    QList<QueryAssistant*> queryAssistants_;

	// 変換書式に関するヘルプ文字列のリスト
	QList<RenameAssistant*> renameAssistants_;

	// リネーム処理クラスのリスト
    QList<RenameMethod*> methods_;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Query::RenameSubject)

#endif // QUERY_H
