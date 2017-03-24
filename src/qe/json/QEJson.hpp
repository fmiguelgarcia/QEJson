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
#include <qe/entity/serialization/AbstractSerializer.hpp>
#include <qe/entity/Types.hpp>
#include <memory>
#include <mutex>

#define QE_JSON_QEJSON_GCC_BUG 1

class QJsonObject;
namespace qe { namespace json {

	class QEJson 
		: public qe::entity::AbstractSerializer
	{
		public:
			static QEJson& instance();

			void save( QObject* const source) const;
			
			void load( const QJsonObject& source, 
					   QObject* const target) const;

#if QE_JSON_QEJSON_GCC_BUG 
			void save( QObject* const source, 
				entity::AbstractSerializedItem* const target) const override;
			
			void load( const entity::AbstractSerializedItem* const source, 
				QObject *const target) const override;
#endif

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

		private:
	};
}}
