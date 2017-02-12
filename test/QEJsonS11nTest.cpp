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
	AnnotateClassOne obj;
	QByteArray data;
	QBuffer buffer( &data);
	QVERIFY( buffer.open( QIODevice::ReadWrite));
	
	QEJsonS11n serilizer( &buffer, QJsonDocument::JsonFormat::Compact);

	QFETCH( int, id);
	QFETCH( QString, description);
	QFETCH( double, realValue);
	QFETCH( QString, expected);
	
	obj.setProperty( "id", id);
	obj.setProperty( "description", description);
	obj.setProperty( "realValue", realValue);

	serilizer.save( &obj);
	const QString dataStr = QString::fromUtf8(data); 
	QVERIFY( expected == dataStr);

	AnnotateClassOne obj2;
	QByteArray data2 = expected.toUtf8();
	QBuffer buffer2( &data2);
	QVERIFY( buffer2.open( QIODevice::ReadOnly));
	QEJsonS11n serilizer2( &buffer);
	
	serilizer2.load( &obj2);
	QVERIFY( obj.m_id == obj2.m_id &&
		obj.m_description == obj2.m_description );
}

QTEST_MAIN(QEJsonS11nTest)
