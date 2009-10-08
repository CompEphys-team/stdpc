#include "AECChannel.h"
#include <QMessageBox>
#include <QObject>


AECChannel::AECChannel()       
{
    inChnNum= -1;
    outChnNum= -1;

    active= false;
    Ve= 0.0;
    kernelSize= 0;

    Kernel.resize(0);
    Ibuffer.resize(0);
}


AECChannel::AECChannel(short int inChnNum, short int outChnNum)
{
    this->inChnNum= inChnNum;
    this->outChnNum= outChnNum;

    active= false;
    Ve= 0.0;
    kernelSize= 0;
    freq= 0;

    Kernel.resize(0);
    Ibuffer.resize(0);
    Tbuffer.resize(0);
}


void AECChannel::initializeKernel(QVector<double> *kernel)//, long int freq, int tbSize)
{ 
    kernelSize= kernel->size();
    //tbufferSize= tbSize;

    // Init kernel
    Kernel.resize(kernelSize);
    for(int i=0; i<kernelSize; i++)
        Kernel[i]= (*kernel)[i];

    // Init Ibuffer
    Ibuffer.resize(kernelSize);
    for(int i=0; i<kernelSize; i++)
        Ibuffer[i]= 0.0;
            
    // Init Tbuffer
    //Tbuffer.resize(tbufferSize);
    //for(int i=0; i<tbufferSize; i++)
    //    Tbuffer[i]= 0.0;
    
    active= true;

}


double AECChannel::calcVe(double current)
{

//    if(kernelSize == 0){
//        QMessageBox::warning(mw, QObject::tr("Information"),
//                                 QObject::tr("AECChannel has not been initialized"),
//                                 QMessageBox::Ok);
//        return 0.0;
//    }


    Ibuffer.resize(kernelSize-1);   // Delete last element
    Ibuffer.prepend(current);       // Append the new current to the beginning of the buffer

    // Calculate the system response, ie. Ve electrode potential
    Ve= 0.0;

    for(int i=0; i<kernelSize; i++)
    {
        // the same order, as Ibuffer is in reverse order (smaller index is later in time)
        Ve += Kernel[i]*Ibuffer[i];
    }

    return Ve;
}


void AECChannel::resetChannel()
{
    Ve= 0.0;

    // Reset Ibuffer
    for(int i=0; i<kernelSize; i++)
        Ibuffer[i]= 0.0;
}



