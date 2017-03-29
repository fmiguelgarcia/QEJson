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
#include <SerializedItem.hpp>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace qe::json;

SerializedItem::SerializedItem( QIODevice *dev)
	: m_dev( dev)
{}

SerializedItem::SerializedItem( const QJsonValue& value)
	: m_dev(nullptr)
{
	setValue( value);
}

SerializedItem::~SerializedItem()
{}

QByteArray SerializedItem::toJson( const QJsonDocument::JsonFormat format) const
{
	QJsonObject object;
	
	if( !m_value.isObject())
		object[ "value"] = m_value;
	else
		object = m_value.toObject();
	
	QJsonDocument doc( object);
	return doc.toJson( format);
}

void SerializedItem::flush( const QJsonDocument::JsonFormat format) const
{
	if( m_dev 
		&& m_dev->isOpen() 
		&& m_dev->isWritable())
	{
		m_dev->write( toJson( format));
	}
}

void SerializedItem::setValue(const QJsonValue& other)
{ m_value = other; }

QJsonValue SerializedItem::value() const
{ return m_value; }

void SerializedItem::insert(const QString& key, const QJsonValue& value)
{
	QJsonObject object = m_value.toObject();
	object.insert( key, value);
	m_value = object;
}
