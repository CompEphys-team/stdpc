#include "configuration.h"
#include "moduleregistry.h"
#include "parameters.h"
#include "ObjectDataTypes.h"

PARAMETERS_DECL_INSCOPE
class Configuration::Impl
{
public:
    dataSavingParams dataSaving;
    BasePlotData plot;
    PerformanceMonitorData perfmon;
    SampleHoldData sampleHold;
    TriggerData trigger;
    SettlingData settling;
    GuiData gui;
    int threadPrio;
};

Configuration &Configuration::instance() {
    static Configuration config;
    return config;
}

Configuration::Configuration() :
    impl{new Impl()}
{
}

Configuration::~Configuration()
{
    delete impl;
}

dataSavingParams &Configuration::dataSaving() const { return impl->dataSaving; }
BasePlotData &Configuration::plot() const { return impl->plot; }
PerformanceMonitorData &Configuration::perfmon() const { return impl->perfmon; }
SampleHoldData &Configuration::sampleHold() const { return impl->sampleHold; }
TriggerData &Configuration::trigger() const { return impl->trigger; }
SettlingData &Configuration::settling() const { return impl->settling; }
GuiData &Configuration::gui() const { return impl->gui; }
int &Configuration::threadPrio() const { return impl->threadPrio; }

QStringList Configuration::load(std::istream &input)
{
    // Read configuration file, setting values in Impl, instantiating modules and adding them to the Registry.
    std::string line;
    QString qline;
    QRegExp whitespace(R"(\s+)");
    QRegExp separators(R"([\.\[\]])");
    while ( std::getline(input, line) ) {
        qline = QString::fromStdString(line).trimmed();
        if ( qline.isEmpty() || qline.startsWith('#') )
            continue;
        int pathEnd = qline.indexOf(whitespace);
        QString value = qline.mid(pathEnd).trimmed();
        QStringList path = qline.left(pathEnd).split(separators, Qt::SkipEmptyParts);
        
        QModelIndex index = ModuleRegistry::instance().addModule(path[0]);
        if ( index.isValid() )
            continue;
        
    }
    return {};
}
