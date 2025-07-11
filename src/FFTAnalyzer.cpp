#include "vsynth/FFTAnalyzer.h"
#include <cmath>
#include <algorithm>

FFTAnalyzer::FFTAnalyzer(int fftSize)
    : m_fftSize(fftSize)
    , m_useWindow(true)
{
    // Allocate FFTW buffers
    m_inputBuffer = fftwf_alloc_real(m_fftSize);
    m_outputBuffer = fftwf_alloc_complex(m_fftSize / 2 + 1);
    
    // Create FFTW plan
    m_plan = fftwf_plan_dft_r2c_1d(m_fftSize, m_inputBuffer, m_outputBuffer, FFTW_ESTIMATE);
    
    // Initialize window function (Hann window)
    m_window.resize(m_fftSize);
    for (int i = 0; i < m_fftSize; ++i) {
        m_window[i] = 0.5f * (1.0f - std::cos(2.0f * PI * i / (m_fftSize - 1)));
    }
    
    // Initialize magnitude spectrum
    m_magnitudeSpectrum.resize(m_fftSize / 2 + 1);
}

FFTAnalyzer::~FFTAnalyzer()
{
    fftwf_destroy_plan(m_plan);
    fftwf_free(m_inputBuffer);
    fftwf_free(m_outputBuffer);
}

void FFTAnalyzer::processBuffer(const std::vector<float>& audioBuffer)
{
    if (audioBuffer.size() < static_cast<size_t>(m_fftSize)) {
        // Zero-pad if buffer is too small
        std::fill(m_inputBuffer, m_inputBuffer + m_fftSize, 0.0f);
        std::copy(audioBuffer.begin(), audioBuffer.end(), m_inputBuffer);
    } else {
        // Use the last m_fftSize samples
        std::copy(audioBuffer.end() - m_fftSize, audioBuffer.end(), m_inputBuffer);
    }
    
    // Apply window function
    if (m_useWindow) {
        applyWindow();
    }
    
    // Compute FFT
    computeFFT();
}

std::vector<float> FFTAnalyzer::getMagnitudeSpectrum()
{
    return m_magnitudeSpectrum;
}

std::vector<float> FFTAnalyzer::getBinFrequencies(int sampleRate)
{
    std::vector<float> frequencies;
    frequencies.reserve(m_magnitudeSpectrum.size());
    
    float binWidth = static_cast<float>(sampleRate) / static_cast<float>(m_fftSize);
    
    for (size_t i = 0; i < m_magnitudeSpectrum.size(); ++i) {
        frequencies.push_back(i * binWidth);
    }
    
    return frequencies;
}

void FFTAnalyzer::setWindowFunction(bool useWindow)
{
    m_useWindow = useWindow;
}

void FFTAnalyzer::applyWindow()
{
    for (int i = 0; i < m_fftSize; ++i) {
        m_inputBuffer[i] *= m_window[i];
    }
}

void FFTAnalyzer::computeFFT()
{
    // Execute FFT
    fftwf_execute(m_plan);
    
    // Calculate magnitude spectrum
    for (int i = 0; i < m_fftSize / 2 + 1; ++i) {
        float real = m_outputBuffer[i][0];
        float imag = m_outputBuffer[i][1];
        float magnitude = std::sqrt(real * real + imag * imag);
        
        // Convert to dB scale (with floor to avoid log(0))
        magnitude = std::max(magnitude, 1e-10f);
        m_magnitudeSpectrum[i] = 20.0f * std::log10(magnitude);
    }
    
    // Normalize to 0-1 range for display
    if (!m_magnitudeSpectrum.empty()) {
        float maxMag = *std::max_element(m_magnitudeSpectrum.begin(), m_magnitudeSpectrum.end());
        float minMag = maxMag - 60.0f; // 60dB dynamic range
        
        for (float& mag : m_magnitudeSpectrum) {
            mag = std::max(0.0f, std::min(1.0f, (mag - minMag) / (maxMag - minMag)));
        }
    }
}
