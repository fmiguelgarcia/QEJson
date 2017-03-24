#include <qe/json/QEJson.hpp>
#include <QObject>

using namespace qe::json;

int main(int argc, char** argv)
{
	QObject * o = new QObject();
	QEJson::instance().save( o);

	return 0;
}
