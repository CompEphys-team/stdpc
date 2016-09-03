#ifndef AP_H
#define AP_H

#include <iostream>
#include <QString>
#include <QRegularExpression>
#include <vector>
#include <functional>
#include <memory>

class AP;
extern std::vector<std::unique_ptr<AP>> params;
std::unique_ptr<AP> const& initAP();

/**
 * @brief Add a parameter to the global registry. See @fn initAP() for examples.
 * @param name: A string version of the fully qualified name of the parameter
 * as you'd use it elsewhere in the code, replacing indices with a single '#'.
 * @param head: A pointer to the top-level variable that contains your parameter
 * @param tail...: Successive member pointers needed to reach the parameter
 * starting from @arg head, ignoring any vector or array indices.
 */
template <typename T, typename... Tail>
inline void addAP(QString name, T *head, Tail... tail);

class AP
{
public:
    inline void readNow(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        this->readLater(rawName, is, ok)();
    }

    /**
     * @brief Returns a lambda to set the parameter to the next value on @arg is.
     *
     * @arg rawName must be an unfiltered string containing at least as many
     * distinct numbers as there are hashes in the parameter's name. rawName
     * is only used for index extraction; no name checking is performed.
     **/
    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr) = 0;

    /**
     * @brief Write all values of this parameter to @arg os.
     *
     * The output format is "paramname value\n", with a separate line for each
     * instance (i.e. index) of the parameter. Parameter instances are traversed
     * in depth-first fashion. Each '#' in the name is replaced with the
     * appropriate index. Additional occurrences of '#' may confuse the algorithm
     * and are therefore strongly discouraged.
     */
    virtual void write(std::ostream &os) = 0;

    static std::unique_ptr<AP> const& find(QString rawName)
    {
        rawName.replace(QRegularExpression("\\[\\d+\\]"), "[#]");
        return *std::find_if(params.begin(), params.end(),
                             [&](std::unique_ptr<AP> &a){return !rawName.compare(a->name);});
    }

    /**
     * @brief Mark this AP deprecated, delegating reading to target and disabling @fn write.
     *
     * Indices in the target name are matched in order, defaulting to 0 if self has fewer indices than target.
     */
    void deprecate(std::unique_ptr<AP> const& target)
    {
        deprecated = target.get();
    }

protected:
    AP(QString &name) : name(name), deprecated(nullptr) {}
    QString name;
    AP *deprecated;
};


namespace APFunc {

template <typename T, typename S, typename... Tail>
std::function<void(T&)> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                    T &head, S T::* index, Tail... tail);
template <typename T, size_t SZ, typename... Tail>
std::function<void(T (&)[SZ])> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                           T (&head)[SZ], Tail... tail);
template <typename T, typename... Tail>
std::function<void(std::vector<T>&)> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                                 std::vector<T> &head, Tail... tail);

template <typename T>
std::function<void(T&)> getReadFunc(QString &, int, std::istream &is, bool *ok, T&)
{
    bool good = is.good();
    if ( ok )
        *ok = good;
    if ( !good )
        return [](T&){};
    T val;
    is >> val;
    return [=](T& v){ v = val; };
}

template <typename T, typename... Tail>
std::function<void(std::vector<T>&)> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                                 std::vector<T> &head, Tail... tail)
{
    QRegularExpressionMatch idxMatch = QRegularExpression("\\[(\\d+)\\]").match(name, offset);
    if ( idxMatch.hasMatch() ) {
        offset = idxMatch.capturedEnd(0);
        size_t index = idxMatch.captured(1).toUInt();
        if ( index >= head.size() )
            head.resize(index+1);
        std::function<void(T&)> func = getReadFunc(name, offset, is, ok, head[index], tail...);
        return [=](std::vector<T>& v){ func(v[index]); };
    } else {
        if (ok) *ok = false;
        return [](std::vector<T>&){};
    }
}

template <typename T, size_t SZ, typename... Tail>
std::function<void(T (&)[SZ])> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                           T (&head)[SZ], Tail... tail)
{
    QRegularExpressionMatch idxMatch = QRegularExpression("\\[(\\d+)\\]").match(name, offset);
    if ( idxMatch.hasMatch() ) {
        offset = idxMatch.capturedEnd(0);
        size_t index = idxMatch.captured(1).toUInt();
        if ( index >= SZ ) {
            if ( ok ) *ok = false;
            return [](T (&)[SZ]){};
        }
        std::function<void(T&)> func = getReadFunc(name, offset, is, ok, head[index], tail...);
        return [=](T (&v)[SZ]){ func(v[index]); };
    } else {
        if (ok) *ok = false;
        return [](T (&)[SZ]){};
    }
}

