#pragma once
#include <AnnClassTwo.hpp>
#include <QEContainerWrapper.hpp>

class AnnotateClassOne : public QObject
{
	Q_OBJECT
	Q_PROPERTY( int id MEMBER m_id)
	Q_PROPERTY( QString description MEMBER m_description)
	Q_PROPERTY( double realValue MEMBER m_realValue)


	Q_CLASSINFO( "id", "@json.property.name=oid")
	Q_CLASSINFO( "description", "@json.property.name=desc")
	Q_CLASSINFO( "realValue",  "@json.property.enable=false")

	Q_PROPERTY( QVariantList classTwoList READ m_classTwoListWrapper WRITE m_classTwoListWrapper)
	
	public:
		Q_INVOKABLE double add( double a, double b) const;
		
	public:
		int m_id;
		QString m_description;
		double m_realValue;

		std::vector<AnnClassTwo> m_classTwoList;
		QEContainerWrapper< decltype( m_classTwoList)> m_classTwoListWrapper { m_classTwoList};
};
