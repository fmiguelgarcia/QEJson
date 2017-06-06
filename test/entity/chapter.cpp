#include "entity/chapter.hpp"
#include <utility>

using namespace std;

namespace {
	static const int ChapterPtrTypeId = qRegisterMetaType<Chapter*>();
}

Chapter::Chapter( QObject* parent, const QString& title) noexcept
	:QObject( parent), title( title)
{}

Chapter::Chapter( Chapter&& other) noexcept
	: QObject( nullptr),
	id( std::move( other.id)),
	title( std::move( other.title)),
	text( std::move( other.text)),
	footNotes( std::move( other.footNotes))
{
	setObjectName( other.objectName());
}

Chapter::Chapter( const Chapter& other) noexcept
	: QObject( nullptr),
	id( other.id),
	title( other.title),
	text( other.text),
	footNotes( other.footNotes)
{
	setObjectName( other.objectName());
}

void Chapter::swap( Chapter& other) noexcept
{
	id = other.id;
	title = other.title;
	text = other.text;
	footNotes = other.footNotes;
}

Chapter& Chapter::operator=( const Chapter& other)
{
	Chapter copyOther( other);
	swap( copyOther);

	return *this;
}

bool Chapter::operator==( const Chapter& other) const
{
	const QString oName = objectName();
	const QString otherName = other.objectName();
	
	return id == other.id
		&& title == other.title
		&& text == other.text
		&& oName == otherName
		&& footNotes == other.footNotes;
}
