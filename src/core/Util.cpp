#include "Util.h"
#include <sstream>

std::istream &operator>>(std::istream &is, QString &str)
{
    std::string tmp;
    is >> tmp;
    str = QString::fromStdString(tmp);
    return is;
}

std::ostream &operator<<(std::ostream &os, const QString &str)
{
    os << str.toStdString();
    return os;
}

std::istream &operator>>(std::istream &is, QColor &col)
{
    std::string tmp;
    is >> tmp;
    QStringList v = QString::fromStdString(tmp).split(',');
    if ( v.size() == 3 )
        col = QColor::fromRgb(v[0].toInt(), v[1].toInt(), v[2].toInt());
    else if ( v.size() == 4 )
        col = QColor::fromRgb(v[0].toInt(), v[1].toInt(), v[2].toInt(), v[3].toInt());
    return is;
}

std::ostream &operator<<(std::ostream &os, const QColor &col)
{
    os << col.red() << ',' << col.green() << ',' << col.blue() << ',' << col.alpha();
    return os;
}

std::istream &operator>>(std::istream &is, QuotedString &str)
{
    char c = '\0';
    enum {Preamble, Quoted, Unquoted, Done} state = Preamble;
    std::stringstream ss;
    while ( state != Done && (c = is.get()) != EOF ) {
        switch ( state ) {
        case Preamble:
            if ( !QChar(c).isSpace() ) {
                if ( c == '"' ) {
                    state = Quoted;
                } else {
                    state = Unquoted;
                    ss << c;
                }
            }
            break;
        case Quoted:
            if ( c == '"' )
                state = Done;
            else
                ss << c;
            break;
        case Unquoted:
            if ( QChar(c).isSpace() )
                state = Done;
            else
                ss << c;
            break;
        case Done:
            break;
        }
    }
    str = QString::fromStdString(ss.str());
    return is;
}

std::ostream &operator<<(std::ostream &os, const QuotedString &str)
{
    os << '"' << str.toStdString() << '"';
    return os;
}
