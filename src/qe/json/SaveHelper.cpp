/*
 * Copyright (C) 2017 Francisco Miguel García Rodríguez
 * Contact: http://www.dmious.com/qe/licensing/
 *
 * This file is part of the QE Common module of the QE Toolkit.
 *
 * $QE_BEGIN_LICENSE$
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
 * General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL3 included in the
 * packaging of this file. Please review the following information to
 * ensure the GNU Lesser General Public License version 3 requirements
 * will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
 *
 * $QE_END_LICENSE$
 */
#include "SaveHelper.hpp"
#include "SerializedItem.hpp"
#include <qe/common/Exception.hpp>
#include <qe/entity/Model.hpp>
#include <qe/entity/ModelRepository.hpp>
#include <qe/entity/EntityDef.hpp>

#include <QJsonObject>
#include <QJsonArray>

using namespace qe::entity;
using namespace qe::common;
using namespace qe::json;

namespace {

	QJsonValue toJsonValue( const QVariant& value)
	{
		QJsonValue jsonValue;
		switch( value.type())
		{
			case QMetaType::Type::QByteArray:
				jsonValue = value.toByteArray().toHex().constData();
				break;
			default:
				jsonValue = QJsonValue::fromVariant( value);
		}

		return jsonValue;
	}

	void insert( ObjectContext& context, const Model &model, const QObject *source,
					 SerializedItem* const target)
	{
		for( const auto& eDef: model.entityDefs())
		{
			if( eDef->mappingType() == EntityDef::MappingType::NoMappingType)
			{
				const QVariant propertyValue = source->property( eDef->propertyName());
				const QJsonValue jsonValue = toJsonValue( propertyValue);
				target->insert( eDef->entityName(), jsonValue);
			}
		}
	}
}

// SaveHelper
// ============================================================================

void SaveHelper::save( ObjectContext& context, const ModelShd& model, 
	QObject *const source, SerializedItem* const target) const
{
	if( find( begin(context), end(context), source) != end(context))
		Exception::makeAndThrow(
			QStringLiteral( "Json Save helper does NOT support recursive relations"));	

	insert( context, *model, source, target);
	saveOneToMany( context, *model, source, target);
}

void SaveHelper::save( QObject*const source, SerializedItem*const target) const
{
	ObjectContext context;
	const ModelShd model = ModelRepository::instance().model( source->metaObject());
	save( context, model, source, target);
}



/// @todo Move this to common class? 
void SaveHelper::saveOneToMany( ObjectContext& context, const Model &model, 
		QObject *source, SerializedItem* const target) const
{
	ScopedStackedObjectContext _( source, context);
	
	for( const auto& eDef : model.entityDefs())
	{
		if( eDef->mappingType() == EntityDef::MappingType::OneToMany)
		{
			const QByteArray& propertyName = eDef->propertyName();
			const QVariant propertyValue = source->property( propertyName);
			if( ! propertyValue.canConvert<QVariantList>())
				Exception::makeAndThrow(
					QStringLiteral("QE Json can only use QVariantList for mapping property %1") 
						.arg( propertyName.constData()));
				
			QJsonArray jsonArray;
			QVariantList values = propertyValue.toList();
			for( QVariant& value : values)
			{
				SerializedItem itemTarget;
				QObject *refItem = value.value<QObject*>();
				if( refItem )
				{
					ModelShd refModel = ModelRepository::instance().model( 
							refItem->metaObject());
					save( context, refModel, refItem, &itemTarget);
				}
				jsonArray.append( itemTarget.value());
			}
			target->insert( eDef->entityName(), jsonArray);
		}
	}
}
