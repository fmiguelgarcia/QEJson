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
#pragma once
#include <qe/json/Global.hpp>
#include <qe/json/SerializedItem.hpp>
#include <qe/json/TypeTraits.hpp>
#include <qe/entity/Types.hpp>
#include <QByteArray>
#include <QVariant>
#include <type_traits>

namespace qe { namespace json { 

	class SerializedItem;
    class QEJSON_EXPORT SaveHelper
	{
		public:
			void save( entity::ObjectContext& context, 
				const entity::ModelShd& model, QObject *const source, 
				SerializedItem* const target) const;
				
			void save( QObject* const source,
				SerializedItem* const taget) const;

			template<
				class T,
				typename = typename std::enable_if< 
					qe::json::is_json_type_supported<T>::type, 
					int>::type
			>
			void save( T&& source, SerializedItem* const target) const
			{ target->setValue( fromVariant( std::forward<T>(source)));}
			
			template<
				class T /*,
				typename = typename std::enable_if< is_json_type_supported<T>::type, int>::type */
			>
			void save( const T& source, SerializedItem* const target) const
			{ target->setValue( fromVariant( source));}

		private:
			template< class T>
			inline QJsonValue fromVariant( T&& source) const
			{ return QJsonValue::fromVariant( source); }
			
			inline QJsonValue fromVariant( const QByteArray& source) const
			{ return QJsonValue( QString::fromUtf8( source.toHex()));}

			inline QJsonValue fromVariant( QByteArray&& source) const
			{ return QJsonValue( QString::fromUtf8( source.toHex()));}
			
			void saveOneToMany( entity::ObjectContext& context, 
				const entity::Model &model, QObject *source, 
				SerializedItem* const target) const;
	};
}}
	
