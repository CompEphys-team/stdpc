# StdpC project file #

# To enable DigiData support, uncoment the following line (requires mingw)
#CONFIG += digidata

# To enable NI support, uncomment the following line (requires NIDAQmx 15.5.1 or newer)
# If NIDAQmx is detected, this has no effect.
#CONFIG += nidaqmx

# To disable NI support (despite NIDAQmx being present in the system), uncomment the following line:
#CONFIG += nonidaqmx

TARGET = StdpC
QT+= widgets printsupport
INCLUDEPATH += \
    $$PWD/src/include \
    $$PWD/src/lib
TEMPLATE = app
CONFIG += qt \
    thread \
    debug_and_release \
    c++11

CONFIG(release, debug|release): DEFINES += NDEBUG

mingw {
    QMAKE_CXXFLAGS_RELEASE += -O3 -flto
    QMAKE_LFLAGS_RELEASE += -O3 -flto

    static {
        QMAKE_LFLAGS += -static
    }
}
msvc {
    QMAKE_CXXFLAGS_RELEASE += /Ox
}

# A directory to place intermediary build files.
RCC_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
#UI_DIR = $$PWD/src/gui/ui

# the sources
FORMS += $$PWD/src/gui/MainWin.ui \
    $$PWD/src/gui/ChemSynDlg.ui \
    $$PWD/src/gui/GapJunctionDlg.ui \
    $$PWD/src/gui/STDPDlg.ui \
    $$PWD/src/gui/ODESTDPDlg.ui \
    $$PWD/src/gui/InputChannelDlg.ui \
    $$PWD/src/gui/GraphDlg.ui \
    $$PWD/src/gui/HHDlg.ui \
    $$PWD/src/gui/AlphaBetaHHDlg.ui \
    $$PWD/src/gui/OutputChannelDlg.ui \
    $$PWD/src/gui/SimulDAQDlg.ui \
    $$PWD/src/gui/ElectrodeCompDlg.ui \
    $$PWD/src/gui/AbSynDlg.ui \
    $$PWD/src/gui/DataSavingDlg.ui \
    $$PWD/src/gui/DestexheSynDlg.ui \
    $$PWD/src/gui/HHModelDlg.ui \
    $$PWD/src/gui/ComponentWidget.ui \
    $$PWD/src/gui/ComponentFactoryWidget.ui \
    $$PWD/src/gui/DaqWidget.ui \
    $$PWD/src/gui/DaqFactoryWidget.ui \
    $$PWD/src/gui/SpikeGenDlg.ui \
    $$PWD/src/gui/PerformanceMonitor.ui \
    $$PWD/src/gui/GraphWidget.ui \
    $$PWD/src/gui/SynapticNoiseDlg.ui \
    $$PWD/src/gui/TriggerDlg.ui \
    $$PWD/src/gui/VoltageClampDlg.ui \
    $$PWD/src/gui/VStepsDlg.ui \
    $$PWD/src/gui/WireDlg.ui \
    src/gui/channelsinusoiddlg.ui \
    src/gui/pipedaqdlg.ui \
    src/gui/treewidgetitem_datasources.ui \
    src/gui/treewidgetitem_models.ui \
    src/gui/InputConductanceDlg.ui

