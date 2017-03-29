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
#include "QEJson.hpp"
#include "SerializedItem.hpp"
#include "SaveHelper.hpp"
#include "LoadHelper.hpp"
#include <qe/common/Exception.hpp>

#include <QFile>

using namespace qe::common;
using namespace qe::entity;
using namespace qe::json;
using namespace std;

namespace {
	SerializedItem * checkedCast(AbstractSerializedItem *item)
	{
		SerializedItem *const jsonTarget = dynamic_cast<SerializedItem*>( item);

		if (!jsonTarget)
			Exception::makeAndThrow(
					QStringLiteral("Json serializer only supports %1 as serialization item")
					.arg(typeid(SerializedItem).name()));

		return jsonTarget;
	}


}

QEJson &QEJson::instance()
{
	static unique_ptr<QEJson> instance;
	static once_flag m_onceFlag;

	call_once(m_onceFlag, [] { instance.reset(new QEJson);});
	return *instance.get();
}

QEJson::QEJson()
	: AbstractSerializer()
{} 

QJsonObject QEJson::parseOrThrow( const QByteArray& data) const
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

// Save
// ======================================================================

void QEJson::save( QObject* const source, 
	AbstractSerializedItem* const target) const
{ AbstractSerializer::save( source, target); }

void QEJson::save(ObjectContext &context, const ModelShd &model,
						QObject *const source,
						AbstractSerializedItem *const target) const
{
	SerializedItem *const jsonTarget = checkedCast(target);

	SaveHelper saver;
	saver.save(context, model, source, jsonTarget);
}

void QEJson::save( QObject* const source) const
{
	// By default, write to standar output
	QFile dev;
	dev.open( stdout, QIODevice::WriteOnly);

	SerializedItem si( &dev);
	save( source, &si); 
} 


// Load 
// ===========================================================================

void QEJson::load( const AbstractSerializedItem* const source, 
	QObject *const target) const
{ AbstractSerializer::load( source, target);}

void QEJson::load(ObjectContext &context, const ModelShd &model,
						const AbstractSerializedItem *const source,
						QObject *const target) const
{
	SerializedItem *const jsonSource =
		 checkedCast(const_cast<AbstractSerializedItem *>(source));

	LoadHelper loader;
	loader.load(context, model, jsonSource, target);
}
