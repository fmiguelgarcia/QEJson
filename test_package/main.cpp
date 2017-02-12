#include "QEJsonS11n.hpp"
#include <QByteArray>
#include <QBuffer>
#include <QObject>

int main(int argc, char** argv)
{
	QByteArray data;
	QBuffer buffer( &data);
	if( buffer.open( QIODevice::ReadWrite))
	{
		QEJsonS11n s11n( &buffer);
	}

	return 0;
}
