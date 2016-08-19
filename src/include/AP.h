#ifndef AP_H
#define AP_H

#include <iostream>
#include <QString>
#include <QRegularExpression>
#include <vector>
#include <functional>
#include <memory>

/**
 * @brief Adjustable/protocol parameter interface
 *
 * AP is the abstract base class with utility functions and a common interface.
 * Child classes exist to allow adding parameters at various positions in the
 * parameter hierarchy. They are named to reflect the hierarchy, from leaf
 * to root; e.g. AP_struct_vector is for a member (AP) of a struct which is
 * in turn stored in a globally accessible vector. (As a mnemonic, think of
 * the underscore as "in a", e.g. "AP in a struct in a vector".)
 *
 * For convenience, there is an overloaded addAP(...) function for each case,
 * see below, which is called with each 'stepping stone' in order from
 * root to leaf of the hierarchy. In the above example, the call would be
 * addAP("structname[#].param", &structname, &structtype::param), where
 * structname is a vector<structtype>. For each indexed stepping stone,
 * i.e. each vector and array, there must be a '#' in the name string.
 **/
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
     * appropriate index. Additional occurrences of '#' are replaced with the
     * rightmost index and are therefore highly discouraged.
     */
    virtual void write(std::ostream &os) = 0;

    QString name;

protected:
    AP(QString name) : name(name) {}

    QRegularExpressionMatch matchName(QString &rawName);
    int getIndex(QRegularExpressionMatch &match, int i);
    int getArrayIndex(QRegularExpressionMatch &match, int i, int arraysize);

    template <typename T>
    int getIndexAndResize(QRegularExpressionMatch &match, int i, std::vector<T> *vec)
    {
        int idx = getIndex(match, i);
        if ( idx < 0 )
            return idx;
        if ( static_cast<typename std::vector<T>::size_type>(idx) >= vec->size() )
            vec->resize(idx+1);
        return idx;
    }
};

template <typename T>                       class AP_direct;
template <typename T>                       class AP_vector;
template <typename T, class S>              class AP_struct_vector;
template <typename T, class subS, class S>  class AP_struct_struct_vector;
template <typename T, class S>              class AP_vector_struct_vector;
template <typename T, int N, class S>       class AP_array_struct_vector;
template <typename T, class subS, class S>  class AP_struct_vector_struct_vector;


extern std::vector<std::unique_ptr<AP>> params;

void initAP();

template <typename T>
inline void addAP(QString name, T * var) {
    params.push_back(std::unique_ptr<AP>(new AP_direct<T>(name, var)));
}

template <typename T>
inline void addAP(QString name, std::vector<T> * vec) {
    params.push_back(std::unique_ptr<AP>(new AP_vector<T>(name, vec)));
}

template <typename T, class S>
inline void addAP(QString name, std::vector<S> * vec, T S::* m) {
    params.push_back(std::unique_ptr<AP>(new AP_struct_vector<T,S>(name, vec, m)));
}

template <typename T, class subS, class S>
inline void addAP(QString name, std::vector<S> * vec, subS S::* sub, T subS::* m) {
    params.push_back(std::unique_ptr<AP>(new AP_struct_struct_vector<T,subS,S>(name, vec, sub, m)));
}

template <typename T, class S>
inline void addAP(QString name, std::vector<S> *vec, std::vector<T> S::* sub) {
    params.push_back(std::unique_ptr<AP>(new AP_vector_struct_vector<T,S>(name, vec, sub)));
}

template <typename T, int N, class S>
inline void addAP(QString name, std::vector<S> *vec, T (S::* arr)[N]) {
    params.push_back(std::unique_ptr<AP>(new AP_array_struct_vector<T,N,S>(name, vec, arr)));
}

template <typename T, class subS, class S>
inline void addAP(QString name, std::vector<S> * vec, std::vector<subS> S::* sub, T subS::* m) {
    params.push_back(std::unique_ptr<AP>(new AP_struct_vector_struct_vector<T,subS,S>(name, vec, sub, m)));
}

std::istream &operator>>(std::istream &is, QString &str);
std::ostream &operator<<(std::ostream &os, const QString &str);


// For top-level members of a single global struct, and for standalone global primitives
// E.g.: `dataSavingPs.enabled`
template <typename T>
class AP_direct : public AP
{
public:
    AP_direct(QString name, T *var) :
        AP(name),
        var(var)
    {}

    virtual std::function<void()> readLater(QString &, std::istream &is, bool *ok=nullptr)
    {
        bool good = is.good();
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){*var = val;};
    }

    virtual void write(std::ostream &os)
    {
        os << name.toStdString() << " " << *var << std::endl;
    }

    T *var;
};

// For elements T in a vector<T> which stands alone or is nested in a single global struct
// E.g.: `SGp.SpikeT[#]`
template <typename T>
class AP_vector : public AP
{
public:
    AP_vector(QString name, std::vector<T> * vec) :
        AP(name),
        vec(vec)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int idx = getIndexAndResize(match, 1, vec);
        bool good = is.good() && idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){(*vec)[idx] = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0;
        for ( T const &val : *vec ) {
            QString n = name;
            n.replace('#', QString::number(i++));
            os << n.toStdString() << " " << val << std::endl;
        }
    }

    std::vector<T> *vec;
};

