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

#include <QMenu>
#include <QContextMenuEvent>
#include "querylineedit.h"
#include "mainwindow.h"
#include "query.h"

QueryLineEdit::QueryLineEdit(QWidget *parent)
	: QLineEdit(parent)
{

}

void QueryLineEdit::insertAssist()
{
	// 正規表現の入力補助
	QAction* action = qobject_cast<QAction*>(sender());
	QString expression = action->data().toString();
	this->insert(expression);
}

void QueryLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
	QSharedPointer<QMenu> menu(createStandardContextMenu());
	menu->addSeparator();
	QMenu* assistMenu = menu->addMenu("入力補助");
	MainWindow* window = qobject_cast<MainWindow*>(this->window());
	foreach(auto assistant, window->query()->queryAssistants()){
		QAction* action = assistMenu->addAction(assistant->description());
		action->setData(assistant->expression());
		QObject::connect(action, SIGNAL(triggered()), this, SLOT(insertAssist()));
	}
	menu->exec(event->globalPos());

}

