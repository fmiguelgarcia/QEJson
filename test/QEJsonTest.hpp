#pragma once
#include <QObject>

class QEJsonTest: public QObject
{
	Q_OBJECT
	private slots:
		void checkSaveAutoIncrement();
		void checkSaveReferences();
};

