#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <memory>

// Base class for Waveform
class Waveform {
public:
    virtual float generate(float time) = 0;
    virtual void setFrequency(float frequency) = 0;
    virtual void setAmplitude(float amplitude) = 0;
    virtual void setPhase(float phase) = 0;
    virtual ~Waveform() = default;
};

// Derived class for Sine Wave
class SineWave : public Waveform {
private:
    float frequency;
    float amplitude;
    float phase;
    
public:
    SineWave(float freq, float amp, float ph) 
        : frequency(freq), amplitude(amp), phase(ph) {}

    float generate(float time) override {
        return amplitude * std::sin(2 * M_PI * frequency * time + phase);
    }

    void setFrequency(float freq) override { frequency = freq; }
    void setAmplitude(float amp) override { amplitude = amp; }
    void setPhase(float ph) override { phase = ph; }
};

// Derived class for Square Wave
class SquareWave : public Waveform {
private:
    float frequency;
    float amplitude;
    float phase;

public:
    SquareWave(float freq, float amp, float ph)
        : frequency(freq), amplitude(amp), phase(ph) {}

    float generate(float time) override {
        float sineValue = std::sin(2 * M_PI * frequency * time + phase);
        return (sineValue >= 0) ? amplitude : -amplitude;
    }

    void setFrequency(float freq) override { frequency = freq; }
    void setAmplitude(float amp) override { amplitude = amp; }
    void setPhase(float ph) override { phase = ph; }
};

// Derived class for Triangle Wave
class TriangleWave : public Waveform {
private:
    float frequency;
    float amplitude;
    float phase;

public:
    TriangleWave(float freq, float amp, float ph)
        : frequency(freq), amplitude(amp), phase(ph) {}

    float generate(float time) override {
        float period = 1.0 / frequency;
        float value = 2 * amplitude / M_PI * std::asin(std::sin(2 * M_PI * frequency * time + phase));
        return value;
    }

    void setFrequency(float freq) override { frequency = freq; }
    void setAmplitude(float amp) override { amplitude = amp; }
    void setPhase(float ph) override { phase = ph; }
};

void generateAndSaveWaveform(Waveform& waveform, float duration, int sampleRate, const std::string& filename) {
    std::vector<std::pair<float, float> > waveformData;
    float timeStep = 1.0f / sampleRate;

    // Generate waveform data
    for (float t = 0; t <= duration; t += timeStep) {
        float value = waveform.generate(t);
        waveformData.push_back({t, value});
    }

    // Save to CSV file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "Time,Value\n";
        for (const auto& data : waveformData) {
            file << data.first << "," << data.second << "\n";
        }
        file.close();
        std::cout << "Waveform saved to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file for writing.\n";
    }
}

int main() {
    int waveformType;
    float frequency, amplitude, phase = 0.0f;
    float duration = 1.0f;
    int sampleRate = 1000;

    std::cout << "Select Waveform Type (1: Sine, 2: Square, 3: Triangle): ";
    std::cin >> waveformType;
    std::cout << "Enter Frequency (Hz): ";
    std::cin >> frequency;
    std::cout << "Enter Amplitude: ";
    std::cin >> amplitude;
    std::cout << "Enter Phase Shift (radians, optional, default=0): ";
    std::cin >> phase;

    std::unique_ptr<Waveform> waveform;
    switch (waveformType) {
        case 1:
            waveform = std::make_unique<SineWave>(frequency, amplitude, phase);
            break;
        case 2:
            waveform = std::make_unique<SquareWave>(frequency, amplitude, phase);
            break;
        case 3:
            waveform = std::make_unique<TriangleWave>(frequency, amplitude, phase);
            break;
        default:
            std::cerr << "Invalid waveform type selected.\n";
            return 1;
    }

    // Generate and save waveform data
    generateAndSaveWaveform(*waveform, duration, sampleRate, "waveform.csv");

    return 0;
}

