#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>

class dataSavingParams;
class BasePlotData;
class PerformanceMonitorData;
class SampleHoldData;
class TriggerData;
class SettlingData;
class GuiData;

class Configuration
{
public:
    static Configuration& instance();

    // Methods to access individual configuration structs
    dataSavingParams& dataSaving() const;
    BasePlotData& plot() const;
    PerformanceMonitorData& perfmon() const;
    SampleHoldData& sampleHold() const;
    TriggerData& trigger() const;
    SettlingData& settling() const;
    GuiData& gui() const;
    int& threadPrio() const;

    // Load/store entire configuration
    QStringList load(std::istream& input);
    void store(std::ostream& output) const;

    using ScriptLine = std::tuple<double, std::function<void(void)>>;
    // Load script file for timed reads. Returns valid entries as a ScriptLine vector, and a QStringList of error messages for invalid entries.
    std::tuple<std::vector<ScriptLine>, QStringList> loadScript(std::istream& input) const;

private:
    class Impl;
    Impl* impl;

    Configuration();
    ~Configuration();
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;
};

#endif // CONFIGURATION_H
