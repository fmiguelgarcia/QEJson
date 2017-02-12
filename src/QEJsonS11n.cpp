#include "QEJsonS11n.hpp"
#include <QEAnnotation.hpp>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QIODevice>
#include <stdexcept>
using namespace std;
namespace 
{
	inline QString JSON_ANN_PROPERTY_ENABLE() noexcept 
	{ return QLatin1Literal("@json.property.enable");}

	inline QString JSON_ANN_PROPERTY_NAME() noexcept 
	{ return QLatin1Literal("@json.property.name");}
}

QJsonValue toJsonValue( const QVariant& value, const int type);
QVariant fromJsonValue( const QJsonValue& value );
void write( QJsonObject& json, const QEAnnotationModel annModel, 
			const QObject* const obj, const QMetaProperty metaProperty);
void read( const QJsonObject &json, const QEAnnotationModel annModel, 
		   QObject *const obj, const QMetaProperty metaProperty);

QEJsonS11n::QEJsonS11n( QIODevice* dev, const QJsonDocument::JsonFormat format)
	: m_dev(dev), m_format(format)
{}

void QEJsonS11n::save( const QObject* const o) const
{
	const QMetaObject * mo = o->metaObject();
	const QEAnnotationModel annModel = QEAnnotation::registerModel( mo);
	QJsonObject json;

	for( int i = mo->propertyOffset() ; i < mo->propertyCount(); ++i)
		write( json, annModel, o, mo->property(i));

	QJsonDocument doc(json);
	const QByteArray jsonData = doc.toJson( m_format);
	m_dev->write( jsonData);
}

const QEJsonS11n &QEJsonS11n::operator<<(const QObject *const source) const
{
	save( source);
	return *this;
}

void QEJsonS11n::load(QObject *const target) const
{
	QJsonParseError jsonParseError;
	const QByteArray data = m_dev->readAll();
	QJsonDocument doc = QJsonDocument::fromJson( data, &jsonParseError);
	if( jsonParseError.error != QJsonParseError::NoError)
	{
		const QString errorMsg = target->tr("Json serializer parse error( at %1): %2")
			.arg( jsonParseError.offset).arg( jsonParseError.errorString()); 
		throw runtime_error( errorMsg.toStdString());
	}

	// Read all properties
	const QMetaObject * mo = target->metaObject();
	const QEAnnotationModel annModel = QEAnnotation::registerModel( mo);
	for( int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) 
		read( doc.object(), annModel, target, mo->property(i)); 
}

void read( const QJsonObject &json, const QEAnnotationModel annModel, QObject *const obj, const QMetaProperty metaProperty)
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
		QVariant propertyValue = fromJsonValue( json[jsonName]);
		obj->setProperty( metaProperty.name(), propertyValue);
	}
}

void write( QJsonObject &json, const QEAnnotationModel annModel, const QObject *const obj, const QMetaProperty metaProperty)
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

QVariant fromJsonValue( const QJsonValue& value )
{
	return value.toVariant();
}
