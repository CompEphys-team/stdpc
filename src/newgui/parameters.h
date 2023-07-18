#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QVariant>
#include <stack>
#include "Util.h"

/**
 * Parameters is the base class for all (adjustable) parameter structures.
 *
 * It includes facilities to register members for reading and writing protocol/script files.
 * Viable member types for registration include integral, floating point, QString and QString-derived, Parameters-derived, and std::vector<Parameters-derived>.
 *
 * For full functionality, inherit from Parameters in conjunction with two macros, PARAMETERS_DECL(derivedclass, baseclass) and PARAMETERS_IMPL(derivedclass),
 * the former of which should be followed by a brace-terminated class declaration, including member declarations, and
 * the latter of which should be followed by a brace-enclosed function body calling PARAMETER(derivedclass, paramname),
 * or, if member names in code and in protocol disagree, function calls of the form `add("protocol-name", &derivedclass::membername);`.
 * Parameters declared within enclosing scope must use the PARAMETERS_IMPL_INSCOPE(derivedclass, scope) instead of PARAMETERS_IMPL.
 *
 * Note that multi-level inheritance is possible (with registration, i.e. macro usage, optional at intermediate levels),
 * but care should be taken not to register the same member twice.
 *
 * Example:
 * // in a.h; declares a `class A : public Parameters`
 * PARAMETERS_DECL(A, Parameters)
 *     int myMember;
 * };
 *
 * // in a.cpp
 * PARAMETERS_IMPL(A)
 *     PARAMETER(A, myMember)
 *     // or: add("nameForMyMemberInProtocolFiles", &A::myMember);
 * }
 */
class Parameters {
public:
    QString read(const QStringList& path, const QVariant& value, int pathIndex=0);
    void write(std::ostream& os, const QString& prefix) const;

    std::tuple<QString, int> _read(const QStringList& path, const QVariant& value, int pathIndex);

protected:
    Parameters() = default;
    virtual ~Parameters() = default;

    class _RegistryBase
    {
    public:
        virtual std::tuple<QString, int> read(Parameters* self, const QStringList& path, int index, const QVariant& value) const = 0;
        virtual void write(const Parameters* self, std::ostream& os, const QString& prefix) const = 0;

        inline _RegistryBase *getBaseReg() const { return baseReg; }
        void setBaseReg(_RegistryBase *newBaseReg);

    protected:
        ~_RegistryBase() = default;
    private:
        _RegistryBase* baseReg = nullptr;
    };

    template <typename Params>
    class RegistryBase : public _RegistryBase
    {
    protected:
        RegistryBase() {
            static_assert(std::is_base_of_v<Parameters, Params>, "Use: Parameters::Registry<DerivedFromParameters>. See also the documentation for Parameters.");
        }

        // Note: The Owner template parameter is technically unnecessary in all overloads (using Params directly would work),
        // but is included to squelch IDE errors raised by clang when registering base class items.
        // Simple types
        template <typename T, typename Owner>
        typename std::enable_if<(std::is_arithmetic_v<T> || std::is_base_of_v<QString, T>) && std::is_base_of_v<Owner, Params>, void>::type
        add(const QString& parameterName, T Owner::* paramPtr)
        {
            readers[parameterName] = [=](Params* self, const QStringList& path, int index, const QVariant& value) -> std::tuple<QString, int> {
                if ( index < path.size() )
                    return {QString("Unexpected child %1 of %2 at position %3 in %4.").arg(path[index], parameterName, QString::number(index), path.join('.')), index};
                self->*paramPtr = value.value<T>();
                return {QString(), index};
            };
            writers[parameterName] = [=](const Params* self, std::ostream& os, const QString& prefix){
                os << prefix << parameterName << ' ' << self->*paramPtr << std::endl;
            };
        }

        // Parameters
        template<typename T, typename Owner>
        typename std::enable_if<std::is_base_of_v<Parameters, T> && std::is_base_of_v<Owner, Params>, void>::type
        add(const QString& parameterName, T Owner::* structPtr)
        {
            readers[parameterName] = [=](Params* self, const QStringList& path, int index, const QVariant& value) -> std::tuple<QString, int> {
                if ( index >= path.size() )
                    return {QString("Expected child of %2 at position %3 in %4.").arg(parameterName, QString::number(index), path.join('.')), index};
                return (self->*structPtr)._read(path, value, index);
            };
            writers[parameterName] = [=](const Params* self, std::ostream& os, const QString& prefix){
                (self->*structPtr).write(os, QString("%1%2.").arg(prefix, parameterName));
            };
        }

