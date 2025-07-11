#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <portaudio.h>
#include <vector>
#include <memory>
#include <mutex>
#include "Synthesizer.h"
#include "Recorder.h"

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();
    
    bool initialize(int sampleRate = 44100, int framesPerBuffer = 256);
    void shutdown();
    
    bool start();
    void stop();
    
    void noteOn(int note, float velocity);
    void noteOff(int note);
    
    // Parameter setters
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);
    void setWaveform(int waveform);
    void setOscillatorCount(int count);
    void setVibratoRate(float rate);
    void setVibratoDepth(float depth);
    void setReverb(float reverb);
    void setDelay(float delay);
    
    // Recording
    void startRecording();
    void stopRecording();
    bool isRecording() const;
    void startPlayback();
    void stopPlayback();
    bool isPlaying() const;
    void exportToFile(const std::string& filename);
    
    // FFT data access
    std::vector<float> getFFTData();
    
    int getSampleRate() const { return m_sampleRate; }
    
private:
    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);
    
    int processAudio(float* output, unsigned long framesPerBuffer);
    
    PaStream* m_stream;
    std::unique_ptr<Synthesizer> m_synthesizer;
    std::unique_ptr<Recorder> m_recorder;
    
    int m_sampleRate;
    int m_framesPerBuffer;
    bool m_isInitialized;
    bool m_isRunning;
    
    std::mutex m_audioMutex;
    
    // FFT buffer for analysis
    std::vector<float> m_fftBuffer;
    size_t m_fftBufferIndex;
    static const size_t FFT_SIZE = 1024;
};

#endif // AUDIOENGINE_H
