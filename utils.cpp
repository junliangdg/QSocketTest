#include "utils.h"
#include <QDateTime>

namespace Utils {

QString getCurrentTimeString()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}


}
