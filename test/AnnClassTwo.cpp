#include <AnnClassTwo.hpp>

namespace {
	static const int AnnClassTwoTypeId = qRegisterMetaType<AnnClassTwo*>();
}

AnnClassTwo::AnnClassTwo( QObject* parent)
	:QObject( parent)
{}

AnnClassTwo::AnnClassTwo( const AnnClassTwo& other)
	: m_id( other.m_id), m_strings( other.m_strings)
{}
