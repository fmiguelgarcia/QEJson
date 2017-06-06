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
#include "LoadHelper.hpp"
#include "S11nContext.hpp"
#include <qe/common/Exception.hpp>
#include <qe/entity/Model.hpp>
#include <qe/entity/ModelRepository.hpp>
#include <qe/entity/EntityDef.hpp>
#include <QStringBuilder>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

using namespace qe::json;
using namespace qe::entity;
using namespace qe::common;

namespace {
	QVariant toVariantByType( const int type, 
		const QJsonValue& jsonValue)
	{
		QVariant value;
		switch( type )
		{
			case QMetaType::Type::QByteArray:
				value = QByteArray::fromHex( jsonValue.toString().toLocal8Bit());
				break;
			default:
				value = jsonValue.toVariant();
		}
		return value;
	}

}

LoadHelper::~LoadHelper()
{}

void LoadHelper::load( 
	const Model& model,
	QObject*const target,
	const S11nContext* const context) const
{
	const QJsonValue value = context->value(); 
	if( value.isObject())
	{
		const QJsonObject jsonObj = value.toObject();
	
		loadObjectFromJson( model, jsonObj, target);
		loadOneToMany( model, target, context);
	}
}

void LoadHelper::loadObjectPointer( 
	QObject* const target,
	const S11nContext* const context) const
{
	const Model model = ModelRepository::instance().model( target->metaObject());

	load( model, target, context);
}

void LoadHelper::loadVariant(
	QVariant& target,
	const S11nContext* const context) const
{
	QJsonValue jsonValue = context->value();
	if( !jsonValue.isNull())
		target = jsonValue.toVariant();
	else
		target = QVariant();
}

QVariant LoadHelper::toVariant( const QJsonValue& value, QByteArray* ) const
{
	return QByteArray::fromHex( value.toString().toUtf8());
}

void LoadHelper::loadObjectFromJson( const Model& model, const QJsonObject& jsonObj, QObject* target) const
{
	for( const EntityDef& eDef : model.entityDefs())
	{
		if( eDef.mappedType() == EntityDef::MappedType::NoMappedType)
		{
			const QVariant value = toVariantByType( 
				eDef.propertyType(),
				jsonObj[ eDef.entityName()]);
			target->setProperty( eDef.propertyName(), value);
		}
	}
}

void LoadHelper::loadOneToMany( 
	const Model& model,
	QObject* const target,
	const S11nContext*const context) const
{
	ScopedS11Context _(target, context);
	
	for( const EntityDef& eDef : model.entityDefs())
	{
		if( eDef.mappedType() == EntityDef::MappedType::OneToMany)
		{
			const optional<Model> manyModel = eDef.mappedModel();
			if( manyModel)
			{
				const QJsonValue value = context->value();
				const QJsonObject jsonObj = value.toObject();
				const QJsonArray jsonArray = jsonObj[ eDef.entityName()]
					.toArray();
		
				QVariantList wrapperList;
				for( int i = 0; i < jsonArray.size(); ++i)
				{
					QVariant value;
					S11nContext itemCtxt( jsonArray[i]);

					const QMetaObject *mo = manyModel->metaObject();
					if( mo )
					{
						QObject * itemObj = mo->newInstance( Q_ARG(QObject*, target));
						if( !itemObj)
							Exception::makeAndThrow(
								QStringLiteral( "QE Json load helper cannot create and instance of class ")
								% mo->className());
		
						load( *manyModel, itemObj, &itemCtxt);
						value = QVariant::fromValue(itemObj);
					}
					else
					{
						load( value, &itemCtxt);
					}

					wrapperList.push_back( value);
				}
				target->setProperty( eDef.propertyName(), wrapperList);
			}
		}
	}
}