template <typename T, typename S, typename... Tail>
std::function<void(T&)> getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
                                    T &head, S T::* index, Tail... tail)
{
    QRegularExpressionMatch structIndex
            = QRegularExpression("\\.[a-zA-Z_][a-zA-Z0-9_]*").match(name, offset);
    if ( structIndex.hasMatch() ) {
        offset = structIndex.capturedEnd(0);
        std::function<void(S&)> func = getReadFunc(name, offset, is, ok, head.*index, tail...);
        return [=](T& v){ func(v.*index); };
    } else {
        if (ok) *ok = false;
        return [](T&){};
    }
}


template <typename T, typename... Tail>
void write(QString &name, std::ostream &os, std::vector<T> &head, Tail... tail);

template <typename T, size_t SZ, typename... Tail>
void write(QString &name, std::ostream &os, T (&head)[SZ], Tail... tail);

template <typename T, typename S, typename... Tail>
void write(QString &name, std::ostream &os, T &head, S T::* index, Tail... tail);

template <typename T>
void write(QString &name, std::ostream &os, T &head)
{
    os << name.toStdString() << " " << head << std::endl;
}

template <typename T, typename... Tail>
void write(QString &name, std::ostream &os, std::vector<T> &head, Tail... tail)
{
    int pos = name.indexOf('#');
    for ( size_t i = 0; i < head.size(); i++ ) {
        QString indexedName(name);
        indexedName.replace(pos, 1, QString::number(i));
        write(indexedName, os, head[i], tail...);
    }
}

template <typename T, size_t SZ, typename... Tail>
void write(QString &name, std::ostream &os, T (&head)[SZ], Tail... tail)
{
    int pos = name.indexOf('#');
    for ( size_t i = 0; i < SZ; i++ ) {
        QString indexedName(name);
        indexedName.replace(pos, 1, QString::number(i));
        write(indexedName, os, head[i], tail...);
    }
}

template <typename T, typename S, typename... Tail>
void write(QString &name, std::ostream &os, T &head, S T::* index, Tail... tail)
{
    write(name, os, head.*index, tail...);
}


template <int... Is>
struct index {};

template <int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template <int... Is>
struct gen_seq<0, Is...> : index<Is...> {};

} // namespace APFunc


template<typename T, typename... Tail>
class APInst : public AP
{
public:
    APInst(QString name, T *head, Tail... tail) : AP(name), head(head), tail(tail...) {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        if ( deprecated ) {
            // Replace existing indices in order
            QString target(deprecated->name);
            QRegularExpressionMatchIterator it = QRegularExpression("\\[(\\d+)\\]").globalMatch(rawName);
            int offset;
            while ( it.hasNext() && (offset = target.indexOf('#')) ) {
                target.replace(offset, 1, it.next().captured(1));
            }

            // Replace remaining indices with 0
            target.replace('#', '0');

            // Read to target
            return deprecated->readLater(target, is, ok);
        } else {
            return getReadFunc(rawName, is, ok, APFunc::gen_seq<sizeof...(Tail)>{});
        }
    }

    virtual void write(std::ostream &os)
    {
        if ( !deprecated )
            write(os, APFunc::gen_seq<sizeof...(Tail)>{});
    }

private:
    T *head;
    std::tuple<Tail...> tail;

    template <int... Is>
    std::function<void()> getReadFunc(QString &rawName, std::istream &is, bool *ok,
                                      APFunc::index<Is...>)
    {
        int offset = rawName.indexOf(QRegularExpression("[\\.\\[]"));
        std::function<void(T&)> func = APFunc::getReadFunc(rawName, offset, is, ok,
                                                          *head, std::get<Is>(tail)...);
        return [=](){ func(*head); };
    }

    template <int... Is>
    void write(std::ostream &os, APFunc::index<Is...>)
    {
        APFunc::write(name, os, *head, std::get<Is>(tail)...);
    }
};


template <typename T, typename... Tail>
inline std::unique_ptr<AP> const& addAP(QString name, T *head, Tail... tail) {
    params.push_back(std::unique_ptr<AP>(new APInst<T, Tail...>(name, head, tail...)));
    return params.back();
}

#endif // AP_H
