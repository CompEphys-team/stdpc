#ifndef CONDUCTANCEMANAGER_H
#define CONDUCTANCEMANAGER_H

#include <QObject>
#include <QMap>
#include "Conductance.h"
#include "ConductanceDlg.h"

class DCThread;

class ConductanceProxy {
public:
    /// Parameter access and manipulation functions:
    /// Return a reference to the indicated parameter set
    virtual ConductanceData& param(size_t i) const = 0;
    /// Return the number of parameter sets present
    virtual size_t size() = 0;
    /// Change the number of parameter sets
    virtual void resize(size_t) = 0;
    /// Remove a parameter set. Subsequent sets are expected to move up into the freed space.
    virtual void remove(size_t) = 0;

    /// Return an identifier for this conductance class (e.g. "CSyn"). Must not contain any whitespace or "/" characters.
    virtual QString conductanceClass() = 0;

    /// Return a human-readable identifier for this conductance class (e.g. "Chemical synapse") for use in the GUI
    virtual inline QString prettyName() { return conductanceClass(); }

    /// Instantiate all assignments (multiplexed as appropriate) from a parameter set
    virtual void instantiate(size_t conductanceID, size_t assignID, DCThread *,
                             std::vector<Conductance*> &preD,
                             std::vector<Conductance*> &inD,
                             std::vector<Conductance*> &postD) = 0;

    /// Create a new conductance dialog
    virtual ConductanceDlg *createDialog(size_t conductanceID, QWidget *parent=nullptr) = 0;
};


class ConductanceManager : public QObject
{
    Q_OBJECT
public:
    ConductanceManager() {}
    ~ConductanceManager();

    /// Initialise all active conductances for end use
    void init(DCThread *);

    /// Remove all conductances and parameter sets e.g. in preparation for parameter import
    void clear();

    /// Returns false if at least one registered parameter set is active (active conductance with at least one active assignment)
    bool empty() const;

    /// Returns a pointer to the (first, if multiplexed) assignment indicated by dex
    const double *conductance(const ChannelIndex &dex);

    QStringList getStatus() const;

    QPair<QVector<ChannelIndex>, QVector<const double *>> toSave() const;

    inline const std::vector<Conductance *> &preDigital() const { return preD; }
    inline const std::vector<Conductance *> &inDigital() const { return inD; }
    inline const std::vector<Conductance *> &postDigital() const { return postD; }

    static QMap<QString, ConductanceProxy*> &Register() { static QMap<QString, ConductanceProxy*> r; return r; } //!< All conductances
    static QMap<QString, ConductanceProxy*> &Synapses() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Synapse subset
    static QMap<QString, ConductanceProxy*> &Currents() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Current subset
    static QMap<QString, ConductanceProxy*> &Tools() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Tool subset
    static inline void RegisterSynapse(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Synapses()[proxy->conductanceClass()] = proxy; }
    static inline void RegisterCurrent(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Currents()[proxy->conductanceClass()] = proxy; }
    static inline void RegisterTool(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Tools()[proxy->conductanceClass()] = proxy; }

private:
    std::vector<Conductance *> preD, inD, postD;
};

#endif // CONDUCTANCEMANAGER_H
