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
#include "QEJson.hpp"
#include "S11nContext.hpp"
#include "SaveHelper.hpp"
#include "LoadHelper.hpp"
#include <qe/common/Exception.hpp>

#include <QFile>

using namespace qe::common;
using namespace qe::entity;
using namespace qe::json;
using namespace std;

/**
 * \class SecuredS11Context
 * \brief 
 *
 */

SecuredS11Context::SecuredS11Context( const AbstractS11nContext* context)
	: m_context( dynamic_cast<const S11nContext*>( context))
{
	if( !m_context)
	{
		m_contextScopeGuard.reset( new S11nContext);
		m_context = m_contextScopeGuard.get();
	}
}

/**
 * \class QEJson
 * \brief
 *
 */

QEJson &QEJson::instance()
{
	static unique_ptr<QEJson> instance;
	static once_flag m_onceFlag;

	call_once(m_onceFlag, [] { instance.reset(new QEJson);});
	return *instance.get();
}

QEJson::QEJson()
	: AbstractSerializer()
{} 

// Save
// ======================================================================

void QEJson::save(
	const Model &model,
	QObject *const source,
	AbstractS11nContext *const context) const
{
	SecuredS11Context sc( context);
	SaveHelper saver;

	saver.save( model, source, sc.get());
}

// Load 
// ===========================================================================

void QEJson::load(
	const Model &model,
	QObject *const target,
	const AbstractS11nContext *const context) const
{
	const SecuredS11Context sc( context);
	LoadHelper loader;
	loader.load( model, target, sc.get());
}
