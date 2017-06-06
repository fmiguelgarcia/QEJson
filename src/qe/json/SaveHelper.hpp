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
#include <qe/json/S11nContext.hpp>
#include <qe/json/TypeTraits.hpp>
#include <qe/entity/Types.hpp>
#include <QByteArray>
#include <QVariant>
#include <type_traits>

namespace qe { namespace json { 
	class SaveHelperPrivate;
	class S11nContext;

	class QEJSON_EXPORT SaveHelper
	{
		public:
			virtual ~SaveHelper();

			void save( 
				const entity::Model& model,
				QObject *const source, 
				S11nContext* const context) const;
				
			template<
				class T,
				class = typename std::enable_if<
					qe::json::is_json_type_supported<T>::value
					|| std::is_same<
						QVariant,
						typename std::decay<T>::type >::value ,
					int>::type
			>
			inline void save( 
				T&& source, 
				S11nContext* const context) const
			{
				saveFundamental( std::forward<T>(source), context);
			}

			template<
				class T,
				class = typename std::enable_if<
					qe::json::is_json_type_supported<T>::value
					|| std::is_same<
						QVariant,
						typename std::decay<T>::type >::value,
					int>::type
			>
			inline void save( 
				const T& source, 
				S11nContext* const context) const
			{
				saveFundamental( source, context);
			}

			template<
				class T,
				class = typename std::enable_if<
					std::is_base_of< QObject, T>::value,
					int>::type
			>
			inline void save( 
				T* const source,
				S11nContext* const context) const
			{
				saveObjectPointer( source, context);
			}

		protected:
			SaveHelperPrivate* d_ptr;

		private:
			template< class T>
			inline void saveFundamental( 
				T&& source, 
				S11nContext* const context) const
			{ context->setValue( fromVariant( std::forward<T>(source)));}

			template< class T>
			inline QJsonValue fromVariant( T&& source) const
			{ return QJsonValue::fromVariant( source); }
			
			inline QJsonValue fromVariant( const QByteArray& source) const
			{ return QJsonValue( QString::fromUtf8( source.toHex()));}

			inline QJsonValue fromVariant( QByteArray&& source) const
			{ return QJsonValue( QString::fromUtf8( source.toHex()));}
	
			void saveObjectPointer( 
				QObject* const source,
				S11nContext* const target) const;
		
			void saveOneToMany( 
				const entity::Model &model, 
				QObject *source, 
				S11nContext* const context) const;

			Q_DECLARE_PRIVATE( SaveHelper);
	};
}}
	
