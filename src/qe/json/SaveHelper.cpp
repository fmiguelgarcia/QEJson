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
#include "S11nContext.hpp"
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

	void insert( 
		const Model &model, 
		const QObject *source,
		S11nContext* const target)
	{
		for( const auto& eDef: model.entityDefs())
		{
			if( eDef.mappedType() == EntityDef::MappedType::NoMappedType)
			{
				const QByteArray& propName = eDef.propertyName();
				const QVariant propertyValue = source->property( propName);
				const QJsonValue jsonValue = toJsonValue( propertyValue);
				target->insert( eDef.entityName(), jsonValue);
			}
		}
	}
}

// SaveHelper
// ============================================================================

SaveHelper::~SaveHelper()
{}

void SaveHelper::save( 
	const ModelShd& model, 
	QObject *const source, 
	S11nContext* const context ) const
{
	if( context->isObjectInContext( source))
		Exception::makeAndThrow(
			QStringLiteral( "Json Save helper does NOT support recursive relations"));	

	insert( *model, source, context);
	saveOneToMany( *model, source, context);
}

void SaveHelper::saveObjectPointer( 
	QObject*const source,
	S11nContext*const target) const
{
	const ModelShd model = ModelRepository::instance().model( source->metaObject());
	save( model, source, target);
}

void SaveHelper::saveOneToMany( 
	const Model &model, 
	QObject *source, 
	S11nContext* const context) const
{
	ScopedS11Context _( source, context);
	
	for( const auto& eDef : model.entityDefs())
	{
		if( eDef.mappedType() == EntityDef::MappedType::OneToMany)
		{
			const QByteArray& propertyName = eDef.propertyName();
			const QVariant propertyValue = source->property( propertyName);
			if( ! propertyValue.canConvert<QVariantList>())
				Exception::makeAndThrow(
					QStringLiteral("QE Json can only use QVariantList for mapping property %1") 
						.arg( propertyName.constData()));
				
			QJsonArray jsonArray;
			QVariantList values = propertyValue.toList();
			for( QVariant& value : values)
			{
				S11nContext itemTarget;
				QObject *refItem = value.value<QObject*>();
				if( refItem )
				{
					ModelShd refModel = ModelRepository::instance().model( 
							refItem->metaObject());
					save( refModel, refItem, &itemTarget);
				}
				jsonArray.append( itemTarget.value());
			}
			context->insert( eDef.entityName(), jsonArray);
		}
	}
}
