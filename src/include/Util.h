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
