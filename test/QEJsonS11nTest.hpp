#pragma once

#include <QObject>

class QEJsonS11nTest: public QObject
{
	Q_OBJECT
	private slots:
		void simpleTypes_data();
		void simpleTypes();
};

