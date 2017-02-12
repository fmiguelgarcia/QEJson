#include "QEJsonS11n.hpp"
#include <QAnnotation.hpp>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIODevice>

namespace 
{
	inline QString JSON_ANN_PROPERTY_ENABLE() noexcept 
	{ return QLatin1Literal("@json.property.enable");}

	inline QString JSON_ANN_PROPERTY_NAME() noexcept 
	{ return QLatin1Literal("@json.property.name");}
}

QJsonValue toJsonValue( const QVariant& value, const int type);
void write( QJsonObject& json, const QAnnotationModel annModel, 
			const QObject* const obj, const QMetaProperty metaProperty);

QEJsonS11n::QEJsonS11n( QIODevice* target, const QJsonDocument::JsonFormat format)
	: m_target(target), m_format(format)
{}

void QEJsonS11n::save( const QObject* const o) const
{
	const QMetaObject * mo = o->metaObject();
	const QAnnotationModel annModel = QAnnotation::registerModel( mo);
	QJsonObject json;

	for( int i = mo->propertyOffset() ; i < mo->propertyCount(); ++i)
		write( json, annModel, o, mo->property(i));

	QJsonDocument doc(json);
	const QByteArray jsonData = doc.toJson( m_format);
	m_target->write( jsonData);
}

const QEJsonS11n &QEJsonS11n::operator<<(const QObject *const source) const
{
	save( source);
	return *this;
}


void write( QJsonObject& json, const QAnnotationModel annModel, 
			const QObject* const obj, const QMetaProperty metaProperty)
{
	const QString propName = metaProperty.name();
	const bool isJsonPropertyEnable = annModel.annotation(
		propName,
		JSON_ANN_PROPERTY_ENABLE()).value( true).toBool();

	if( isJsonPropertyEnable)
	{
		const QString jsonName = annModel.annotation(
			propName,
			JSON_ANN_PROPERTY_NAME()).value( propName).toString();
		const QVariant propertyValue = metaProperty.read( obj);
		
		json.insert( jsonName, toJsonValue( propertyValue, metaProperty.type())); 
	}
}

QJsonValue toJsonValue( const QVariant& value, const int type)
{
	/// @todo Make this recursive for complex types.
	return QJsonValue::fromVariant( value);
}
