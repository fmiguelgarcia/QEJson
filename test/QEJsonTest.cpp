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


QTEST_MAIN(QEJsonTest)
