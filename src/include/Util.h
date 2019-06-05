/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <QString>
#include <QColor>

template<class T>
void getEntry(T &target, const T source, bool &change)
{
    T tmp = source;
    if (tmp != target) {
        target = tmp;
        change = true;
    }
}

/// A QString that saves/loads with quotes. Treat it like a QString, but get quoted AP values.
/// For backwards compatibility, operator>> will read from the first non-whitespace character it finds;
///  if that isn't a double quote ("), it'll read up to the next whitespace as if it were a normal string read.
class QuotedString : public QString {
public:
    template <typename... Args> QuotedString(Args... args) : QString(args...) {}
};
std::istream &operator>>(std::istream &is, QuotedString &str);
std::ostream &operator<<(std::ostream &os, const QuotedString &str);

std::istream &operator>>(std::istream &is, QString &str);
std::ostream &operator<<(std::ostream &os, const QString &str);

std::istream &operator>>(std::istream &is, QColor &col);
std::ostream &operator<<(std::ostream &os, const QColor &col);

#endif // UTIL_H
