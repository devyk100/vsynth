#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <vector>
#include <memory>
#include <map>
#include "Oscillator.h"
#include "ADSREnvelope.h"
#include "Effects.h"

struct Voice {
    int note;
    float velocity;
    std::vector<std::unique_ptr<Oscillator>> oscillators;
    std::unique_ptr<ADSREnvelope> envelope;
    bool isActive;
    float phase;
    
    Voice(int n, float v, int oscillatorCount, int sampleRate);
    ~Voice() = default;
    
    float process(float deltaTime);
    void release();
};

class Synthesizer
{
public:
    Synthesizer(int sampleRate);
    ~Synthesizer();
    
    void noteOn(int note, float velocity);
    void noteOff(int note);
    
    float process();
    
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
    
private:
    void cleanupVoices();
    float noteToFrequency(int note);
    
    int m_sampleRate;
    float m_deltaTime;
    
    std::vector<std::unique_ptr<Voice>> m_voices;
    static const int MAX_VOICES = 16; // Polyphony limit
    
    // Global parameters
    float m_attack;
    float m_decay;
    float m_sustain;
    float m_release;
    int m_waveform;
    int m_oscillatorCount;
    float m_vibratoRate;
    float m_vibratoDepth;
    
    // Effects
    std::unique_ptr<Effects> m_effects;
    
    // Vibrato LFO
    float m_vibratoPhase;
};

#endif // SYNTHESIZER_H
