#include "QEJsonTest.hpp"
#include "entity/book.hpp"
#include "entity/chapter.hpp"
#include <qe/json/QEJson.hpp>
#include <qe/json/SerializedItem.hpp>

#include <QCryptographicHash>
#include <QJsonArray>
#include <QTest>
using namespace qe::json;
using namespace qe::entity;

void QEJsonTest::checkSaveAutoIncrement()
{
	Book book;
	book.title = "Effective Modern C++";
	book.author = "Scott Meyers";
	book.pages = 303;
  	book.binSignature = QCryptographicHash::hash( book.title.toUtf8(),
		QCryptographicHash::Sha256);

	SerializedItem target; 
	QEJson::instance().save( &book, &target);
	QJsonObject jsonObject = target.value().toObject();
	
	QVERIFY( jsonObject[ QLatin1String("title")].toString() == book.title);
	QVERIFY( jsonObject[ QLatin1String("author")].toString() == book.author);
	QVERIFY( jsonObject[ QLatin1String("pages")].toInt() == book.pages);
	QVERIFY( jsonObject[ QLatin1String("binSignature")].toString().toLocal8Bit() == book.binSignature.toHex());
}

void QEJsonTest::checkSaveReferences()
{
	Book book;
	book.title = "Effective Modern C++";
	book.author = "Scott Meyers";
	book.pages = 303;
  	book.binSignature = QCryptographicHash::hash( book.title.toUtf8(),
		QCryptographicHash::Sha256);

	// Save book to get its id.
	SerializedItem target;
	QEJson::instance().save( &book, &target);
	QJsonObject jsonObject = target.value().toObject();
	
	QVERIFY( jsonObject[ QLatin1String("title")].toString() == book.title);
	QVERIFY( jsonObject[ QLatin1String("author")].toString() == book.author);
	QVERIFY( jsonObject[ QLatin1String("pages")].toInt() == book.pages);
	QVERIFY( jsonObject[ QLatin1String("binSignature")].toString().toLocal8Bit() == book.binSignature.toHex());

	// Chapters don't use autoincrement id, so we generate them from book.id
	Chapter ch1( nullptr, "Deducing Types");
	ch1.text = "- During template type deduction, arguments that are references "
		"are treated as non-references, i.e., their reference-ness is ignored."
		"\n - When deducing types for universal reference parameters, lvalue "
		"arguments get special treatment.";
	ch1.id = 1 + book.id * 1000;
	ch1.setObjectName( "Chapter 1");
	
	Chapter ch2( nullptr, "Understand auto type deduction");
	ch2.text = " - auto type deduction is usually the same as template type "
		"deduction, but auto type  deduction  assumes  that  a  braced  "
		"initializer  represents  a  std::initializer_list, and template type"
		"deduction doesn’t.\n -  auto in  a  function  return  type  or  a "
		"lambda  parameter  implies  template  type deduction, not auto type "
		"deduction.";
	ch2.id = 2 + book.id * 1000;

	// Update book and save.
	book.chapters = { ch1, ch2};
	QEJson::instance().save( &book, &target);

	jsonObject = target.value().toObject();
	const QJsonArray jsonChapters = jsonObject["chapters"].toArray();
	QVERIFY( static_cast<uint>(jsonChapters.size()) == book.chapters.size());
	
	for( int i = 0; i < jsonChapters.size(); ++i)
	{
		const Chapter & ch = book.chapters[i];
		const QJsonObject jsonCh = jsonChapters[i].toObject();
		
		QVERIFY( ch.id == jsonCh[ "id"].toInt());
		QVERIFY( ch.objectName() == jsonCh[ "objectName"].toString());
		QVERIFY( ch.text == jsonCh[ "text"].toString());
		QVERIFY( ch.title == jsonCh[ "title"].toString());
	}
	
	Book loadedBook;
	QEJson::instance().load( &target, &loadedBook);

	QVERIFY( book == loadedBook);
}

