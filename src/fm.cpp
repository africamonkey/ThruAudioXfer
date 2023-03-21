#include <iostream>
#include <cmath>

using namespace std;

const double pi = 3.14159265358979323846;

// Function to modulate the digital signal using FM
void modulateSignal(int signal[], int size, double carrierFreq, double sampleRate, double modIndex, double modSignal[])
{
    for (int i = 0; i < size; i++) {
        // Calculate the phase of the carrier signal
        double phase = 2 * pi * carrierFreq * i / sampleRate;
        
        // Calculate the frequency deviation based on the amplitude of the digital signal
        double freqDeviation = modIndex * signal[i];
        
        // Calculate the modulated signal using FM
        modSignal[i] = sin(phase + freqDeviation);
    }
}

// Function to demodulate the modulated signal back to the original digital signal
void demodulateSignal(double modSignal[], int size, double carrierFreq, double sampleRate, double modIndex, int demodSignal[])
{
    for (int i = 0; i < size; i++) {
        // Calculate the phase of the carrier signal
        double phase = 2 * pi * carrierFreq * i / sampleRate;
        
        // Calculate the instantaneous frequency of the modulated signal
        double instantFreq = (phase + modSignal[i]) / (2 * pi * i / sampleRate);
        
        // Calculate the amplitude of the original digital signal based on the frequency deviation
        demodSignal[i] = round(instantFreq / modIndex);
    }
}

// Example usage of the modulation and demodulation functions
int main()
{
    // Set the parameters of the modulation and demodulation process
    int size = 100;
    double carrierFreq = 1000.0;
    double sampleRate = 8000.0;
    double modIndex = 50.0;
    
    // Generate a test digital signal
    int signal[size];
    for (int i = 0; i < size; i++) {
        signal[i] = (i % 2 == 0) ? 1 : 0;
    }
    
    // Modulate the digital signal using FM
    double modSignal[size];
    modulateSignal(signal, size, carrierFreq, sampleRate, modIndex, modSignal);
    
    // Demodulate the modulated signal back to the original digital signal
    int demodSignal[size];
    demodulateSignal(modSignal, size, carrierFreq, sampleRate, modIndex, demodSignal);
    
    // Print the original and demodulated signals
    cout << "Original Signal: ";
    for (int i = 0; i < size; i++) {
        cout << signal[i] << " ";
    }
    cout << endl;
    
    cout << "Demodulated Signal: ";
    for (int i = 0; i < size; i++) {
        cout << demodSignal[i] << " ";
    }
    cout << endl;
    
    return 0;
}
