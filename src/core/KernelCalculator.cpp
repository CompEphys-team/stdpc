#include "KernelCalculator.h"
#include "math.h"


KernelCalculator::KernelCalculator()
{    

}

void KernelCalculator::SetParams(int fullKerLen, int elecKerLen, double sRate,
                                 QVector<double> iVector, QVector<double> vVector)
{
    fullKernel.resize(fullKerLen);
    elecKernel.resize(elecKerLen);

    samplingRate = sRate;

    iVec = iVector;
    vVec = vVector;
}


// Takes the current (iVector) and voltage (vVector) data vectors
// and calculates the kernels
void KernelCalculator::CalculateKernel()
{
    CalcFullKernel();
    CalcElecKernel();
}


// Full kernel calculation
// TODO: Consider the weighted convolution with the actual sampling time vector
//       instead of the currently used equally sampled one to achieve better result !!
void KernelCalculator::CalcFullKernel()
{
    int dataLen = iVec.size();
    double averV = 0.0;
    double averI = 0.0;

    // Calculate the correlation vector <v(n)i(n-k)>
    //   and the autocorrelation vector <i(n)i(n-k)>
    QVector<double> VI(fullKernel.size());
    QVector<double> II(fullKernel.size());


    // taking <v> as the reference potential simplifies the formulas (from Brette's code)
    for ( int i=0; i<dataLen; i++ )
    {
        averV += vVec[i];
        averI += iVec[i];
    }
    averV /= dataLen;
    averI /= dataLen;
    double averI2 = averI * averI;

    for ( int k=0; k<fullKernel.size(); k++ )
    {
        for ( int i=0; i<dataLen-k; i++ ) VI[k] += (vVec[k+i]-averV)*iVec[i];
        VI[k] /= dataLen-k;
        VI[k] -= averI2;

        for ( int i=0; i<dataLen-k; i++ ) II[k] += iVec[k+i]*iVec[i];
        II[k] /= dataLen-k;
    }

    fullKernel = LevonsinDurbin(II, VI);
}


// Levinson-Durbin algorithm
QVector<double> KernelCalculator::LevonsinDurbin(QVector<double> II, QVector<double> VI)
{
    int n = II.size();

    QVector<double> b(n);
    QVector<double> x(n);
    QVector<double> tmp(n);

    b[0] = 1.0 / II[0];
    x[0] = VI[0] * b[0];

    double alpha, beta, u, v;

    for ( int i=1; i<n; i++ )
    {
        alpha = 0.0;
        for ( int j=0; j<i; j++ ) alpha += II[j+1] * b[j];
        u = 1.0 / (1-alpha*alpha);
        v = -alpha * u;
        for ( int j=0; j<i; j++ ) tmp[j] = b[j];
        if ( i>1 ) for ( int j=0; j<i-1; j++ ) b[j+1] = v*tmp[i-2-j] + u*tmp[j];
        b[0] = v * tmp[i-1];
        b[i] = u * tmp[i-1];
        beta = VI[i];
        for ( int j=0; j<i; j++ ) beta -= II[i-j]*x[j];
        for ( int j=0; j<=i; j++ ) x[j] += beta*b[j];
    }

    return x;
}


// Electrode kernel calculation
void KernelCalculator::CalcElecKernel()
{
    QVector<double> fitParams(2);

    QVector<double> tailVector(fullKernel.size()-elecKernel.size());
    QVector<double> timeVector(fullKernel.size()-elecKernel.size());

    // Get the tail of the full kernel (membrane response)
    for ( int i=0; i<fullKernel.size()-elecKernel.size(); i++ )
    {
        tailVector[i] = fullKernel[elecKernel.size()+i];
        timeVector[i] = (elecKernel.size()+i)/samplingRate;
    }

    // Exponential fit to the tail to find the membrane time constant and resistance
    fitParams = ExpFit(timeVector, tailVector);
    if (fitParams[1] != 0.0) tau_m = -1.0/fitParams[1];
    else { tau_m = -1; return; }
    R_m = exp(fitParams[0])*tau_m*samplingRate;

    // Replace tail by fit to reduce noise
    for ( int i=elecKernel.size(); i<fullKernel.size(); i++ )
    {
        fullKernel[i] = R_m/tau_m/samplingRate * exp(-i/samplingRate/tau_m);
    }

    // Clean the negative values from the beginning
    for ( int i=0; fullKernel[i]<0.0; i++ ) fullKernel[i] = 0.0;

    // Electrode resistance
    R_e = 0.0;
    for ( int i=0; i<elecKernel.size(); i++ ) R_e += fullKernel[i] - R_m/tau_m/samplingRate * exp(-i/samplingRate/tau_m);

    OptimizeKe(); // final optimization on electrode kernel
    elecKernel[0] = fullKernel[0]; // restore first element for this implementation

    // Calculate tau_e and R_e for result check

    int maxIndex = 0;    // get maximal element to start exponenitial fitting from
    for ( int i=1; i<elecKernel.size(); i++ )
    {
        if ( elecKernel[i] > elecKernel[maxIndex] )   maxIndex = i;
    }

    tailVector.resize(elecKernel.size()-maxIndex);
    timeVector.resize(elecKernel.size()-maxIndex);

    for ( int i=0; i<elecKernel.size()-maxIndex; i++ )
    {
        tailVector[i] = elecKernel[maxIndex+i];
        timeVector[i] = i/samplingRate;    // first zero part (due to data acquisiton delay) is not included and second rising  part (DAQ transient?) is neglected
    }

    fitParams = ExpFit(timeVector, tailVector);
    if (fitParams[1] != 0.0) tau_e = -1.0/fitParams[1];
    else                     tau_e = -1;

    R_e = 0.0;
    for ( int i=0; i<elecKernel.size(); i++ ) R_e += elecKernel[i];

    return;
}


