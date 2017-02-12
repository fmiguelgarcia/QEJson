#pragma once
#include <QECommon/QES11n.hpp>
#include <QObject>
#include <QJsonDocument>

class QIODevice;
class QEJsonS11n :  public QES11nBase
{
	public:
		QEJsonS11n( QIODevice* dev, 
					const QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented );
		
		void save( const QObject* const source) const override;
		void load( QObject* const target) const override;
		
		const QEJsonS11n & operator<<( const QObject *const source) const;
		const QEJsonS11n & operator>>( QObject *const source) const;

	private:
		Q_DISABLE_COPY(QEJsonS11n)
		
		QIODevice* const m_dev;
		const QJsonDocument::JsonFormat m_format;
};
