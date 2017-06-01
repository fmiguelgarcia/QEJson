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
#include <qe/json/TypeTraits.hpp>
#include <qe/json/S11nContext.hpp>
#include <qe/entity/Types.hpp>
#include <QVariant>
#include <QString>

class QJsonObject;
namespace qe { namespace json {
	class S11nContext;
	class LoadHelperPrivate;

	class QEJSON_EXPORT LoadHelper
	{
		public:
			virtual ~LoadHelper();

			void load( 
				const entity::ModelShd& model, 
				QObject *const target,
				const S11nContext *const source) const;

			/// @brief It loads fundamental types.
			template<
				class T,
				class = typename std::enable_if< 
					qe::json::is_json_type_supported<T>::value, 
					int>::type
			>
			inline void load( 
				T&& target,
				const S11nContext* const context) const
			{
				loadFundamental( std::forward<T>(target), context);
			}

			/// @brief It loads object pointers which class is a QObject derived class.
			template<
				class T,
				class = typename std::enable_if< 
					std::is_base_of< QObject, T>::value,
			  		int>::type
			>
			inline void load(
				T* target,
				const S11nContext* const context) const
			{
				loadObjectPointer( target, context);
			}

			/// @brief It loads into QVariant objects.
			template<
				class T,
				class = typename std::enable_if<
					std::is_same< QVariant, typename std::decay<T>::type>::value,
					int>::type
			>
			inline void load( 
				T& target,
				const S11nContext* const context) const
			{
				loadVariant( target, context);
			}

		protected:
			void loadObjectPointer(
				QObject* const target,
				const S11nContext* const context) const;

			void loadVariant(
				QVariant& target,
				const S11nContext* const context) const;

			template< class T>
			void loadFundamental(
				T&& target,
				const S11nContext* const context) const
			{
				QJsonValue jsonValue = context->value();
				if( jsonValue.isObject())
					jsonValue = jsonValue.toObject().value("value");

				const QVariant var = toVariant( jsonValue, std::addressof(target));
				target = var.value< typename std::remove_reference<T>::type> ();
			}

		protected:
			template< class T>
			inline QVariant toVariant( const QJsonValue& value, T* ) const
			{ return value.toVariant();}

			/// @brief It is only to transform to QByteArray when we know target
			/// type on compile time.
			QVariant toVariant( 
				const QJsonValue& value,
				QByteArray* ) const;
			
			void loadObjectFromJson( 
				const entity::Model& model,
				const QJsonObject& jsonObj, 
				QObject* target) const; 
			
			void loadOneToMany( 
				const entity::ModelShd& model, 
				QObject* const target,
				const S11nContext *const context) const;

			LoadHelperPrivate *d_ptr;

		private:
			Q_DECLARE_PRIVATE( LoadHelper);
	};
}}