void QEJsonTest::checkLoadHelperBoolType_data()
{
	QTest::addColumn<bool>( "input");
	
	QTest::newRow("1") << true;
	QTest::newRow("2") << false;
}

void QEJsonTest::checkLoadHelperBoolType()
{ 
	QFETCH( bool, input);
	bool output;
	SerializedItem si;
	
	si.setValue( QJsonValue::fromVariant( input));
	QEJson::instance().load(  &si, output);
	QVERIFY( input == output);
}

void QEJsonTest::checkLoadHelperIntType_data()
{
	QTest::addColumn<int>( "input");
	
	QTest::newRow("1") << 0;
	QTest::newRow("2") << 1024;
	QTest::newRow("3") << -1024;
	QTest::newRow("4") << std::numeric_limits<int>::max();
	QTest::newRow("5") << std::numeric_limits<int>::min();
	QTest::newRow("6") << std::numeric_limits<int>::max() +1;
	QTest::newRow("7") << std::numeric_limits<int>::min() -1;
}

void QEJsonTest::checkLoadHelperIntType()
{ 
	QFETCH( int, input);
	int output;
	SerializedItem si;
	
	si.setValue( QJsonValue::fromVariant( input));
	QEJson::instance().load(  &si, output);
	QVERIFY( input == output);
}

void QEJsonTest::checkLoadHelperDoubleType_data()
{
	QTest::addColumn<double>( "input");
	
	QTest::newRow("1") << 0.0;
	QTest::newRow("2") << 13.14159265359;
	QTest::newRow("3") << std::numeric_limits<double>::max();
	QTest::newRow("4") << std::numeric_limits<double>::min();
	QTest::newRow("5") << std::numeric_limits<double>::max() +1.01;
	QTest::newRow("6") << std::numeric_limits<double>::min() -1.01;
}

void QEJsonTest::checkLoadHelperDoubleType()
{ 
	QFETCH( double, input);
	double output;
	SerializedItem si;
	
	si.setValue( QJsonValue::fromVariant( input));
	QEJson::instance().load(  &si, output);
	QVERIFY( input == output);
}

void QEJsonTest::checkLoadHelperStringType_data()
{
	QTest::addColumn<QString>( "input");
	
	QTest::newRow("1") << QString("QString test");
	QTest::newRow("2") << QString("Some € extrange characters ñÇ");
	QTest::newRow("3") << QString::fromUtf8( u8"Some € extrange characters ñÇ");
	QTest::newRow("5") << QString::fromUtf16( u"Some € extrange characters ñÇ");
	QTest::newRow("4") << QString::fromWCharArray( L"Some € extrange characters ñÇ");
	QTest::newRow("6") << QString::fromUcs4( U"Some € extrange characters ñÇ");
}

void QEJsonTest::checkLoadHelperStringType()
{ 
	QFETCH( QString, input);
	QString output;
	SerializedItem si;
	
	si.setValue( QJsonValue::fromVariant( input));
	QEJson::instance().load(  &si, output);
	QVERIFY( input == output);
}

void QEJsonTest::checkLoadHelperQByteArrayType_data()
{
	QTest::addColumn<QByteArray>( "input");
	
	QTest::newRow("1") << QByteArrayLiteral("QString test");
	QTest::newRow("2") << QByteArray::fromHex( QByteArrayLiteral("AD0000374E"));
	QTest::newRow("3") << QByteArray();
}

void QEJsonTest::checkLoadHelperQByteArrayType()
{ 
	QFETCH( QByteArray, input);
	QByteArray output;
	SerializedItem si;
	
	si.setValue( QJsonValue::fromVariant( input.toHex()));
	QEJson::instance().load(  &si, output);
	QVERIFY( input == output);
}

