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
#include <S11nContext.hpp>
#include <qe/common/Exception.hpp>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace qe::json;
using namespace qe::common;

namespace {
	
	QJsonObject parseOrThrow( const QByteArray& data)
	{
		QJsonParseError parseError;
		QJsonDocument doc = QJsonDocument::fromJson( data, &parseError);
		if( parseError.error != QJsonParseError::NoError)
		{
			Exception::makeAndThrow(
					QString( "QE Json has found a parse error at offset %1: %2")
					.arg( parseError.error)
					.arg( parseError.errorString()));
		}
		return doc.object();
	}
}



S11nContext::S11nContext( QIODevice *dev)
	: AbstractS11nContext( QVariantList()),
	m_dev( dev)
{}

S11nContext::S11nContext( const QJsonValue& value)
	: AbstractS11nContext( QVariantList()),
	m_dev(nullptr)
{
	setValue( value);
}

#if 0
S11nContext::S11nContext( const QJsonObject& json)
	: AbstractS11nContext( QVariantList()),
	m_dev(nullptr)
{
	setValue( json);
}
#endif

S11nContext::S11nContext( const QByteArray& json)
	: S11nContext( parseOrThrow( json))
{}
			

S11nContext::~S11nContext()
{}

QByteArray S11nContext::toJson( const QJsonDocument::JsonFormat format) const
{
	QJsonObject object;
	
	if( !m_value.isObject())
		object[ "value"] = m_value;
	else
		object = m_value.toObject();
	
	QJsonDocument doc( object);
	return doc.toJson( format);
}

void S11nContext::flush( const QJsonDocument::JsonFormat format) const
{
	if( m_dev 
		&& m_dev->isOpen() 
		&& m_dev->isWritable())
	{
		m_dev->write( toJson( format));
	}
}

void S11nContext::setValue(const QJsonValue& other)
{ m_value = other; }

QJsonValue S11nContext::value() const
{ return m_value; }

void S11nContext::insert(const QString& key, const QJsonValue& value)
{
	QJsonObject object = m_value.toObject();
	object.insert( key, value);
	m_value = object;
}
