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

#include "query.h"
#include <QDir>
#include <QFileInfo>

Query::Query(QObject *parent, QList<RenameMethod*> methods) :
    QObject(parent),
    caseSensitive_(false),
    recursive_(false),
    subject_(FOLDER | FILE),
    methods_(methods),
    patternSyntax_(QRegExp::RegExp2)
{
	createRenameAssistantsList(methods);
}

Query::~Query(){
    foreach(RenameMethod* method, methods_){
        delete method;
    }
}

QList<Rename> Query::getRenameList(QString rootPath, QString query, QString renameString)
{
    // メソッドの状態をリセット
    foreach(RenameMethod* method, methods_){
        method->reset();
    }
    QList<Rename> list;
    getRenameListInternal(list, rootPath, query, renameString);
    return list;
}

void Query::getRenameListInternal(QList<Rename>& list, QString parentPath, QString query, QString renameString)
{
    QRegExp exp(query, caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive, patternSyntax_);

    QDir dir(parentPath);
    if(!dir.exists()) return;

    QFileInfoList fiList = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo info, fiList){
        QString fileName = info.fileName();
        bool isDir = info.isDir();
        bool isSubject = ((subject_ & FOLDER) && isDir) ||
                ((subject_ & FILE) && !isDir);

        if(recursive_ && isDir){
            getRenameListInternal(list, parentPath + "/" + fileName, query, renameString);
        }

        if(isSubject && exp.exactMatch(fileName)){
            Rename rename;
            rename.from_ = fileName;
            rename.to_ = fileName;
            rename.parent_ = parentPath;

            foreach(RenameMethod* method, methods_){
                // 複数のリネーム方式を同時に使用するため、各RenameMethodはtoに記載されたファイル名を改変する
                rename.to_ = method->rename(parentPath, rename.to_, query, caseSensitive_, renameString);
            }
            list.append(rename);
        }
	}
}

void Query::createRenameAssistantsList(QList<RenameMethod *> methods)
{
	foreach(RenameMethod* method, methods){
		auto assistants = method->getRenameAssistants();
		renameAssistants_.append(assistants);
	}
	foreach(RenameAssistant* assistant, renameAssistants_){
		assistant->setParent(this);
	}
}