HEADERS += \
    $$PWD/src/include/Mainwin.h \
    $$PWD/src/include/ChemSynDlg.h \
    $$PWD/src/include/GapJunctionDlg.h \
    $$PWD/src/include/HHDlg.h \
    $$PWD/src/include/AlphaBetaHHDlg.h \
    $$PWD/src/include/DCThread.h \
    $$PWD/src/include/STDPDlg.h \
    $$PWD/src/include/ODESTDPDlg.h \
    $$PWD/src/include/LUtables.h \
    $$PWD/src/include/InputChannelDlg.h \
    $$PWD/src/include/OutputChannelDlg.h \
    $$PWD/src/include/Channels.h \
    $$PWD/src/include/SimulDAQDlg.h \
    $$PWD/src/include/ChemSyn.h \
    $$PWD/src/include/GapJunction.h \
    $$PWD/src/include/HH.h \
    $$PWD/src/include/AbHH.h \
    $$PWD/src/include/Daq.h \
    $$PWD/src/include/SimulDAQ.h \
    $$PWD/src/include/SpkGen.h \
    $$PWD/src/include/GraphDlg.h \
    $$PWD/src/include/DAQDlg.h \
    $$PWD/src/include/AbSynDlg.h \
    $$PWD/src/include/ElectrodeCompDlg.h \
    $$PWD/src/include/AbSyn.h \
    $$PWD/src/include/AECChannel.h \
    $$PWD/src/include/KernelCalculator.h \
    $$PWD/src/include/Calibrator.h \
    $$PWD/src/include/DataSaver.h \
    $$PWD/src/include/DataSavingDlg.h \
    $$PWD/src/include/ObjectDataTypes.h \
    $$PWD/src/include/Global.h \
    $$PWD/src/include/DestexheSynDlg.h \
    $$PWD/src/include/DestexheSyn.h \
    $$PWD/src/include/AP.h \
    $$PWD/src/include/ChannelListModel.h \
    $$PWD/src/include/HHNeuron.h \
    $$PWD/src/include/HHModelDlg.h \
    $$PWD/src/include/ChannelIndex.h \
    $$PWD/src/include/AssignmentWidget.h \
    $$PWD/src/include/ComponentTable.h \
    $$PWD/src/include/ComponentWidget.h \
    $$PWD/src/include/Clock.h \
    $$PWD/src/include/DaqTable.h \
    $$PWD/src/include/DaqWidget.h \
    $$PWD/src/include/DeviceManager.h \
    $$PWD/src/include/Component.h \
    $$PWD/src/include/WideComboBox.h \
    $$PWD/src/include/DaqOpts.h \
    $$PWD/src/include/DaqOptsBase.h \
    $$PWD/src/include/QCustomPlot.h \
    $$PWD/src/include/CircularFifo.h \
    $$PWD/src/include/SpikeGenDlg.h \
    $$PWD/src/include/ChannelBufferHelper.h \
    $$PWD/src/include/AssignmentCell.h \
    $$PWD/src/include/ModelManager.h \
    $$PWD/src/include/Model.h \
    $$PWD/src/include/ModelOpts.h \
    $$PWD/src/include/ModelDlg.h \
    $$PWD/src/include/Util.h \
    $$PWD/src/include/PerformanceMonitor.h \
    $$PWD/src/include/ConductanceManager.h \
    $$PWD/src/include/Conductance.h \
    $$PWD/src/include/Synapse.h \
    $$PWD/src/include/ConductanceDlg.h \
    $$PWD/src/include/IonicCurrent.h \
    $$PWD/src/include/GraphWidget.h \
    $$PWD/src/include/SynapticNoise.h \
    $$PWD/src/include/SynapticNoiseDlg.h \
    $$PWD/src/include/TriggerDlg.h \
    $$PWD/src/include/VoltageClampDlg.h \
    $$PWD/src/include/VoltageClamp.h \
    $$PWD/src/include/VStepsDlg.h \
    $$PWD/src/include/VSteps.h \
    $$PWD/src/include/Wire.h \
    $$PWD/src/include/WireDlg.h \
    src/include/PipeDAQ.h \
    src/include/channeltransform.h \
    src/include/channelsinusoid.h \
    src/include/channelsinusoiddlg.h \
    src/include/pipedaqdlg.h \
    src/include/treewidgetitem_datasources.h \
    src/include/treewidgetitem_models.h \
    src/include/InputConductance.h \
    src/include/InputConductanceDlg.h

