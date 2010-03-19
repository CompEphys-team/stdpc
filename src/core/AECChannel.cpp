#include "AECChannel.h"
#include <QObject>


AECChannel::AECChannel()
{
    inChnNum = -1;
    outChnNum = -1;

    active = false;
    saving = false;

    kerSampPer = 0.0;
    kernelTime = 0.0;
    v_e = 0.0;

    buffInd= 0;

    kernel.resize(0);
    iBuffer.resize(0);
    tBuffer.resize(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For fixed timestep (need to be removed eventually) (don't forget to change DCThread main loop as well!!)
//void AECChannel::Initialize(short int inChnNum, short int outChnNum, double kerSampPer, QVector<double> ker)
//{
//    this->inChnNum = inChnNum;
//    this->outChnNum = outChnNum;
//    this->kerSampPer = kerSampPer;
//
//    // Init kernel
//    kernel.resize(ker.size());
//    for ( int i=0; i<kernel.size(); i++ )
//        kernel[i] = ker[i];
//
//    int tempBufferSize= kernel.size();
//
//    // Init Ibuffer
//    iBuffer.resize(tempBufferSize);
//    for ( int i=0; i<iBuffer.size(); i++ )
//        iBuffer[i]= 0.0;
//
//    // Init Tbuffer
//    tBuffer.resize(tempBufferSize);
//    for ( int i=0; i<tBuffer.size(); i++ )
//        tBuffer[i]= 0.0;
//
//    active = true;
//
//}
//
//
//double AECChannel::CalculateVe(double current, double dt)
//{
//
////    if(kernelSize == 0){
////        QMessageBox::warning(mw, QObject::tr("Information"),
////                                 QObject::tr("AECChannel has not been initialized"),
////                                 QMessageBox::Ok);
////        return 0.0;
////    }
//
//
//    iBuffer.resize(kernel.size()-1);   // Delete last element
//    iBuffer.prepend(current);       // Append the new current to the beginning of the buffer
//
//    // Calculate the system response, ie. Ve electrode potential
//    v_e = 0.0;
//
//    for ( int i=0; i<kernel.size(); i++ )
//    {
//        // the same order, as Ibuffer is in reverse order (smaller index is later in time)
//        v_e += kernel[i]*iBuffer[i];
//    }
//
//    return v_e;
//}
// Fixed timestep codeblock ends
/////////////////////////////////////////////////////////////////////////////////////////////////////////



void AECChannel::Initialize(short int inChnNum, short int outChnNum, double kerSampPer, QVector<double> ker)
{
    this->inChnNum = inChnNum;
    this->outChnNum = outChnNum;
    this->kerSampPer = kerSampPer;
    this->kernelTime = kerSampPer * ker.size();

    // Init kernel
    kernel.resize(ker.size());
    for ( int i=0; i<kernel.size(); i++ )
        kernel[i] = ker[i];

    // Size of the temporary buffers
    // (somewhat arbitrarily assuming that the clamping frequency (1) will not be higher
    // than 'X' times the calibration frequency AND (2) for a longer period than the time duration of the kernel)
    int tempBufferSize= kernel.size() * 10;  // 'X' = 10

    // Init Ibuffer
    iBuffer.resize(tempBufferSize);
    for ( int i=0; i<iBuffer.size(); i++ )
        iBuffer[i]= 0.0;

    // Init Tbuffer
    tBuffer.resize(tempBufferSize);
    for ( int i=0; i<tBuffer.size(); i++ )
        tBuffer[i]= 1.0; // set it to a positive value to prevent useless calculation while the process is starting off

    buffInd = tBuffer.size()-1;

    active = true;
}


double AECChannel::CalculateVe(double current, double dt)
{
    //  Update the buffer index, and the time and current buffers
    if ( buffInd < tBuffer.size()-1 ) buffInd++;
    else                              buffInd = 0;
    tBuffer[buffInd] = dt;
    iBuffer[buffInd] = current;

    // Calculate the system response, ie. Ve electrode potential (v_e)

    v_e = 0.0;                  // the voltage artifact to calculate
    QVector<double> k_tr;       // the transformed kernel
    int ker_ind = 0;            // kernel index
    double t_curr = 0.0;        // running remaining time from the current level currently processed
    double t_remk = 0.0;        // remaining portion from the kernel element currently processed
    int length = 0;             // the length of the transformed kernel
    int run_ind = 0;            // running index variable

    // TODO: this transformed kernel calculation can be made faster (eg. the first 'for' loop does not necessary, 'k_tr' can be made static, etc.)

    // Calculate the transformed kernel length we need
    double t_sum = 0.0;
    run_ind = buffInd;

    for ( int i=0; i<tBuffer.size(); i++ )
    {
        t_sum += tBuffer[run_ind];
        length++;

        if ( t_sum >= kernelTime )
            break;

        if ( run_ind > 0 ) run_ind--;
        else               run_ind = tBuffer.size()-1;

    }

    // Transform kernel to the current time vector
    k_tr = QVector<double>(length);
    int i;
    run_ind = buffInd;

    for ( i=0; i<length; i++ )
    {
        t_curr = tBuffer[run_ind];

        // pre
        if ( t_remk != 0.0 )
        {
            if ( t_curr == t_remk )
            {
                k_tr[i] = t_remk/kerSampPer * kernel[ker_ind];
                t_remk = 0.0;
                ker_ind++;
                if ( ker_ind == kernel.size() )
                    break;
                if ( run_ind > 0 ) run_ind--;
                else               run_ind = tBuffer.size()-1;
                continue;
            }
            else if ( t_curr > t_remk )
            {
                k_tr[i] = t_remk/kerSampPer * kernel[ker_ind];
                t_curr -= t_remk;
                t_remk = 0.0;
                ker_ind++;
                if ( ker_ind == kernel.size() )
                    break;
            }
            else    // t_curr < t_remk
            {
                k_tr[i] = t_curr/kerSampPer * kernel[ker_ind];
                t_remk = t_remk - t_curr;

                if ( run_ind > 0 ) run_ind--;
                else               run_ind = tBuffer.size()-1;
                continue;
            }
        }

        // main
        while( t_curr >= kerSampPer )
        {
            k_tr[i] += kernel[ker_ind];

            ker_ind++;
            if ( ker_ind == kernel.size() )
                    break;

            t_curr -= kerSampPer;
        }

        // post
        if ( ker_ind == kernel.size() )
            break;

        if( t_curr == 0.0 )
            t_remk = 0.0;
        else
        {
            k_tr[i] += t_curr/kerSampPer * kernel[ker_ind];
            t_remk = kerSampPer - t_curr;
        }

        if ( run_ind > 0 ) run_ind--;
        else               run_ind = tBuffer.size()-1;

    }

    length = i; // Just to be safe from unexpected jump out from the previous loop

    // Calculate the convolution of the transformed kernel and the current vector
    //run_ind = buffInd;
    if ( buffInd > 0 ) run_ind=buffInd-1;
    else               run_ind = iBuffer.size()-1;

    for ( int i=0; i<length; i++ )
    {
        v_e += k_tr[i] * iBuffer[run_ind];

        if ( run_ind > 0 ) run_ind--;
        else               run_ind = iBuffer.size()-1;
    }
    
    return v_e;
}


void AECChannel::ResetChannel()
{
    v_e = 0.0;
    buffInd = 0;
    // Reset buffers
    for ( int i=0; i<iBuffer.size(); i++ )
        iBuffer[i] = 0.0;

    for ( int i=0; i<tBuffer.size(); i++ )
        tBuffer[i] = 0.0;

}


void AECChannel::Activate()
{
    active = true;
}

void AECChannel::Inactivate()
{
    active = false;
}

bool AECChannel::IsActive()
{
    return active;
}
