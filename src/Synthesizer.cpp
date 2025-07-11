#include "vsynth/Synthesizer.h"
#include <cmath>
#include <algorithm>
#include <ranges>

// Voice Implementation
Voice::Voice(int n, float v, int oscillatorCount, int sampleRate)
    : note(n), velocity(v), isActive(true), phase(0.0f)
{
    float frequency = 440.0f * std::pow(2.0f, (n - 69) / 12.0f);
    
    // Create oscillators
    for (int i = 0; i < oscillatorCount; ++i) {
        auto osc = std::make_unique<Oscillator>(frequency, sampleRate);
        // Slightly detune additional oscillators for richness
        if (i > 0) {
            float detune = 1.0f + (i * 0.01f);
            osc->setFrequency(frequency * detune);
        }
        oscillators.push_back(std::move(osc));
    }
    
    envelope = std::make_unique<ADSREnvelope>(sampleRate);
    envelope->trigger();
}

float Voice::process(float deltaTime)
{
    if (!isActive) return 0.0f;
    
    float output = 0.0f;
    
    // Mix all oscillators
    for (auto& osc : oscillators) {
        output += osc->process();
    }
    
    // Average the oscillators
    if (!oscillators.empty()) {
        output /= static_cast<float>(oscillators.size());
    }
    
    // Apply envelope
    float envLevel = envelope->process();
    output *= envLevel * velocity;
    
    // Check if voice should be deactivated
    if (!envelope->isActive()) {
        isActive = false;
    }
    
    return output;
}

void Voice::release()
{
    envelope->release();
}

// Synthesizer Implementation
Synthesizer::Synthesizer(int sampleRate)
    : m_sampleRate(sampleRate)
    , m_deltaTime(1.0f / static_cast<float>(sampleRate))
    , m_attack(0.1f)
    , m_decay(0.2f)
    , m_sustain(0.7f)
    , m_release(0.5f)
    , m_waveform(0)
    , m_oscillatorCount(2)
    , m_vibratoRate(5.0f)
    , m_vibratoDepth(0.02f)
    , m_vibratoPhase(0.0f)
{
    m_effects = std::make_unique<Effects>(sampleRate);
}

Synthesizer::~Synthesizer() = default;

void Synthesizer::noteOn(int note, float velocity)
{
    // Check if we already have this note playing
    for (auto& voice : m_voices) {
        if (voice->note == note && voice->isActive) {
            voice->release(); // Release the old one
            break;
        }
    }
    
    // Remove inactive voices if we're at the limit
    if (m_voices.size() >= MAX_VOICES) {
        cleanupVoices();
        
        // If still at limit, remove the oldest voice
        if (m_voices.size() >= MAX_VOICES) {
            m_voices.erase(m_voices.begin());
        }
    }
    
    // Create new voice
    auto voice = std::make_unique<Voice>(note, velocity, m_oscillatorCount, m_sampleRate);
    
    // Set voice parameters
    voice->envelope->setAttack(m_attack);
    voice->envelope->setDecay(m_decay);
    voice->envelope->setSustain(m_sustain);
    voice->envelope->setRelease(m_release);
    
    for (auto& osc : voice->oscillators) {
        osc->setWaveform(static_cast<WaveformType>(m_waveform));
    }
    
    m_voices.push_back(std::move(voice));
}

void Synthesizer::noteOff(int note)
{
    for (auto& voice : m_voices) {
        if (voice->note == note && voice->isActive) {
            voice->release();
        }
    }
}

float Synthesizer::process()
{
    float output = 0.0f;
    
    // Calculate vibrato
    float vibrato = std::sin(m_vibratoPhase) * m_vibratoDepth;
    m_vibratoPhase += (2.0f * M_PI * m_vibratoRate) * m_deltaTime;
    if (m_vibratoPhase >= 2.0f * M_PI) {
        m_vibratoPhase -= 2.0f * M_PI;
    }
    
    // Process all voices
    for (auto& voice : m_voices) {
        if (voice->isActive) {
            // Apply vibrato to oscillators
            float baseFreq = noteToFrequency(voice->note);
            float vibratoFreq = baseFreq * (1.0f + vibrato);
            
            for (size_t i = 0; i < voice->oscillators.size(); ++i) {
                float detune = (i > 0) ? (1.0f + (i * 0.01f)) : 1.0f;
                voice->oscillators[i]->setFrequency(vibratoFreq * detune);
            }
            
            output += voice->process(m_deltaTime);
        }
    }
    
    // Clean up inactive voices periodically
    static int cleanupCounter = 0;
    if (++cleanupCounter > 1000) {
        cleanupVoices();
        cleanupCounter = 0;
    }
    
    // Apply effects
    output = m_effects->process(output);
    
    // Limit output
    return std::max(-1.0f, std::min(1.0f, output));
}

void Synthesizer::setAttack(float attack)
{
    m_attack = attack;
    for (auto& voice : m_voices) {
        voice->envelope->setAttack(attack);
    }
}

void Synthesizer::setDecay(float decay)
{
    m_decay = decay;
    for (auto& voice : m_voices) {
        voice->envelope->setDecay(decay);
    }
}

void Synthesizer::setSustain(float sustain)
{
    m_sustain = sustain;
    for (auto& voice : m_voices) {
        voice->envelope->setSustain(sustain);
    }
}

void Synthesizer::setRelease(float release)
{
    m_release = release;
    for (auto& voice : m_voices) {
        voice->envelope->setRelease(release);
    }
}

void Synthesizer::setWaveform(int waveform)
{
    m_waveform = waveform;
    for (auto& voice : m_voices) {
        for (auto& osc : voice->oscillators) {
            osc->setWaveform(static_cast<WaveformType>(waveform));
        }
    }
}

void Synthesizer::setOscillatorCount(int count)
{
    m_oscillatorCount = std::max(1, std::min(3, count));
    // Note: This will only affect new voices
}

void Synthesizer::setVibratoRate(float rate)
{
    m_vibratoRate = rate;
}

void Synthesizer::setVibratoDepth(float depth)
{
    m_vibratoDepth = depth;
}

void Synthesizer::setReverb(float reverb)
{
    m_effects->setReverbAmount(reverb);
}

void Synthesizer::setDelay(float delay)
{
    m_effects->setDelayAmount(delay);
}

void Synthesizer::cleanupVoices()
{
    m_voices.erase(
        std::remove_if(m_voices.begin(), m_voices.end(),
            [](const std::unique_ptr<Voice>& voice) {
                return !voice->isActive;
            }),
        m_voices.end()
    );
}

float Synthesizer::noteToFrequency(int note)
{
    return 440.0f * std::pow(2.0f, (note - 69) / 12.0f);
}