// Fits an exponential function onto the vectors 'x' (time) and 'y' (data)
// returns a,b coefficients in the form of y = a * exp(b*x)
QVector<double> KernelCalculator::ExpFit(QVector<double> x, QVector<double> y)
{
    QVector<double> fitParams(2);

    double a;
    double b;

    int n = x.size();

    // Substitute negative elements with the minimal positive value
    double minPosY = y[0];
    for ( int i=1; i<n; i++ ) if ( minPosY < 0.0 || (y[i] < minPosY && y[i] > 0.0)) minPosY = y[i];
    for ( int i=0; i<n; i++ ) if ( y[i] <= 0.0 ) y[i] = minPosY;

    double Sx2y    = 0.0;
    double Sylogy  = 0.0;
    double Sxy     = 0.0;
    double Sxylogy = 0.0;
    double Sy      = 0.0;

    // Calculate the sums
    for ( int i=0; i<n; i++ )
    {
        Sx2y    += x[i] * x[i] * y[i];
        Sylogy  += y[i] * log(y[i]);
        Sxy     += x[i] * y[i];
        Sxylogy += x[i] * y[i] * log(y[i]);
        Sy      += y[i];
    }

    // Solve for a and b
    a = ( Sx2y * Sylogy  - Sxy * Sxylogy ) / ( Sy * Sx2y - Sxy * Sxy );
    b = ( Sy   * Sxylogy - Sxy * Sylogy  ) / ( Sy * Sx2y - Sxy * Sxy );
    

    fitParams[0] = a;
    fitParams[1] = b;

    return fitParams;
}


// Optimize the residual electrode kernel component using the golden search algorithm
void KernelCalculator::OptimizeKe()
{
    double lowerX, upperX, middleX1, middleX2;
    double middleY1, middleY2;

    double goldenRatio = 1.0/((3+sqrt(5))/2);
    double tolerance = 0.001;

    lowerX = 0.5 * R_e;
    upperX = 2.0 * R_e;
    middleX1 = lowerX + (upperX-lowerX) * goldenRatio;
    middleX2 = lowerX + (upperX-lowerX) * (1-goldenRatio);

    middleY1 = RemoveKm(middleX1);
    middleY2 = RemoveKm(middleX2);

    int i = 0;
    while ( fabs(upperX-lowerX) > tolerance * R_e || i++ < 100 )
    {
        if ( middleY1 < middleY2 )
        {
            upperX = middleX2;
            middleX2 = middleX1;
            middleX1 = upperX-middleX2+lowerX;

            middleY2 = middleY1;
            middleY1 = RemoveKm(middleX1);
        }
        else {
            lowerX = middleX1;
            middleX1 = middleX2;
            middleX2 = upperX-middleX1+lowerX;

            middleY1 = middleY2;
            middleY2 = RemoveKm(middleX2);
        }
    }

    if ( middleY1 < middleY2 )
        RemoveKm(middleX1);
    else
        RemoveKm(middleX2);
}


// Calculates the electrode kernel and returns the remaining error on the full kernel
double KernelCalculator::RemoveKm(double Re)
{
    double error = 0.0;
    double alpha = R_m / Re / tau_m / samplingRate;
    double lambda = exp(-1.0/samplingRate/tau_m);
    QVector<double> Y(fullKernel.size());
    QVector<double> K_elec(fullKernel.size());

    Y[0] = alpha/(alpha+1) * fullKernel[0];
    for( int i=1; i<fullKernel.size(); i++ )
        Y[i] = alpha/(1+alpha)*fullKernel[i] + lambda/(1+alpha)*Y[i-1];

    for( int i=1; i<fullKernel.size(); i++ )  K_elec[i] = fullKernel[i] - Y[i];
    for( int i=0; i<elecKernel.size(); i++ )  elecKernel[i] = K_elec[i];

    for ( int i=elecKernel.size(); i<fullKernel.size(); i++ )  error += K_elec[i] * K_elec[i];

    return error;
}


QVector<double> KernelCalculator::GetFullKernel()
{
    QVector<double> returnKernel(fullKernel.size());

    for( int i=0; i<fullKernel.size(); i++ ) returnKernel[i] = fullKernel[i];

    return returnKernel;
}


void KernelCalculator::SetFullKernel(QVector<double> kernel)
{
    fullKernel.clear();
    for( int i=0; i<kernel.size(); i++ ) fullKernel.append(kernel[i]);
}

void KernelCalculator::SetElecKernel(QVector<double> kernel)
{
    elecKernel.clear();
    for( int i=0; i<kernel.size(); i++ ) elecKernel.append(kernel[i]);
}


QVector<double> KernelCalculator::GetElecKernel()
{
    QVector<double> returnKernel(elecKernel.size());

    for( int i=0; i<elecKernel.size(); i++ ) returnKernel[i] = elecKernel[i];

    return returnKernel;
}


double KernelCalculator::GetTauM()
{
   return tau_m;
}


double KernelCalculator::GetRM()
{
    return R_m;
}


double KernelCalculator::GetTauE()
{
    return tau_e;
}

double KernelCalculator::GetRE()
{
    return R_e;
}
