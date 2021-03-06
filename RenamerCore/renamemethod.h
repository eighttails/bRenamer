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

#ifndef RENAMEMETHOD_H
#define RENAMEMETHOD_H

#include <QObject>

class RenameAssistant;

class RenameMethod : public QObject
{
    Q_OBJECT
public:
    explicit RenameMethod(QObject *parent = 0);

	// 元ファイル名とクエリ文字列から、リネーム後のファイル名を返す。
	// クエリ文字列は正規表現を用いた置換の場合に参照する。
    virtual QString rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString) = 0;

	// 状態を持つ置換方式については、一括置換の実行ごとに状態をリセットする。
    virtual void reset() {};

	// 入力補助情報を返す
	// オブジェクトの解放責任は呼び出し側が負う。
	virtual QList<RenameAssistant*> getRenameAssistants() = 0;

};

#endif // RENAMEMETHOD_H
