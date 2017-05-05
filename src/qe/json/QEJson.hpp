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
#pragma once
#include <qe/json/Global.hpp>
#include <qe/json/SerializedItem.hpp>
#include <qe/json/LoadHelper.hpp>
#include <qe/json/SaveHelper.hpp>
#include <qe/json/TypeTraits.hpp>
#include <qe/entity/AbstractSerializer.hpp>
#include <qe/entity/Types.hpp>
#include <QJsonObject>
#include <memory>
#include <mutex>
#include <utility>

#define QE_JSON_QEJSON_GCC_BUG 1

class QJsonObject;
namespace qe { namespace json {

    class QEJSON_EXPORT QEJson
		: public qe::entity::AbstractSerializer
	{
		public:
			static QEJson& instance();

			// Save to Serialized Item 
			// ===============================================================
			void save( QObject* const source, 
				entity::AbstractSerializedItem* const target) const override;
	
			void save( QObject* const source) const;
			
			// Save to SI
            template< class T>
			void save( T&& source, SerializedItem* const target) const
			{
				SaveHelper saver;
				saver.save( std::forward<T>(source), target);
			}
			
            template< class T>
			void save( const T& source, SerializedItem* const target) const
			{
				SaveHelper saver;
				saver.save( source, target);
			}


			// Load from Serialized Item
			// ===============================================================
	
			void load( const entity::AbstractSerializedItem* const source, 
				QObject *const target) const override;
		
			template< class T>
			void load( const SerializedItem* const source, 
				T&& target) const
			{
				LoadHelper loader;
				loader.load( source, std::forward<T>(target));
			}
				
			// Load from ByteArray
			template< class T>
			void load( const QByteArray& source,
				T&& target) const
			{
				load( parseOrThrow(source), std::forward<T>(target));
			}
			
			template< class T>
			void load( const QJsonObject& source,
				T&& target) const
			{
				SerializedItem si( source);
				load( &si, target);
			}


			// Save N parameters
			// ===================================================================	
			void saveN( SerializedItem& target) const 
			{}
	
			template< class T, typename ...Args>
			void saveN( SerializedItem& target, std::tuple<QString,T>& source, 
					Args&&... params) const
			{
				saveN( target, std::get<0>( source), std::get<1>( source),
					std::forward<Args>( params)...);
			}

			template< class T, typename ...Args>
			void saveN( SerializedItem& target, const QString& key, 
					T&& source, Args&&... params) const
			{
				SerializedItem part;
				save( part, source);
				
				target.insert( key, part.value());
				QJsonObject jsonObj = target.value().toObject();
				saveN( jsonObj, std::forward<Args>( params)...);
				
				target.setValue( jsonObj);
			}

			template<typename ...Args>
			void saveN( SerializedItem& target, Args&&... params) const
			{ saveN( target, std::forward<Args>( params)...);}


		protected:
			QEJson(); 
			QEJson( const QEJson&);
			QEJson& operator=( const QEJson& );
			
			void save( entity::ObjectContext& context, 
				const entity::ModelShd& model, QObject *const source, 
				entity::AbstractSerializedItem* const target) const override;

			void load( entity::ObjectContext& context, 
				const entity::ModelShd& model,
			  	const entity::AbstractSerializedItem *const source,
				QObject *const target) const override;
				
			QJsonObject parseOrThrow( const QByteArray& data) const;

		private:
	};
}}
