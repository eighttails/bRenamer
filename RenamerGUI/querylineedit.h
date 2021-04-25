#ifndef QUERYLINEEDIT_H
#define QUERYLINEEDIT_H

#include <QLineEdit>

class QueryLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	QueryLineEdit(QWidget *parent = nullptr);

public slots:
	void insertAssist();
protected:
	virtual void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // QUERYLINEEDIT_H
