#include "QEJsonS11nTest.hpp"
#include "AnnotateClassOne.hpp"
#include <QEJsonS11n.hpp>
#include <QByteArray>
#include <QBuffer>
#include <QTest>

void QEJsonS11nTest::simpleTypes_data()
{
	QTest::addColumn<int>( "id");
	QTest::addColumn<QString>( "description");
	QTest::addColumn<double>( "realValue");
	QTest::addColumn<QString>( "expected");
	
	QTest::newRow("C1") << 5 << "Short description" << 3.14 
		<< "{\"desc\":\"Short description\",\"oid\":5}";
	QTest::newRow("C1") << -1 << "Description 'with' some \"rare\" symbols: @ { € # *" << 3.14 
		<< "{\"desc\":\"Description 'with' some \\\"rare\\\" symbols: @ { € # *\",\"oid\":-1}";
}

void QEJsonS11nTest::simpleTypes()
{
	AnnotateClassOne objSource, objTarget;
	QByteArray dataSource, dataTarget;

	QFETCH( int, id);
	QFETCH( QString, description);
	QFETCH( double, realValue);
	QFETCH( QString, expected);

	// Test Save
	QBuffer bufferTarget( &dataTarget);
	QVERIFY( bufferTarget.open( QIODevice::ReadWrite));

	objSource.setProperty( "id", id);
	objSource.setProperty( "description", description);
	objSource.setProperty( "realValue", realValue);

	QEJsonS11n s11nSave( &bufferTarget, nullptr, QJsonDocument::JsonFormat::Compact);
	s11nSave.save( &objSource);
	const QString dataTargetStr = QString::fromUtf8( dataTarget);
	QVERIFY( expected == dataTargetStr);

	// Test load
	dataSource = expected.toUtf8();
	QBuffer bufferSource( &dataSource);
	QVERIFY( bufferSource.open( QIODevice::ReadOnly));
	QEJsonS11n serilizer2( &bufferSource);
	
	serilizer2.load( &objTarget);
	QVERIFY( objSource.m_id == objTarget.m_id &&
		objSource.m_description == objTarget.m_description );
}

QTEST_MAIN(QEJsonS11nTest)
