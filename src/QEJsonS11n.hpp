#pragma once
#include <QES11n.hpp>
#include <QObject>
#include <QJsonDocument>

class QEAnnotationModel;
class QIODevice;
class QEJsonS11n :  public QES11nBase
{
	public:
		QEJsonS11n( QIODevice* dev,
					QObject *parent = nullptr,
					const QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented );
		
		void save( const QObject* const source) const override;
		void load( QObject* const target) const override;
		QString mimeType() const override;
		
		const QEJsonS11n & operator<<( const QObject *const source) const;
		const QEJsonS11n & operator>>( QObject *const source) const;

	private:
		Q_DISABLE_COPY(QEJsonS11n)

		void save( const QObject* const o, QJsonObject& json) const;

		QJsonValue toJsonValue( const QVariant& value) const;
		QJsonValue nativeToJsonValue( const QVariant& value) const;
		QJsonValue userTypeToJsonValue( const QObject* obj) const;
		void writeProperty( const QEAnnotationModel& model, 
				const QString& propertyName, const QObject* const obj, 
				QJsonObject& jsonObj) const;

		QVariant fromJsonValue( const QJsonValue& value) const;
	
	private:
		
		QIODevice* const m_dev;
		const QJsonDocument::JsonFormat m_format;
};
