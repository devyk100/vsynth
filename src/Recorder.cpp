#include "vsynth/Recorder.h"
#include <iostream>
#include <algorithm>

Recorder::Recorder(int sampleRate)
    : m_sampleRate(sampleRate)
    , m_isRecording(false)
    , m_isPlaying(false)
    , m_recordingTime(0.0f)
    , m_playbackTime(0.0f)
    , m_playbackIndex(0)
{
}

void Recorder::startRecording()
{
    m_isRecording = true;
    m_recordingTime = 0.0f;
    m_noteEvents.clear();
    m_audioBuffer.clear();
}

void Recorder::stopRecording()
{
    m_isRecording = false;
}

void Recorder::startPlayback()
{
    if (!m_noteEvents.empty()) {
        m_isPlaying = true;
        m_playbackTime = 0.0f;
        m_playbackIndex = 0;
        m_eventsToPlay.clear();
    }
}

void Recorder::stopPlayback()
{
    m_isPlaying = false;
    m_eventsToPlay.clear();
}

void Recorder::recordNoteEvent(int note, float velocity, bool isNoteOn)
{
    if (m_isRecording) {
        m_noteEvents.emplace_back(m_recordingTime, note, velocity, isNoteOn);
    }
}

void Recorder::recordAudioSample(float sample)
{
    if (m_isRecording) {
        m_audioBuffer.push_back(sample);
        m_recordingTime += 1.0f / static_cast<float>(m_sampleRate);
    }
}

void Recorder::processPlayback(float deltaTime)
{
    if (!m_isPlaying) return;
    
    m_playbackTime += deltaTime;
    
    // Find events that should be played at this time
    m_eventsToPlay.clear();
    
    while (m_playbackIndex < m_noteEvents.size()) {
        const auto& event = m_noteEvents[m_playbackIndex];
        
        if (event.timestamp <= m_playbackTime) {
            m_eventsToPlay.push_back(event);
            m_playbackIndex++;
        } else {
            break;
        }
    }
    
    // Check if playback is finished
    if (m_playbackIndex >= m_noteEvents.size()) {
        // Allow some time for release phases
        if (m_playbackTime > m_recordingTime + 2.0f) {
            stopPlayback();
        }
    }
}

std::vector<NoteEvent> Recorder::getEventsToPlay()
{
    return m_eventsToPlay;
}

void Recorder::clear()
{
    m_noteEvents.clear();
    m_audioBuffer.clear();
    m_recordingTime = 0.0f;
    m_playbackTime = 0.0f;
    m_playbackIndex = 0;
    m_eventsToPlay.clear();
}

void Recorder::exportToWAV(const std::string& filename)
{
    if (m_audioBuffer.empty()) {
        std::cerr << "No audio data to export" << std::endl;
        return;
    }
    
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }
    
    // Convert float samples to 16-bit PCM
    std::vector<int16_t> pcmData;
    pcmData.reserve(m_audioBuffer.size());
    
    for (float sample : m_audioBuffer) {
        // Clamp and convert to 16-bit
        sample = std::max(-1.0f, std::min(1.0f, sample));
        int16_t pcmSample = static_cast<int16_t>(sample * 32767.0f);
        pcmData.push_back(pcmSample);
    }
    
    int dataSize = static_cast<int>(pcmData.size() * sizeof(int16_t));
    writeWAVHeader(file, dataSize);
    
    // Write PCM data
    file.write(reinterpret_cast<const char*>(pcmData.data()), dataSize);
    
    file.close();
    std::cout << "Exported audio to: " << filename << std::endl;
}

void Recorder::exportToMIDI(const std::string& filename)
{
    writeMIDIFile(filename);
}

void Recorder::exportNoteEvents(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }
    
    file << "# VSynth Note Events Export\n";
    file << "# Format: timestamp note velocity on/off\n";
    
    for (const auto& event : m_noteEvents) {
        file << event.timestamp << " " 
             << event.note << " " 
             << event.velocity << " " 
             << (event.isNoteOn ? "on" : "off") << "\n";
    }
    
    file.close();
    std::cout << "Exported note events to: " << filename << std::endl;
}

void Recorder::writeWAVHeader(std::ofstream& file, int dataSize)
{
    // WAV header structure
    struct WAVHeader {
        char riff[4] = {'R', 'I', 'F', 'F'};
        uint32_t fileSize;
        char wave[4] = {'W', 'A', 'V', 'E'};
        char fmt[4] = {'f', 'm', 't', ' '};
        uint32_t fmtSize = 16;
        uint16_t audioFormat = 1; // PCM
        uint16_t numChannels = 1; // Mono
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign = 2; // 16-bit mono
        uint16_t bitsPerSample = 16;
        char data[4] = {'d', 'a', 't', 'a'};
        uint32_t dataSize;
    };
    
    WAVHeader header;
    header.sampleRate = static_cast<uint32_t>(m_sampleRate);
    header.byteRate = header.sampleRate * header.blockAlign;
    header.dataSize = static_cast<uint32_t>(dataSize);
    header.fileSize = sizeof(WAVHeader) - 8 + header.dataSize;
    
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

void Recorder::writeMIDIFile(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open MIDI file for writing: " << filename << std::endl;
        return;
    }
    
    // Simple MIDI file format (Type 0, single track)
    // This is a basic implementation - a full MIDI writer would be more complex
    
    // MIDI Header
    file.write("MThd", 4);
    uint32_t headerLength = 6;
    file.write(reinterpret_cast<const char*>(&headerLength), 4);
    uint16_t format = 0; // Type 0
    uint16_t tracks = 1;
    uint16_t division = 480; // Ticks per quarter note
    
    // Write in big-endian format
    headerLength = __builtin_bswap32(headerLength);
    format = __builtin_bswap16(format);
    tracks = __builtin_bswap16(tracks);
    division = __builtin_bswap16(division);
    
    file.write(reinterpret_cast<const char*>(&format), 2);
    file.write(reinterpret_cast<const char*>(&tracks), 2);
    file.write(reinterpret_cast<const char*>(&division), 2);
    
    // Track header
    file.write("MTrk", 4);
    
    // Calculate track data (simplified)
    std::vector<uint8_t> trackData;
    
    // Add note events (simplified conversion)
    float lastTime = 0.0f;
    for (const auto& event : m_noteEvents) {
        uint32_t deltaTime = static_cast<uint32_t>((event.timestamp - lastTime) * 480.0f);
        
        // Write variable length quantity for delta time
        if (deltaTime < 128) {
            trackData.push_back(static_cast<uint8_t>(deltaTime));
        } else {
            // Simplified - handle only small delta times
            trackData.push_back(0);
        }
        
        // MIDI event
        if (event.isNoteOn) {
            trackData.push_back(0x90); // Note on, channel 0
        } else {
            trackData.push_back(0x80); // Note off, channel 0
        }
        
        trackData.push_back(static_cast<uint8_t>(event.note));
        trackData.push_back(static_cast<uint8_t>(event.velocity * 127.0f));
        
        lastTime = event.timestamp;
    }
    
    // End of track
    trackData.push_back(0x00); // Delta time
    trackData.push_back(0xFF); // Meta event
    trackData.push_back(0x2F); // End of track
    trackData.push_back(0x00); // Length
    
    uint32_t trackLength = static_cast<uint32_t>(trackData.size());
    trackLength = __builtin_bswap32(trackLength);
    file.write(reinterpret_cast<const char*>(&trackLength), 4);
    file.write(reinterpret_cast<const char*>(trackData.data()), trackData.size());
    
    file.close();
    std::cout << "Exported MIDI to: " << filename << std::endl;
}