SOURCES += $$PWD/src/core/Main.cpp \
    $$PWD/src/gui/MainWin.cpp \
    $$PWD/src/gui/ChemSynDlg.cpp \
    $$PWD/src/gui/GapJunctionDlg.cpp \
    $$PWD/src/gui/HHDlg.cpp \
    $$PWD/src/gui/AlphaBetaHHDlg.cpp \
    $$PWD/src/core/DCThread.cpp \
    $$PWD/src/gui/STDPDlg.cpp \
    $$PWD/src/gui/ODESTDPDlg.cpp \
    $$PWD/src/core/LUtables.cpp \
    $$PWD/src/gui/InputChannelDlg.cpp \
    $$PWD/src/core/Global.cpp \
    $$PWD/src/core/Global_func.cpp \
    $$PWD/src/gui/OutputChannelDlg.cpp \
    $$PWD/src/core/Channels.cpp \
    $$PWD/src/gui/SimulDAQDlg.cpp \
    $$PWD/src/models/ChemSyn.cpp \
    $$PWD/src/models/GapJunction.cpp \
    $$PWD/src/models/HH.cpp \
    $$PWD/src/models/AbHH.cpp \
    $$PWD/src/drivers/Daq.cpp \
    $$PWD/src/drivers/SimulDAQ.cpp \
    $$PWD/src/models/SpkGen.cpp \
    $$PWD/src/gui/GraphDlg.cpp \
    $$PWD/src/gui/DAQDlg.cpp \
    $$PWD/src/gui/AbSynDlg.cpp \
    $$PWD/src/gui/ElectrodeCompDlg.cpp \
    $$PWD/src/models/AbSyn.cpp \
    $$PWD/src/core/AECChannel.cpp \
    $$PWD/src/core/KernelCalculator.cpp \
    $$PWD/src/core/Calibrator.cpp \
    $$PWD/src/core/DataSaver.cpp \
    $$PWD/src/gui/DataSavingDlg.cpp \
    $$PWD/src/gui/DestexheSynDlg.cpp \
    $$PWD/src/models/DestexheSyn.cpp \
    $$PWD/src/core/AP.cpp \
    $$PWD/src/gui/ChannelListModel.cpp \
    $$PWD/src/models/HHNeuron.cpp \
    $$PWD/src/gui/HHModelDlg.cpp \
    $$PWD/src/core/ChannelIndex.cpp \
    $$PWD/src/gui/ComponentTable.cpp \
    $$PWD/src/gui/ComponentWidget.cpp \
    $$PWD/src/drivers/Clock.cpp \
    $$PWD/src/gui/DaqTable.cpp \
    $$PWD/src/gui/DaqWidget.cpp \
    $$PWD/src/core/DeviceManager.cpp \
    $$PWD/src/gui/WideComboBox.cpp \
    $$PWD/src/gui/QCustomPlot.cpp \
    $$PWD/src/gui/SpikeGenDlg.cpp \
    $$PWD/src/core/ChannelBufferHelper.cpp \
    $$PWD/src/core/ModelManager.cpp \
    $$PWD/src/models/Model.cpp \
    $$PWD/src/gui/ModelOpts.cpp \
    $$PWD/src/gui/DaqOpts.cpp \
    $$PWD/src/core/Util.cpp \
    $$PWD/src/gui/PerformanceMonitor.cpp \
    $$PWD/src/core/ConductanceManager.cpp \
    $$PWD/src/models/Synapse.cpp \
    $$PWD/src/models/IonicCurrent.cpp \
    $$PWD/src/gui/Component.cpp \
    $$PWD/src/gui/GraphWidget.cpp \
    $$PWD/src/models/SynapticNoise.cpp \
    $$PWD/src/gui/SynapticNoiseDlg.cpp \
    $$PWD/src/gui/TriggerDlg.cpp \
    $$PWD/src/gui/VoltageClampDlg.cpp \
    $$PWD/src/models/VoltageClamp.cpp \
    $$PWD/src/gui/VStepsDlg.cpp \
    $$PWD/src/models/VSteps.cpp \
    $$PWD/src/models/Wire.cpp \
    src/drivers/PipeDAQ.cpp \
    src/gui/WireDlg.cpp \
    src/gui/channelsinusoiddlg.cpp \
    src/gui/pipedaqdlg.cpp \
    src/gui/treewidgetitem_datasources.cpp \
    src/gui/treewidgetitem_models.cpp \
    src/models/InputConductance.cpp \
    src/gui/InputConductanceDlg.cpp \
    src/models/channelsinusoid.cpp \
    src/models/channeltransform.cpp

digidata {
    mingw {
        LIBS += $$PWD/staticlib/pt_ioctl_tn.a
        FORMS += $$PWD/src/gui/DigiDataDlg.ui
        HEADERS += $$PWD/src/include/DigiData.h \
            $$PWD/src/include/DigiDataDlg.h \
            $$PWD/src/include/Pt_ioctl_tn.h \
            $$PWD/src/include/PortTalkX_IOCTL.h
        SOURCES += $$PWD/src/drivers/DigiData.cpp \
            $$PWD/src/gui/DigiDataDlg.cpp
        DEFINES += DIGIDATA_PT
    } else {
        message("Digidata is not supported in msvc build.")
    }
}

NIDAQPATH = $$(NIEXTCCOMPILERSUPP)
isEmpty(NIDAQPATH) {
    message("No NIDAQMx installation detected, building without NI support.")
    message("To force enable, add CONFIG+=nidaqmx to the qmake call.")
} else {
    !CONFIG(nonidaqmx): CONFIG += nidaqmx
}

nidaqmx {
    # NIDAQmx static build based on NI DAQmx 15.5.1
    mingw {
        DEPENDPATH += $$PWD/src/nidaqmx
        INCLUDEPATH += $$PWD/src/nidaqmx
        LIBS += $$PWD/src/nidaqmx/nidaqmx.a
    }
    msvc {
        contains(QMAKE_HOST.arch, x86_64) {
            LIBS += $$NIDAQPATH/lib64/msvc/NIDAQmx.lib
        } else {
            LIBS += $$NIDAQPATH/lib32/msvc/NIDAQmx.lib
        }
        INCLUDEPATH += $$NIDAQPATH/include
    }

    SOURCES += $$PWD/src/gui/NIDAQDlg.cpp \
        $$PWD/src/drivers/Nidaq.cpp
    FORMS += $$PWD/src/gui/NIDAQDlg.ui

    HEADERS += $$PWD/src/include/NIDAQDlg.h \
        $$PWD/src/include/Nidaq.h

    DEFINES += NATIONAL_INSTRUMENTS
}
