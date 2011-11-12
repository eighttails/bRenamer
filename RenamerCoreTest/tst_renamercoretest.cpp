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

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtGui/QFileSystemModel>
#include "query.h"
#include "regexpmethod.h"
#include "dummymethod.h"
#include "renamecommand.h"

class RenamerCoreTest : public QObject
{
    Q_OBJECT
public:
    RenamerCoreTest();
private:
    void emptyDir(QString path);
private Q_SLOTS:
    void cleanup();
    void regExpQueryTest1();
    void regExpMrthodTest1();
    void renameCommandTest1();
private:
    QString testPath_;
};

RenamerCoreTest::RenamerCoreTest()
{
}

void deleteFolder(const QString folderPath)
{
    QDir dir(folderPath);
    if(!dir.exists()) return;

    /*再帰的にディレクトリを削除*/
    QFileInfoList fiList = dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo info, fiList){
        if(info.isDir()){
            deleteFolder(info.absoluteFilePath());
        } else {
            QFile file(info.absoluteFilePath());
            file.remove();
        }
    }
   dir.rmdir(dir.path());
}

void RenamerCoreTest::cleanup()
{
    deleteFolder(testPath_);
}

void createFile(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadWrite);
    file.close();
}

void RenamerCoreTest::regExpQueryTest1()
{
    testPath_ = QDir::tempPath() + "/regExpQueryTest1";
    deleteFolder(testPath_);
    QDir tmp = QDir::temp();
    tmp.mkpath(testPath_);
    tmp.mkpath(testPath_ + "/folderAAA");
    tmp.mkpath(testPath_ + "/folderBBB");
    tmp.mkpath(testPath_ + "/folderBBB/folderAAA2");
    createFile(testPath_ + "/folderAAA/AAA1.txt");
    createFile(testPath_ + "/AAA2.txt");
    createFile(testPath_ + "/aaa.txt");

    QList<RenameMethod*> methods;
    methods.append(new DummyMethod());
    Query q(this, methods);
    {
        q.setRecursive(true);
        QList<Rename> list = q.getRenameList(testPath_, ".*AAA.*", "");
        QVERIFY(list.count() == 5);
        int i = 0;
        QVERIFY(list[i++].from_ == "aaa.txt");
        QVERIFY(list[i++].from_ == "AAA2.txt");
        QVERIFY(list[i++].from_ == "AAA1.txt");
        QVERIFY(list[i++].from_ == "folderAAA");
        QVERIFY(list[i++].from_ == "folderAAA2");
    }
    {
        q.setRecursive(false);
        q.setSubject(Query::FILE);
        QList<Rename> list = q.getRenameList(testPath_, ".*AAA.*", "");
        QVERIFY(list.count() == 2);
        int i = 0;
        QVERIFY(list[i++].from_ == "aaa.txt");
        QVERIFY(list[i++].from_ == "AAA2.txt");
    }
    {
        q.setRecursive(false);
        q.setSubject(Query::FILE);
        q.setCaseSensitive(true);
        QList<Rename> list = q.getRenameList(testPath_, ".*aaa.*", "");
        QVERIFY(list.count() == 1);
        int i = 0;
        QVERIFY(list[i++].from_ == "aaa.txt");
    }
    {
        q.setRecursive(false);
        q.setSubject(Query::FOLDER);
        QList<Rename> list = q.getRenameList(testPath_, ".*AAA.*", "");
        QVERIFY(list.count() == 1);
        int i = 0;
        QVERIFY(list[i++].from_ == "folderAAA");
    }
    {
        q.setRecursive(true);
        q.setSubject(Query::FOLDER);
        QList<Rename> list = q.getRenameList(testPath_, ".*AAA.*", "");
        QVERIFY(list.count() == 2);
        int i = 0;
        QVERIFY(list[i++].from_ == "folderAAA");
        QVERIFY(list[i++].from_ == "folderAAA2");
    }
    {
        q.setRecursive(true);
        q.setPatternSyntax(QRegExp::Wildcard);
        q.setSubject(Query::FOLDER | Query::FILE);
        QList<Rename> list = q.getRenameList(testPath_, "*AAA2*", "");
        QVERIFY(list.count() == 2);
        int i = 0;
        QVERIFY(list[i++].from_ == "AAA2.txt");
        QVERIFY(list[i++].from_ == "folderAAA2");
    }
}

void RenamerCoreTest::regExpMrthodTest1()
{
    testPath_ = QDir::tempPath() + "/regExpMrthodTest1";
    deleteFolder(testPath_);
    QDir tmp = QDir::temp();
    tmp.mkpath(testPath_);
    tmp.mkpath(testPath_ + "/folderAAA");
    tmp.mkpath(testPath_ + "/folderBBB");
    tmp.mkpath(testPath_ + "/folderBBB/folderAAA2");
    createFile(testPath_ + "/folderAAA/AAA1.txt");
    createFile(testPath_ + "/AAA2.txt");

    QList<RenameMethod*> methods;
    methods.append(new RegExpMethod());
    Query q(this, methods);
    {
        q.setRecursive(true);
        QList<Rename> list = q.getRenameList(testPath_, "(.*)AAA(.*)", "\\1CCC\\2");
        QVERIFY(list.count() == 4);
        int i = 0;
        QVERIFY(list[i++].to_ == "CCC2.txt");
        QVERIFY(list[i++].to_ == "CCC1.txt");
        QVERIFY(list[i++].to_ == "folderCCC");
        QVERIFY(list[i++].to_ == "folderCCC2");
    }
}

void RenamerCoreTest::renameCommandTest1()
{
    testPath_ = QDir::tempPath() + "/renameCommandTest1";
    deleteFolder(testPath_);
    QDir tmp = QDir::temp();
    tmp.mkpath(testPath_);
    tmp.mkpath(testPath_ + "/folderAAA");
    tmp.mkpath(testPath_ + "/folderBBB");
    tmp.mkpath(testPath_ + "/folderBBB/folderAAA2");
    createFile(testPath_ + "/folderAAA/AAA1.txt");
    createFile(testPath_ + "/AAA2.txt");

    QList<RenameMethod*> methods;
    methods.append(new RegExpMethod());
    Query q(this, methods);
    {
        q.setRecursive(true);
        QList<Rename> list = q.getRenameList(testPath_, "(.*)AAA(.*)", "\\1CCC\\2");
        QVERIFY(list.count() == 4);
        int i = 0;
        QVERIFY(list[i++].to_ == "CCC2.txt");
        QVERIFY(list[i++].to_ == "CCC1.txt");
        QVERIFY(list[i++].to_ == "folderCCC");
        QVERIFY(list[i++].to_ == "folderCCC2");

        RenameCommand command(list);
        command.redo();
        //#PENDING VERIFY
        command.undo();
    }
}
QTEST_MAIN(RenamerCoreTest)

#include "tst_renamercoretest.moc"
