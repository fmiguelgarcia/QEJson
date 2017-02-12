#pragma once
#include <QObject>
#include <QJsonDocument>

class QIODevice;
class QEJsonS11n
{
	public:
		QEJsonS11n( QIODevice* target, 
					const QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented );
		void save( const QObject* const source) const;
		
		const QEJsonS11n & operator<<( const QObject *const source) const;

	private:
		QIODevice* const m_target;
		const QJsonDocument::JsonFormat m_format;
};
