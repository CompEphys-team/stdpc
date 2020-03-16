/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
