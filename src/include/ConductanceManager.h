/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
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

#ifndef CONDUCTANCEMANAGER_H
#define CONDUCTANCEMANAGER_H

#include <QObject>
#include <QMap>
#include "Conductance.h"
#include "ConductanceDlg.h"

class DCThread;
class ConductanceManager;

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
    virtual void instantiate(size_t conductanceID, size_t assignID, DCThread *, ConductanceManager *manager) = 0;

    /// Create a new conductance dialog
    virtual ConductanceDlg *createDialog(size_t conductanceID, QWidget *parent=nullptr) = 0;

    virtual ~ConductanceProxy() = default;
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

    static QMap<QString, ConductanceProxy*> &Register() { static QMap<QString, ConductanceProxy*> r; return r; } //!< All conductances
    static QMap<QString, ConductanceProxy*> &Synapses() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Synapse subset
    static QMap<QString, ConductanceProxy*> &Currents() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Current subset
    static QMap<QString, ConductanceProxy*> &Tools() { static QMap<QString, ConductanceProxy*> r; return r; } //!< Tool subset
    static inline void RegisterSynapse(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Synapses()[proxy->conductanceClass()] = proxy; }
    static inline void RegisterCurrent(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Currents()[proxy->conductanceClass()] = proxy; }
    static inline void RegisterTool(ConductanceProxy *proxy) { Register()[proxy->conductanceClass()] = Tools()[proxy->conductanceClass()] = proxy; }

    std::vector<Conductance *> preD, inD, postD;
    std::vector<Conductance *> transform_devIn, transform_devOut, transform_modIn, transform_modOut;
};

#endif // CONDUCTANCEMANAGER_H
