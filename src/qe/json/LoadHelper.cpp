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
#include "SerializedItem.hpp"
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

void LoadHelper::load( ObjectContext& context, 
	const ModelShd& model, const SerializedItem*const source, 
	QObject*const target) const
{
	const QJsonValue value = source->value(); 
	if( value.isObject())
	{
		const QJsonObject jsonObj = value.toObject();
	
		loadObjectFromJson( *model, jsonObj, target);
		loadOneToMany( context, model, source, target);
	}
}

void LoadHelper::load( const SerializedItem* const source,
	QObject* const target) const
{
	ObjectContext context;
	const ModelShd model = ModelRepository::instance().model( target->metaObject());

	load( context, model, source, target);
}




void LoadHelper::loadObjectFromJson( const Model& model, const QJsonObject& jsonObj, QObject* target) const
{
	for( const EntityDefShd& eDef : model.entityDefs())
	{
		if( eDef->mappingType() == EntityDef::MappingType::NoMappingType)
		{
			const QVariant value = toVariantByType( 
				eDef->propertyType(), 
				jsonObj[ eDef->entityName()]);
			target->setProperty( eDef->propertyName(), value);
		}
	}
}

void LoadHelper::loadOneToMany( ObjectContext& context, const ModelShd& model,
	const SerializedItem *const source, QObject* const target) const
{
	ScopedStackedObjectContext _(target, context);
	
	for( const EntityDefShd& eDef : model->entityDefs())
	{
		if( eDef->mappingType() == EntityDef::MappingType::OneToMany)
		{
			ModelShd manyModel = ModelRepository::instance().model( eDef->mappingEntity());
			const QJsonValue value = source->value();
			const QJsonObject jsonObj = value.toObject();
			const QJsonArray jsonArray = jsonObj[ eDef->entityName()]
				.toArray();
		
			QVariantList wrapperList;
			for( int i = 0; i < jsonArray.size(); ++i)
			{
				// 1. Create a new object
				QObject * itemObj = eDef->mappingEntity()->newInstance( 
					Q_ARG(QObject*, target));
				if( !itemObj)
					Exception::makeAndThrow(
						QStringLiteral( "QE Json load helper cannot create and instance of class ")
						% eDef->mappingEntity()->className());
		
				SerializedItem si( jsonArray[i]);
				load( context, manyModel, &si, itemObj); 
				wrapperList.push_back( QVariant::fromValue(itemObj));
			}
			target->setProperty( eDef->propertyName(), wrapperList);
		}
	}
}
