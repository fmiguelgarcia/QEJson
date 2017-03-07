/*
 * Copyright (C) 2017 francisco miguel garcia rodriguez  
 * Contact: http://www.dmious.com/qe/licensing/
 *
 * This file is part of the QE Common module of the QE Toolkit.
 *
 * $QE_BEGIN_LICENSE:LGPL21$
 * Commercial License Usage
 * Licensees holding valid commercial QE licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and The Dmious Company. For licensing terms
 * and conditions see http://www.dmious.com/qe/terms-conditions. For further
 * information use the contact form at http://www.dmious.com/contact-us.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 3 as published by the Free
 * Software Foundation and appearing in the file LICENSE.LGPLv3 
 * included in the packaging of this file. Please review the
 * following information to ensure the GNU Lesser General Public License
 * requirements will be met: https://www.gnu.org/licenses/lgpl.html and
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 * $QE_END_LICENSE$
 */
#include "QEJsonS11n.hpp"
#include <QEAnnotation.hpp>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIODevice>
#include <stdexcept>
using namespace std;

namespace 
{	inline QString JSON_ANN_CLASS_KEY() noexcept
	{ return QLatin1Literal( "class" );}
	inline QString JSON_ANN_PROPERTY_ENABLE() noexcept 
	{ return QLatin1Literal( "@json.property.enable");}
	inline QString JSON_ANN_PROPERTY_NAME() noexcept 
	{ return QLatin1Literal( "@json.property.name");}
	inline QString JSON_ANN_CLASS_PARENT() noexcept
	{ return QLatin1Literal( "@json.class.parent" );}
}

QEJsonS11n::QEJsonS11n( QIODevice* dev, QObject* parent, const QJsonDocument::JsonFormat format)
	: QES11nBase(parent), m_dev(dev), m_format(format)
{}

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

	const bool useParentProperties = annModel.annotation( JSON_ANN_CLASS_KEY(), JSON_ANN_CLASS_PARENT())
		.value( false).toBool();
	
	const int propertyBegin = (useParentProperties) ? (0) : (mo->propertyOffset());
	/*for( int i = propertyBegin; i < mo->propertyCount(); ++i)
		read( doc.object(), annModel, target, mo->property(i)); */
}

QString QEJsonS11n::mimeType() const 
{
	return QLatin1Literal("application/json; charset=UTF-8");
}

#if 0
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
#endif 

void QEJsonS11n::save( const QObject* const o) const
{
	QJsonObject json;
	save( o, json);	

	// Write document
	QJsonDocument doc(json);
	const QByteArray jsonData = doc.toJson( m_format);
	m_dev->write( jsonData);
}


void QEJsonS11n::save( const QObject* const o, QJsonObject& json) const
{
	const QMetaObject * mo = o->metaObject();
	const QEAnnotationModel annModel = QEAnnotation::registerModel( mo);
	const bool useParentProperties = annModel.annotation( JSON_ANN_CLASS_KEY(), JSON_ANN_CLASS_PARENT())
		.value( false).toBool();
	const int propertyBegin = (useParentProperties) ? (0) : (mo->propertyOffset());

	for( int i = propertyBegin ; i < mo->propertyCount(); ++i)
		writeProperty( annModel, mo->property(i).name(), o, json);
}

void QEJsonS11n::writeProperty( const QEAnnotationModel& model, 
		const QString& propertyName, const QObject* const obj, 
		QJsonObject& jsonObj) const
{
	const bool isJsonPropertyEnable = model.annotation( propertyName,
				JSON_ANN_PROPERTY_ENABLE()).value( true).toBool();

	if( isJsonPropertyEnable)
	{
		const QString jsonName = model.annotation( propertyName,
					JSON_ANN_PROPERTY_NAME()).value( propertyName).toString();
		const QVariant propertyValue = obj->property( propertyName.toLocal8Bit().constData());
		jsonObj.insert( jsonName, toJsonValue( propertyValue)); 
	}
}

QJsonValue QEJsonS11n::nativeToJsonValue( const QVariant& value) const
{
	QJsonValue jsonValue;
	const int type = value.type();	
	switch( type )
	{
		case QMetaType::QStringList:
			jsonValue = QJsonArray::fromStringList( value.toStringList());
			break;
		case QMetaType::QVariantList:
		{
			QJsonArray array;
			for( QVariant& item : value.toList())
				array.push_back( toJsonValue( item));
			jsonValue = array;
			break;
		}
		default:
			jsonValue = QJsonValue::fromVariant( value);
	}
	return jsonValue;
}

QJsonValue QEJsonS11n::userTypeToJsonValue( const QObject* obj) const
{
	const QMetaObject * mo = obj->metaObject();
	QJsonValue jsonValue;

	if( mo)
	{
		const QEAnnotationModel annModel = QEAnnotation::registerModel( mo);
		const bool useParentProperties = annModel.annotation( 
				JSON_ANN_CLASS_KEY(), JSON_ANN_CLASS_PARENT()).value( false).toBool();

		const int propertyBegin = (useParentProperties) ? (0) : (mo->propertyOffset());

		QJsonObject jsonObj;
		for( int i = propertyBegin ; i < mo->propertyCount(); ++i)
			writeProperty( annModel, mo->property(i).name(), obj, jsonObj);
		jsonValue = jsonObj;
	}
	
	return jsonValue;
}

QJsonValue QEJsonS11n::toJsonValue( const QVariant& value) const
{
	QJsonValue jsonValue;
	const int type = value.type();
	
	if( type < QVariant::UserType)
		jsonValue = nativeToJsonValue( value);
	else
	{
		if( value.canConvert<QObject*>())
		{
			QObject* rawObj = value.value<QObject*>();
			jsonValue = userTypeToJsonValue( rawObj);
		}
	}

	return jsonValue;
}

QVariant QEJsonS11n::fromJsonValue( const QJsonValue& value ) const
{
	return value.toVariant();
}
