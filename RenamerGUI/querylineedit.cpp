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

