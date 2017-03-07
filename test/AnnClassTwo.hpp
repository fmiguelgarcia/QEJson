#pragma once
#include <QObject>
#include <QStringList>

class AnnClassTwo : public QObject
{
	Q_OBJECT
	Q_PROPERTY( int id MEMBER m_id)
	Q_PROPERTY( QStringList strs MEMBER m_strings)

	public:
		Q_INVOKABLE explicit AnnClassTwo( QObject* parent = nullptr);
		AnnClassTwo( const AnnClassTwo& other);

	public:
		int m_id;
		QStringList m_strings;
};
Q_DECLARE_METATYPE( AnnClassTwo*);
Q_DECLARE_METATYPE( AnnClassTwo);
