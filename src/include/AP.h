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
void initAP();
std::istream &operator>>(std::istream &is, QString &str);
std::ostream &operator<<(std::ostream &os, const QString &str);

/**
 * @brief Add a parameter to the global registry. See @fn initAP() for examples.
 * @param name: A string version of the fully qualified name of the parameter
 * as you'd use it elsewhere in the code, replacing indices with a single '#'.
 * @param head: A pointer to the top-level variable that contains your parameter
 * @param tail...: Successive member pointers needed to reach the parameter
 * starting from @arg head, ignoring any vector or array indices.
 */
template <typename T, typename... Tail>
inline AP* addAP(QString name, T *head, Tail... tail);

/**
 * @brief Same as addAP(QString, T, Tail...), but adding to a local registry vec.
 */
template <typename T, typename... Tail>
inline AP* addAP(std::vector<std::unique_ptr<AP>> &vec, QString name, T *head, Tail... tail);

/**
 * @brief Add a deprecated AP, which delegates reading to @arg target and disables @fn write.
 * @param name: @see addAP. Indices in the target name are matched in order, defaulting to 0
 * if name has fewer indices than target->name.
 * @param nIgnoredEarlyIndices: The number of indices at the start of target->name that should
 * be set to 0 on insertion. Normal index matching resumes on the remaining indices.
 */
inline AP* addDeprecatedAP(QString name, AP* target, int nIgnoredEarlyIndices = 0);

/**
 * @brief deprecateChannelsTo returns a (temporary) list of APs deprecating the removed inChnp/outChnp
 * parameters to a specific DAQ.
 * @param prefix is the DAQ param name to deprecate to, e.g. "NIDAQp[#]". Should not include a "." at the end.
 * @return A list of APDeprec for temporary use while importing a script/protocol
 */
std::vector<std::unique_ptr<AP>> deprecateChannelsTo(QString prefix);

/**
 * @brief readProtocol reads a protocol file, taking into account config version
 * @param is - the protocol file
 * @param callback - pointer to a function that is called when a parameter can't be found. Receives the
 * raw parameter name as a QString argument. May return true to cancel reading.
 * @return true if everything went to plan; false when cancelled through callback or if protocol is malformed.
 */
bool readProtocol(std::istream &is, std::function<bool(QString)> *callback = nullptr);

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

    static AP* find(QString rawName, std::vector<std::unique_ptr<AP>> *vec = &params)
    {
        rawName.replace(QRegularExpression("\\[\\d+\\]"), "[#]");
        std::vector<std::unique_ptr<AP>>::iterator it;
        it = std::find_if(vec->begin(), vec->end(), [&](std::unique_ptr<AP> &a){return !rawName.compare(a->name());});
        if ( it == vec->end() )
            return nullptr;
        else
            return it->get();
    }

    inline QString const& name() { return _name; }

protected:
    AP(QString &name) : _name(name) {}
    QString _name;
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

template <typename T, typename Base, typename S, typename... Tail>
typename std::enable_if<(std::is_base_of<Base, T>::value), std::function<void(T&)>>::type
  getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
              T &head, S Base::* index, Tail... tail);

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


template <typename T, typename Base, typename S, typename... Tail>
typename std::enable_if<(std::is_base_of<Base, T>::value), std::function<void(T&)>>::type
  getReadFunc(QString &name, int offset, std::istream &is, bool *ok,
              T &head, S Base::* index, Tail... tail)
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

template <typename T, typename Base, typename S, typename... Tail>
typename std::enable_if<(std::is_base_of<Base, T>::value), void>::type
  write(QString &name, std::ostream &os, T &head, S Base::* index, Tail... tail);

template <typename T>
void write(QString &name, std::ostream &os, T &head)
{
    os << name.toStdString() << " " << head << std::endl;
}

template <typename T, typename... Tail>
void write(QString &name, std::ostream &os, std::vector<T> &head, Tail... tail)
{
    int pos = name.indexOf('#');
    for ( size_t i = head.size(); i > 0; i-- ) {
        QString indexedName(name);
        indexedName.replace(pos, 1, QString::number(i-1));
        write(indexedName, os, head[i-1], tail...);
    }
}

template <typename T, size_t SZ, typename... Tail>
void write(QString &name, std::ostream &os, T (&head)[SZ], Tail... tail)
{
    int pos = name.indexOf('#');
    for ( size_t i = SZ; i > 0; i-- ) {
        QString indexedName(name);
        indexedName.replace(pos, 1, QString::number(i-1));
        write(indexedName, os, head[i-1], tail...);
    }
}

template <typename T, typename S, typename... Tail>
void write(QString &name, std::ostream &os, T &head, S T::* index, Tail... tail)
{
    write(name, os, head.*index, tail...);
}

template <typename T, typename Base, typename S, typename... Tail>
typename std::enable_if<(std::is_base_of<Base, T>::value), void>::type
  write(QString &name, std::ostream &os, T &head, S Base::* index, Tail... tail)
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
        return getReadFunc(rawName, is, ok, APFunc::gen_seq<sizeof...(Tail)>{});
    }

    virtual void write(std::ostream &os)
    {
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
        APFunc::write(_name, os, *head, std::get<Is>(tail)...);
    }
};


class APDeprec : public AP
{
public:
    APDeprec(QString name, AP *target, int nIgnoredEarlyIndices=0) : AP(name), target(target), nIgnore(nIgnoredEarlyIndices) {}
    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        // Replace existing indices in order
        QString tName(target->name());
        QRegularExpressionMatchIterator it = QRegularExpression("\\[(\\d+)\\]").globalMatch(rawName);
        int offset, i = 0;
        while ( it.hasNext() && (offset = tName.indexOf('#')) > 0 ) {
            if ( i++ < nIgnore )
                tName.replace(offset, 1, '0');
            else
                tName.replace(offset, 1, it.next().captured(1));
        }

        // Replace remaining indices with 0
        tName.replace('#', '0');

        // Read to target
        return target->readLater(tName, is, ok);
    }

    virtual void write(std::ostream &) {}

private:
    AP *target;
    int nIgnore;
};


template <typename T, typename... Tail>
inline AP* addAP(std::vector<std::unique_ptr<AP>> &vec, QString name, T *head, Tail... tail) {
    vec.push_back(std::unique_ptr<AP>(new APInst<T, Tail...>(name, head, tail...)));
    return vec.back().get();
}

template <typename T, typename... Tail>
inline AP* addAP(QString name, T *head, Tail... tail) {
    return addAP(params, name, head, tail...);
}

inline AP* addDeprecatedAP(QString name, AP *target, int nIgnore) {
    params.push_back(std::unique_ptr<AP>(new APDeprec(name, target, nIgnore)));
    return params.back().get();
}

#endif // AP_H