// For top-level members T of structs S in a global vector
// E.g.: `outChnData[#].active`
template <typename T, class S>
class AP_struct_vector : public AP
{
public:
    AP_struct_vector(QString name, std::vector<S> *vec, T S::* m) :
        AP(name),
        vec(vec),
        m(m)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int idx = getIndexAndResize(match, 1, vec);
        bool good = is.good() && idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){(*vec)[idx].*m = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0;
        for ( S const &s : *vec ) {
            QString n = name;
            n.replace('#', QString::number(i++));
            os << n.toStdString() << " " << s.*m << std::endl;
        }
    }

    std::vector<S> *vec;
    T S::* m;
};

// For members T in a struct subS nested within the top-level struct S in a global vector
// E.g. `CSynp[#].ST.Table`
template <typename T, class subS, class S>
class AP_struct_struct_vector : public AP
{
public:
    AP_struct_struct_vector(QString name, std::vector<S> *vec, subS S::* sub, T subS::* m) :
        AP(name),
        vec(vec),
        sub(sub),
        m(m)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int idx = getIndexAndResize(match, 1, vec);
        bool good = is.good() && idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){(*vec)[idx].*sub.*m = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0;
        for ( S const &s : *vec ) {
            QString n = name;
            n.replace('#', QString::number(i++));
            os << n.toStdString() << " " << s.*sub.*m << std::endl;
        }
    }

    std::vector<S> *vec;
    subS S::* sub;
    T subS::* m;
};

// For elements T in a vector<T> nested within the top-level struct S in a global vector
// E.g. (hypothetically, assuming several SGs): `SGp[#].SpikeT[#]`
template <typename T, class S>
class AP_vector_struct_vector : public AP
{
public:
    AP_vector_struct_vector(QString name, std::vector<S> *vec, std::vector<T> S::* sub) :
        AP(name),
        vec(vec),
        sub(sub)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int vec_idx = getIndexAndResize(match, 1, vec);
        int sub_idx = getIndexAndResize(match, 2, &((*vec)[vec_idx].*sub));
        bool good = is.good() && vec_idx >= 0 && sub_idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){((*vec)[vec_idx].*sub)[sub_idx] = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0, first = name.indexOf('#');
        for ( S const &s : *vec ) {
            QString n = name;
            n.replace(first, 1, QString::number(i++));
            int j = 0;
            for ( T const &val : s.*sub ) {
                QString nj = n;
                nj.replace('#', QString::number(j++));
                os << nj.toStdString() << " " << val << std::endl;
            }
        }
    }

    std::vector<S> *vec;
    std::vector<T> S::* sub;
};

// For elements T in a C-style array T[N] nested within the top-level struct S in a global vector
// E.g. `Graphp[#].active[#]`
template <typename T, int N, class S>
class AP_array_struct_vector : public AP
{
public:
    AP_array_struct_vector(QString name, std::vector<S> *vec, T (S::* arr)[N]) :
        AP(name),
        vec(vec),
        arr(arr)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int vec_idx = getIndexAndResize(match, 1, vec);
        int arr_idx = getArrayIndex(match, 2, N);
        bool good = is.good() && vec_idx >= 0 && arr_idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){((*vec)[vec_idx].*arr)[arr_idx] = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0, first = name.indexOf('#');
        for ( S const &s : *vec ) {
            QString n = name;
            n.replace(first, 1, QString::number(i++));
            for ( int j = 0; j < N; j++ ) {
                QString nj = n;
                nj.replace('#', QString::number(j));
                os << nj.toStdString() << " " << (s.*arr)[j] << std::endl;
            }
        }
    }

    std::vector<S> *vec;
    T (S::* arr)[N];
};

// For members T in a struct vector<subS> nested within the top-level struct S in a global vector
// E.g. `mhHHp[#].assign[#].active`
template <typename T, class subS, class S>
class AP_struct_vector_struct_vector : public AP
{
public:
    AP_struct_vector_struct_vector(QString name, std::vector<S> *vec, std::vector<subS> S::* sub, T subS::* m) :
        AP(name),
        vec(vec),
        sub(sub),
        m(m)
    {}

    virtual std::function<void()> readLater(QString &rawName, std::istream &is, bool *ok=nullptr)
    {
        QRegularExpressionMatch match = matchName(rawName);
        int vec_idx = getIndexAndResize(match, 1, vec);
        int sub_idx = getIndexAndResize(match, 2, &((*vec)[vec_idx].*sub));
        bool good = is.good() && vec_idx >= 0 && sub_idx >= 0;
        if ( ok )
            *ok = good;
        if ( !good )
            return []{};
        T val;
        is >> val;
        return [=](){((*vec)[vec_idx].*sub)[sub_idx].*m = val;};
    }

    virtual void write(std::ostream &os)
    {
        int i = 0, first = name.indexOf('#');
        for ( S const &s : *vec ) {
            QString n = name;
            n.replace(first, 1, QString::number(i++));
            int j = 0;
            for ( subS const &ss : s.*sub ) {
                QString nj = n;
                nj.replace('#', QString::number(j++));
                os << nj.toStdString() << " " << ss.*m << std::endl;
            }
        }
    }

    std::vector<S> *vec;
    std::vector<subS> S::* sub;
    T subS::* m;
};

#endif // AP_H
