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
#include <QStringList>
#include "sequencemethod.h"

SequenceMethod::SequenceMethod(QObject *parent) :
    RenameMethod(parent),
    origin_(INT_MIN),
    digits_(INT_MIN),
    currentNum_(INT_MIN)
{
}

void SequenceMethod::reset()
{
    origin_ = INT_MIN;
    digits_ = INT_MIN;
    currentNum_ = INT_MIN;
}

QString SequenceMethod::rename(QString path, QString fileName, QString query, bool caseSensitive, QString renameString)
{
	Q_UNUSED(query)
	Q_UNUSED(caseSensitive)
	Q_UNUSED(renameString)

    // 置換リテラルと桁数を検出
    const QString format = "<s(\\d*),{0,1}(\\d*)>";
    QRegExp regExp(format);
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    int pos = 0;
    pos = regExp.indexIn(fileName, pos);
    if(pos == -1) return fileName;

    // 桁数を決定
    if(digits_ == INT_MIN){
        if(regExp.cap(1) != ""){
            digits_ = regExp.cap(1).toInt();
        } else {
            digits_ = 1;
        }
    }
    // オリジンを決定
    if(origin_ == INT_MIN){
        if(regExp.cap(2) != ""){
            origin_ = regExp.cap(2).toInt();
        } else {
            origin_ = 1;
        }
        // 初期値を設定
        currentNum_ = origin_;
    }

    // 前回処理したパス。連番はフォルダごとに振る
    static QString prevPath;
    if(prevPath != path){
        currentNum_ = origin_;
        prevPath = path;
    }

    QString renamed = fileName;

    // 先頭を0で埋める
    QString seqStr = QString::number(currentNum_);
    while(seqStr.length() < digits_)
        seqStr.prepend("0");

    renamed.replace(QRegExp(format), seqStr);
    currentNum_++;

    return renamed;

}