#if 0
void QEJsonTest::checkLoadHelperNativeTypes()
{
	SerializedItem si;

	// QMetaType::Bool
	bool boolValue = false;
	si.setValue( QJsonValue::fromVariant( true));
	QEJson::instance().load( &si, boolValue);
	QVERIFY( si.value().toBool() == boolValue);
	si.setValue( QJsonValue::fromVariant( false));
	QEJson::instance().load( &si, boolValue);
	QVERIFY( si.value().toBool() == boolValue);
	
	// QMetaType::Int
	int integer;
	si.setValue( QJsonValue::fromVariant( 13));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	si.setValue( QJsonValue::fromVariant( -1024));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< int >::max() ));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< int >::min() ));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< int >::max() +1 ));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< int >::min() -1 ));
	QEJson::instance().load(  &si, integer);
	QVERIFY( si.value().toInt() == integer);
	
	// QMetaType::UInt
	uint uInteger;
	si.setValue( QJsonValue::fromVariant( 1024u));
	QEJson::instance().load(  &si, uInteger);
	QVERIFY( si.value().toVariant().toUInt() == uInteger);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< uint >::min()));
	QEJson::instance().load(  &si, uInteger);
	QVERIFY( si.value().toVariant().toUInt() == uInteger);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< uint >::max()));
	QEJson::instance().load(  &si, uInteger);
	QVERIFY( si.value().toVariant().toUInt() == uInteger);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< uint >::max() +1));
	QEJson::instance().load(  &si, uInteger);
	QVERIFY( si.value().toVariant().toUInt() == uInteger);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits< uint >::min() -1));
	QEJson::instance().load(  &si, uInteger);
	QVERIFY( si.value().toVariant().toUInt() == uInteger);
	
	// QMetaType Double
	double doubleValue;
	si.setValue( QJsonValue::fromVariant( 3.14159265358 ));
	QEJson::instance().load(  &si, doubleValue);
	QVERIFY( si.value().toDouble() == doubleValue);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits<double>::max()));
	QEJson::instance().load(  &si, doubleValue);
	QVERIFY( si.value().toDouble() == doubleValue);
	si.setValue( QJsonValue::fromVariant( std::numeric_limits<double>::min()));
	QEJson::instance().load(  &si, doubleValue);
	QVERIFY( si.value().toDouble() == doubleValue);
	
	// QMetaType QChar
	QChar qCharValue;
	si.setValue( QJsonValue::fromVariant( QChar('a')));
	QEJson::instance().load(  &si, qCharValue);
	QVERIFY( si.value().toVariant().toChar() == qCharValue);
	si.setValue( QJsonValue::fromVariant( QChar( L'€')));
	QEJson::instance().load(  &si, qCharValue);
	QVERIFY( si.value().toVariant().toChar() == qCharValue);
	si.setValue( QJsonValue::fromVariant( u'ñ'));
	QEJson::instance().load(  &si, qCharValue);
	QVERIFY( si.value().toVariant().toChar() == qCharValue);
	si.setValue( QJsonValue::fromVariant( U'ñ'));
	QEJson::instance().load(  &si, qCharValue);
	QVERIFY( si.value().toVariant().toChar() == qCharValue);

	// QMetaType::QString
	QString qstrValue;
	si.setValue( QJsonValue::fromVariant( QString("Test string")));
	QEJson::instance().load(  &si, qstrValue);
	QVERIFY( si.value().toString() == qstrValue);

}
#endif

void QEJsonTest::checkSaveHelper()
{
	SerializedItem si;

	const QString str( "Text"); 
	QEJson::instance().save( str, &si);
	QVERIFY( si.value().toString() == str);

	const QByteArray ba( "AB1010");
	QEJson::instance().save( ba, &si);
	const QString siValue = si.value().toString();
	QVERIFY( siValue == ba.toHex());
	
	const int integer = 13;
	QEJson::instance().save( integer , &si);
	QVERIFY( si.value().toInt() == integer);
	
	const double real = 3.14;
	QEJson::instance().save( real, &si);
	QVERIFY( si.value().toDouble() == real);
}


QTEST_MAIN(QEJsonTest)
