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
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <type_traits>

namespace qe { namespace json { 

	template<typename...>
	struct __or_;

	template<>
	struct __or_<>
	: public std::false_type
	{ };
	
	template<typename _B1>
	struct __or_<_B1>
	: public _B1
	{ };
	
	template<typename _B1, typename _B2>
	struct __or_<_B1, _B2>
	: public std::conditional<_B1::value, _B1, _B2>::type
	{ };
	
	template<typename _B1, typename _B2, typename _B3, typename... _Bn>
	struct __or_<_B1, _B2, _B3, _Bn...>
	: public std::conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
	{ };
	
	/// @brief It is used to ensure that just some types are processed as native.
	///
	/// Supported types are:
	///		- QString
	///		- QByteArray
	///		- Integral types: int, short, float, double, etc.
	///		.
	template <class T>
	struct is_json_type_supported 
		: public __or_<
			typename std::is_same< 
				QString, 
				typename std::decay<T>::type >::type
			,typename std::is_same<
				QByteArray,
				typename std::decay<T>::type >::type
            /*,typename std::is_same<
				QVariant,
                typename std::decay<T>::type >::type*/
			,typename std::is_same<
				QChar,
				typename std::decay<T>::type >::type
			,typename std::is_arithmetic<
				typename std::decay<T>::type >::type
		>::type
	{};
	
	/*			
		private:
			typedef typename std::decay<T>::type U;
			
		public:
			typedef typename std::integral_constant<
				bool,
				std::is_same< QString, U >::value ||
				std::is_same< QByteArray, U>::value || 
				std::is_arithmetic< U>::value
			>::type type;
	};
	*/
	
}}
