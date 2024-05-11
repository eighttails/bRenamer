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

#include <QFileSystemModel>
#include <QUndoStack>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "query.h"
#include "queryassistant.h"
#include "regexpmethod.h"
#include "sequencemethod.h"
#include "parentfoldermethod.h"
#include "renamecommand.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	query_(NULL),
	fsModel_(NULL)
{
	ui->setupUi(this);

	// クエリ生成
	QList<RenameMethod*> methods;
	methods.append(new RegExpMethod());
	methods.append(new SequenceMethod());
	methods.append(new ParentFolderMethod());
	query_ = new Query(this, methods);
	query_->queryAssistants().append(new QueryAssistant("任意のファイル、フォルダ", "^(.*)$", query_));
	query_->queryAssistants().append(new QueryAssistant("拡張子が.txtのファイル", "(.*)\\.txt", query_));

	// 検索対象パスを設定
	QString homePath = QDir::homePath();
	ui->lineEditPath->setText(homePath);

	// ツリービュー作成
	fsModel_ = new QFileSystemModel(this);
	fsModel_->setRootPath(homePath);
	fsModel_->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	ui->treeViewFileSystem->setModel(fsModel_);
	ui->treeViewFileSystem->setCurrentIndex(fsModel_->index(homePath));
	ui->treeViewFileSystem->expand(ui->treeViewFileSystem->currentIndex());
	ui->treeViewFileSystem->setColumnWidth(0, 200);
	connect(ui->treeViewFileSystem, SIGNAL(clicked(QModelIndex)), this, SLOT(updateLineEditPath(QModelIndex)));

	// リネーム対象
	connect(ui->checkBoxFileSubject, SIGNAL(toggled(bool)), this, SLOT(updateSubjects()));
	connect(ui->checkBoxFolderSubject, SIGNAL(toggled(bool)), this, SLOT(updateSubjects()));

	// 大文字と小文字を区別する
	connect(ui->checkBoxCaseSensitive, SIGNAL(toggled(bool)), query_, SLOT(setCaseSensitive(bool)));

	// サブフォルダを含む
	connect(ui->checkBoxRecursive, SIGNAL(toggled(bool)), query_, SLOT(setRecursive(bool)));

	// プレビュー領域
	ui->tableWidgetPreview->resizeColumnToContents(0);
	ui->tableWidgetPreview->setColumnWidth(1, 200);
	ui->tableWidgetPreview->setColumnWidth(2, 200);
	ui->tableWidgetPreview->setColumnWidth(3, 200);

	// Undo
	undoStack_.setUndoLimit(1);
	connect(&undoStack_, SIGNAL(canUndoChanged(bool)), ui->pushButtonUndo, SLOT(setEnabled(bool)));

	// 履歴
	restoreHistory();

    // ウィンドウサイズの復元
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
	saveHistory();
	delete ui;
}

void MainWindow::updateLineEditPath(QModelIndex index)
{
	QString path = fsModel_->filePath(index);
	ui->lineEditPath->setText(path);
	ui->treeViewFileSystem->resizeColumnToContents(0);
}

void MainWindow::updateSubjects()
{
	Query::RenameSubject subject = Query::NONE;
	if(ui->checkBoxFileSubject->checkState() == Qt::Checked){
		subject |= Query::FILE;
	}
	if(ui->checkBoxFolderSubject->checkState() == Qt::Checked){
		subject |= Query::FOLDER;
	}
	query_->setSubject(subject);
}

QList<Rename> MainWindow::createRenameList()
{
	return query_->getRenameList(
				ui->lineEditPath->text(),
				ui->LineEditQueryString->text(),
				ui->LineEditRenameString->text());
}

void MainWindow::updatePreview(const QList<Rename>& list)
{
	ui->tableWidgetPreview->clearContents();
	ui->tableWidgetPreview->setRowCount(list.size());
	for(int i = 0; i < list.size(); ++i){
		const Rename& r = list[i];
		QFileIconProvider v;
		int j = 0;
		ui->tableWidgetPreview->setItem(i, j++, new QTableWidgetItem(v.icon(QFileInfo(QDir(r.parent_), r.from_)), ""));
		ui->tableWidgetPreview->setItem(i, j++, new QTableWidgetItem(r.from_));
		ui->tableWidgetPreview->setItem(i, j++, new QTableWidgetItem(r.to_));
		ui->tableWidgetPreview->setItem(i, j++, new QTableWidgetItem(r.parent_));
	}
}

void MainWindow::addHistory(QString query, QString rename)
{
	foreach(auto h, history_){
		// すでに履歴に含まれている場合は何もしない
		if (h.query == query && h.rename == rename)
			return;
	}
	history_.push_back(History(query, rename));
	// 履歴は5個まで記憶する
	while (history_.size() > 5){
		history_.pop_front();
	}
	ui->comboBoxHistory->clear();
	ui->comboBoxHistory->addItem("");
	foreach (auto h, history_) {
		ui->comboBoxHistory->addItem(h.query + " -> " + h.rename);
	}
}

void MainWindow::saveHistory()
{
	QSettings settings;
	settings.beginWriteArray("history");
	for (int i = 0; i < history_.size(); ++i) {
		auto h = history_.at(i);
		settings.setArrayIndex(i);
		settings.setValue("query", h.query);
		settings.setValue("rename", h.rename);
	}
	settings.endArray();
}

void MainWindow::restoreHistory()
{
	ui->comboBoxHistory->clear();
	ui->comboBoxHistory->addItem("");

	QSettings settings;
	int size = settings.beginReadArray("history");
	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		addHistory(settings.value("query").toString(),
				   settings.value("rename").toString());
	}
	settings.endArray();
}



Query *MainWindow::query() const
{
    return query_;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // ウィンドウサイズの保存
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::on_pushButtonPreview_clicked()
{
	QList<Rename> list = createRenameList();
	updatePreview(list);
}
void MainWindow::on_pushButtonRename_clicked()
{
	QList<Rename> list = createRenameList();
	updatePreview(list);
	if(list.size() > 0){
		RenameCommand* command = new RenameCommand(list);
		undoStack_.push(command);
		addHistory(ui->LineEditQueryString->text(),
				   ui->LineEditRenameString->text());
	}
}

void MainWindow::on_pushButtonUndo_clicked()
{
	if(undoStack_.canUndo()){
		undoStack_.undo();
	}
}

void MainWindow::on_checkBoxShowHiddenFolder_toggled(bool enabled)
{
	fsModel_->setFilter(enabled ?
							QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden :
							QDir::AllDirs | QDir::NoDotAndDotDot);
}

void MainWindow::on_pushButtonOpenFolder_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, "フォルダを参照", ui->lineEditPath->text());
	if (!dir.isEmpty()){
		ui->lineEditPath->setText(dir);
		ui->treeViewFileSystem->setCurrentIndex(fsModel_->index(dir));
		ui->treeViewFileSystem->expand(ui->treeViewFileSystem->currentIndex());
	}
}

void MainWindow::on_comboBoxHistory_currentIndexChanged(int index)
{
	if (index <= 0) return;
	// コンボボックスの0番目は空白であることを前提とする
	History history = history_[index - 1];
	ui->LineEditQueryString->setText(history.query);
	ui->LineEditRenameString->setText(history.rename);
	QList<Rename> list = createRenameList();
	updatePreview(list);
	ui->comboBoxHistory->setCurrentIndex(0);
}
