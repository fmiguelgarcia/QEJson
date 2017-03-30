#pragma once
#include <QObject>

class QEJsonTest: public QObject
{
	Q_OBJECT
	private slots:
		void checkSaveAutoIncrement();
		void checkSaveReferences();
		
		void checkSaveHelper();
		
		// Load Helper test
		void checkLoadHelperBoolType_data();
		void checkLoadHelperBoolType();
		void checkLoadHelperIntType_data();
		void checkLoadHelperIntType();
		void checkLoadHelperDoubleType_data();
		void checkLoadHelperDoubleType();
		
		void checkLoadHelperStringType_data();
		void checkLoadHelperStringType();

		void checkLoadHelperQByteArrayType_data();
		void checkLoadHelperQByteArrayType();

		void checkLoadHelperQVariantType_data();
		void checkLoadHelperQVariantType();
};

