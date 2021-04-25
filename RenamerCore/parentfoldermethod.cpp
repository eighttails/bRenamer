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
#include <QRegExp>
#include <QDir>
#include "parentfoldermethod.h"
#include "renameassistant.h"
ParentFolderMethod::ParentFolderMethod(QObject *parent) :
    RenameMethod(parent)
{
}

QString ParentFolderMethod::rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString)
{
    const QString format = "<p(\\d*)>";
    QRegExp regExp(format);
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    int pos = 0;
    pos = regExp.indexIn(fileName, pos);
    if(pos == -1) return fileName;

    int hierarchy = 1;
    // 遡る階層数を決定
    if(regExp.cap(1) != ""){
        hierarchy = regExp.cap(1).toInt();
    }

    QString parentPath = path;
    for (int i = 0; i < hierarchy - 1; i++){
        parentPath += QDir::separator() + QString("..");
    }

    QString parentFolder;
    QDir dir(QDir(parentPath).canonicalPath());
    if(dir.exists()){
        parentFolder = dir.dirName();
    } else {
        //親フォルダが存在しない場合は空文字を返す
        parentFolder = "";
    }

    QString renamed = fileName;
    renamed.replace(QRegExp(regExp.cap(0)), parentFolder);

    // 同一置換文字列内に複数の<f>リテラルがあった場合に対処するため、再帰処理にする。
    return rename(path, renamed, query, caseSensitive, renameString);
}


QList<RenameAssistant *> ParentFolderMethod::getRenameAssistants()
{
	QList<RenameAssistant *> list;
	list.append(new RenameAssistant("親フォルダ名:1階層上", "<p1>"));
	return list;
}
