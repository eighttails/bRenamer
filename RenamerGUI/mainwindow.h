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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QUndoStack>
#include <QQueue>
#include "rename.h"
class Query;
class QFileSystemModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	Query *query() const;

	struct History
	{
		History(QString q, QString r)
			: query(q)
			, rename(r){}
		QString query;
		QString rename;
	};

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

private slots:
	void updateLineEditPath(QModelIndex index);
	void updateSubjects();

	void on_pushButtonPreview_clicked();
	void on_pushButtonRename_clicked();
	void on_pushButtonUndo_clicked();

	void on_checkBoxShowHiddenFolder_toggled(bool enabled);
	void on_pushButtonOpenFolder_clicked();

	void on_comboBoxHistory_currentIndexChanged(int index);

private:
	QList<Rename> createRenameList();
	void updatePreview(const QList<Rename>& list);
	void addHistory(QString query, QString rename);
	void saveHistory();
	void restoreHistory();

	Ui::MainWindow *ui;
	Query* query_;
	QFileSystemModel* fsModel_;

	// 実行されたリネームの履歴(undo用)
	QUndoStack undoStack_;

	// 実行されたリネームの履歴（再実行用）
	QQueue<History> history_;


};

#endif // MAINWINDOW_H
