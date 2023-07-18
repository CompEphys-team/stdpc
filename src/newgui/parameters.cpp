#include "parameters.h"

QString Parameters::read(const QStringList &path, const QVariant &value, int pathIndex)
{
    return std::get<0>(_read(path, value, pathIndex));
}

std::tuple<QString, int> Parameters::_read(const QStringList &path, const QVariant &value, int pathIndex)
{
    std::vector<std::tuple<QString, int>> retvals;
    _RegistryBase* reg = getRegistry();
    while ( reg ) {
        std::tuple<QString, int> retval = reg->read(this, path, pathIndex, value);
        if ( std::get<0>(retval).isEmpty() )
            return retval;
        retvals.emplace_back(retval);
        reg = reg->getBaseReg();
    }
    {
        int deepest = -1;
        QStringList causes;
        for ( auto retval : retvals ) {
            int depth = std::get<1>(retval);
            if ( depth > deepest ) {
                deepest = depth;
                causes = QStringList(std::get<0>(retval));
            } else if ( depth == deepest && !causes.contains(std::get<0>(retval)) ) {
                causes.append(std::get<0>(retval));
            }
        }
        return {causes.join(" --OR-- "), deepest};
    }
}

void Parameters::write(std::ostream &os, const QString &prefix) const
{
    std::vector<const _RegistryBase*> registries;
    const _RegistryBase* reg = getRegistry();
    while ( reg ) {
        registries.push_back(reg);
        reg = reg->getBaseReg();
    }
    for ( auto regIter = registries.rbegin(); regIter != registries.rend(); ++regIter ) {
        (*regIter)->write(this, os, prefix);
    }
}

void Parameters::_RegistryBase::setBaseReg(_RegistryBase *newBaseReg) {
    if ( baseReg == nullptr )
        baseReg = newBaseReg;
}
