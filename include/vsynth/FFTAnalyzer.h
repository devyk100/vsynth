#ifndef FFTANALYZER_H
#define FFTANALYZER_H

#include <vector>
#include <complex>
#include <fftw3.h>

class FFTAnalyzer
{
public:
    FFTAnalyzer(int fftSize = 1024);
    ~FFTAnalyzer();
    
    void processBuffer(const std::vector<float>& audioBuffer);
    std::vector<float> getMagnitudeSpectrum();
    std::vector<float> getBinFrequencies(int sampleRate);
    
    void setWindowFunction(bool useWindow = true);
    
    int getFFTSize() const { return m_fftSize; }
    
private:
    void applyWindow();
    void computeFFT();
    
    int m_fftSize;
    bool m_useWindow;
    
    // FFTW data
    float* m_inputBuffer;
    fftwf_complex* m_outputBuffer;
    fftwf_plan m_plan;
    
    // Window function
    std::vector<float> m_window;
    
    // Results
    std::vector<float> m_magnitudeSpectrum;
    
    // Constants
    static constexpr float PI = 3.14159265359f;
};

#endif // FFTANALYZER_H
