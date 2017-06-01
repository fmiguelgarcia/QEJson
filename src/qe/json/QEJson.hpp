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
#include <qe/json/S11nContext.hpp>
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
	class QEJsonPrivate;
	
	class SecuredS11Context
	{
		public:
			explicit SecuredS11Context( 
				const qe::entity::AbstractS11nContext* context);

			inline S11nContext* get() noexcept
			{ return const_cast<S11nContext*>( m_context); }
			
			inline const S11nContext* get() const noexcept
			{ return m_context; }

		private:
			const S11nContext * m_context;
			std::unique_ptr<const S11nContext> m_contextScopeGuard;
	};


	class QEJSON_EXPORT QEJson
		: public qe::entity::AbstractSerializer
	{
		public:
			static QEJson& instance();

			// Save to Serialized Item 
			// ===============================================================
			inline void save( 
				QObject* const source, 
				entity::AbstractS11nContext* const context = nullptr) const override
			{ qe::entity::AbstractSerializer::save( source, context);}
	
			// Save to SI
         template< class T>
			void save( T&& source, S11nContext* const context = nullptr) const
			{
				SaveHelper saver;
				saver.save( std::forward<T>(source), context);
			}
			
			template< class T>
			void save( const T& source, S11nContext* const context) const
			{
				SecuredS11Context sc( context);
				SaveHelper saver;
				saver.save( source, sc.get());
			}

			// Load from Serialized Item
			// ===============================================================
	
			inline void load( 
				QObject *const target,
				const entity::AbstractS11nContext* const context = nullptr) 
					const override
			{ qe::entity::AbstractSerializer::load( target, context);}
		
			template< class T>
			void load( 
				T&& target,
				const S11nContext* const context = nullptr) const
			{
				const SecuredS11Context sc( context);
				LoadHelper loader;
				loader.load( std::forward<T>(target), sc.get());
			}
				
			// Save N parameters
			// ===================================================================	
			void saveN( S11nContext& context) const 
			{}
	
			template< class T, typename ...Args>
			void saveN( 
				S11nContext& context, 
				std::tuple<QString,T>& source, 
				Args&&... params) const
			{
				saveN( context, std::get<0>( source), std::get<1>( source),
					std::forward<Args>( params)...);
			}

			template< class T, typename ...Args>
			void saveN( 
				S11nContext& context, 
				const QString& key, 
				T&& source, 
				Args&&... params) const
			{
				S11nContext contextPart;
				save( source, contextPart);
				
				context.insert( key, contextPart.value());
				QJsonObject jsonObj = context.value().toObject();
				saveN( jsonObj, std::forward<Args>( params)...);
				
				context.setValue( jsonObj);
			}

			template<typename ...Args>
			inline void saveN( 
				S11nContext& context,
				Args&&... params) const
			{ saveN( context, std::forward<Args>( params)...);}


		protected:
			QEJson(); 
			QEJson( const QEJson&);
			QEJson& operator=( const QEJson& );
			
			void save( 
				const entity::ModelShd& model, 
				QObject *const source, 
				entity::AbstractS11nContext* const context) const override;

			void load( 
				const entity::ModelShd& model,
				QObject *const target,
			  	const entity::AbstractS11nContext *const context) const override;
				
		private:
			Q_DECLARE_PRIVATE(QEJson);
	};
}}
