#include "vsynth/AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine()
    : m_stream(nullptr)
    , m_sampleRate(44100)
    , m_framesPerBuffer(256)
    , m_isInitialized(false)
    , m_isRunning(false)
    , m_fftBufferIndex(0)
{
    m_fftBuffer.resize(FFT_SIZE, 0.0f);
}

AudioEngine::~AudioEngine()
{
    shutdown();
}

bool AudioEngine::initialize(int sampleRate, int framesPerBuffer)
{
    m_sampleRate = sampleRate;
    m_framesPerBuffer = framesPerBuffer;
    
    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    // Create synthesizer and recorder
    m_synthesizer = std::make_unique<Synthesizer>(m_sampleRate);
    m_recorder = std::make_unique<Recorder>(m_sampleRate);
    
    // Setup stream parameters
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default output device." << std::endl;
        Pa_Terminate();
        return false;
    }
    
    outputParameters.channelCount = 1; // Mono output
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;
    
    // Open stream
    err = Pa_OpenStream(&m_stream,
                        nullptr, // No input
                        &outputParameters,
                        m_sampleRate,
                        m_framesPerBuffer,
                        paClipOff,
                        audioCallback,
                        this);
    
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return false;
    }
    
    m_isInitialized = true;
    return true;
}

void AudioEngine::shutdown()
{
    if (m_isRunning) {
        stop();
    }
    
    if (m_stream) {
        Pa_CloseStream(m_stream);
        m_stream = nullptr;
    }
    
    if (m_isInitialized) {
        Pa_Terminate();
        m_isInitialized = false;
    }
}

bool AudioEngine::start()
{
    if (!m_isInitialized || !m_stream) {
        return false;
    }
    
    PaError err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    
    m_isRunning = true;
    return true;
}

void AudioEngine::stop()
{
    if (m_stream && m_isRunning) {
        Pa_StopStream(m_stream);
        m_isRunning = false;
    }
}

void AudioEngine::noteOn(int note, float velocity)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->noteOn(note, velocity);
        if (m_recorder) {
            m_recorder->recordNoteEvent(note, velocity, true);
        }
    }
}

void AudioEngine::noteOff(int note)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->noteOff(note);
        if (m_recorder) {
            m_recorder->recordNoteEvent(note, 0.0f, false);
        }
    }
}

void AudioEngine::setAttack(float attack)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setAttack(attack);
    }
}

void AudioEngine::setDecay(float decay)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setDecay(decay);
    }
}

void AudioEngine::setSustain(float sustain)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setSustain(sustain);
    }
}

void AudioEngine::setRelease(float release)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setRelease(release);
    }
}

void AudioEngine::setWaveform(int waveform)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setWaveform(waveform);
    }
}

void AudioEngine::setOscillatorCount(int count)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setOscillatorCount(count);
    }
}

void AudioEngine::setVibratoRate(float rate)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setVibratoRate(rate);
    }
}

void AudioEngine::setVibratoDepth(float depth)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setVibratoDepth(depth);
    }
}

void AudioEngine::setReverb(float reverb)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setReverb(reverb);
    }
}

void AudioEngine::setDelay(float delay)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_synthesizer) {
        m_synthesizer->setDelay(delay);
    }
}

void AudioEngine::startRecording()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_recorder) {
        m_recorder->startRecording();
    }
}

void AudioEngine::stopRecording()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_recorder) {
        m_recorder->stopRecording();
    }
}

bool AudioEngine::isRecording() const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_audioMutex));
    return m_recorder ? m_recorder->isRecording() : false;
}

void AudioEngine::startPlayback()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_recorder) {
        m_recorder->startPlayback();
    }
}

void AudioEngine::stopPlayback()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_recorder) {
        m_recorder->stopPlayback();
    }
}

bool AudioEngine::isPlaying() const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_audioMutex));
    return m_recorder ? m_recorder->isPlaying() : false;
}

void AudioEngine::exportToFile(const std::string& filename)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    if (m_recorder) {
        // Check file extension (C++20 features)
        if (filename.ends_with(".wav")) {
            m_recorder->exportToWAV(filename);
        } else if (filename.ends_with(".mid") || filename.ends_with(".midi")) {
            m_recorder->exportToMIDI(filename);
        } else {
            m_recorder->exportNoteEvents(filename);
        }
    }
}

std::vector<float> AudioEngine::getFFTData()
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    return m_fftBuffer;
}

int AudioEngine::audioCallback(const void* inputBuffer, void* outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void* userData)
{
    AudioEngine* engine = static_cast<AudioEngine*>(userData);
    return engine->processAudio(static_cast<float*>(outputBuffer), framesPerBuffer);
}

int AudioEngine::processAudio(float* output, unsigned long framesPerBuffer)
{
    std::lock_guard<std::mutex> lock(m_audioMutex);
    
    // Process playback events
    if (m_recorder && m_recorder->isPlaying()) {
        float deltaTime = static_cast<float>(framesPerBuffer) / static_cast<float>(m_sampleRate);
        m_recorder->processPlayback(deltaTime);
        
        auto events = m_recorder->getEventsToPlay();
        for (const auto& event : events) {
            if (event.isNoteOn) {
                m_synthesizer->noteOn(event.note, event.velocity);
            } else {
                m_synthesizer->noteOff(event.note);
            }
        }
    }
    
    // Generate audio
    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        float sample = 0.0f;
        
        if (m_synthesizer) {
            sample = m_synthesizer->process();
        }
        
        output[i] = sample;
        
        // Record audio sample
        if (m_recorder) {
            m_recorder->recordAudioSample(sample);
        }
        
        // Store sample for FFT analysis
        m_fftBuffer[m_fftBufferIndex] = sample;
        m_fftBufferIndex = (m_fftBufferIndex + 1) % FFT_SIZE;
    }
    
    return paContinue;
}