        // std::vector<Parameters>
        template<typename T, typename Owner>
        typename std::enable_if<std::is_base_of_v<Parameters, T> && std::is_base_of_v<Owner, Params>, void>::type
        add(const QString& parameterName, std::vector<T> Owner::* vectorPtr)
        {
            readers[parameterName] = [=](Params* self, const QStringList& path, int index, const QVariant& value) -> std::tuple<QString, int> {
                std::vector<T> &vec = self->*vectorPtr;
                if ( index >= path.size() )
                    return {QString("Missing index to %2 at position %3 in %4.").arg(parameterName, QString::number(index), path.join('.')), index};
                bool ok;
                int vecIdx = path[index].toInt(&ok);
                if ( !ok || vecIdx < 0 )
                    return {QString("Index %1 to %2 at position %3 in %4 is invalid.").arg(path[index], parameterName, QString::number(index), path.join('.')), index};
                if ( ++index >= path.size() )
                    return {QString("Expected child of %2[%1] at position %3 in %4.").arg(QString::number(vecIdx), parameterName, QString::number(index), path.join('.')), index};
                if ( vecIdx >= vec.size() )
                    vec.resize(vecIdx + 1);
                return vec[vecIdx]._read(path, value, index);
            };
            writers[parameterName] = [=](const Params* self, std::ostream& os, const QString& prefix){
                const std::vector<T> &vec = self->*vectorPtr;
                QString extendedPrefix = QString("%1%2[%3].").arg(prefix, parameterName);
                for ( int i = int(vec.size()-1); i >= 0; --i ) {
                    vec[i].write(os, extendedPrefix.arg(i));
                }
            };
        }

    private:
        QMap<QString, std::function<std::tuple<QString, int>(Params *self, const QStringList&, int, const QVariant&)>> readers;
        QMap<QString, std::function<void(const Params *self, std::ostream&, const QString&)>> writers;

        std::tuple<QString, int> read(Parameters* self, const QStringList& path, int index, const QVariant& value) const override
        {
            Params* trueSelf = static_cast<Params*>(self);
            if ( index >= path.size() )
                return {QString("Expected child at position %3 in %4.").arg(QString::number(index), path.join(".")), index};
            QString parameterName = path[index];
            if ( !readers.contains(parameterName) )
                return {QString("Unregistered parameter %1 at position %3 in %4").arg(parameterName, QString::number(index), path.join(".")), index};
            return readers.value(parameterName)(trueSelf, path, index+1, value);
        }

        void write(const Parameters* self, std::ostream& os, const QString& prefix) const override
        {
            const Params* trueSelf = static_cast<const Params*>(self);
            for ( auto fn : qAsConst(writers) )
                fn(trueSelf, os, prefix);
        }
    };

    virtual inline _RegistryBase* getRegistry() const { return nullptr; }
};

#define PARAMETERS_DECL(classname, base) \
class classname : public base \
{ \
    using super = base; \
    class Registry : public RegistryBase<classname> { \
    public: \
        Registry(); \
    }; \
    static Registry REG; \
protected: \
    inline _RegistryBase* getRegistry() const override { return &REG; } \
public: \
    classname();

#define PARAMETERS_IMPL(classname) \
classname::classname() { REG.setBaseReg(super::getRegistry()); } \
classname::Registry classname::REG; \
classname::Registry::Registry() \
// Follow with { a brace-enclosed function body }.

#define PARAMETERS_IMPL_INSCOPE(classname, scope) \
scope::classname::classname() { REG.setBaseReg(super::getRegistry()); } \
scope::classname::Registry scope::classname::REG; \
scope::classname::Registry::Registry() \
// Follow with { a brace-enclosed function body }.

#define PARAMETER(classname, varname) add(#varname, &classname::varname);

#endif // PARAMETERS_H
