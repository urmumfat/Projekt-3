#include <matplot/matplot.h>
#include <iostream>
#include <string>
#include "AudioFile.h"
#include <cmath>
#include <vector>

using namespace matplot;
using namespace std;



void get_wave_input(string& type, double& amplitude, double& frequency, double& range, double& duty_cycle) {
    cout << "Enter the type of function (sin/cos/square/sawtooth): ";
    cin >> type;

    if (type == "sin" || type == "cos") {
        cout << "Enter the amplitude: ";
        cin >> amplitude;
        cout << "Enter the frequency: ";
        cin >> frequency;
        cout << "Enter the range (0 to range * 2 * pi): ";
        cin >> range;
    }
    else if (type == "square") {
        cout << "Enter the amplitude: ";
        cin >> amplitude;
        cout << "Enter the frequency: ";
        cin >> frequency;
        cout << "Enter the duty cycle (0 to 1): ";
        cin >> duty_cycle;
        cout << "Enter the range (0 to range * 2 * pi): ";
        cin >> range;
    }
    else if (type == "sawtooth") {
        cout << "Enter the amplitude: ";
        cin >> amplitude;
        cout << "Enter the frequency: ";
        cin >> frequency;
        cout << "Enter the range (0 to range * 2 * pi): ";
        cin >> range;
    }
    else {
        cerr << "Invalid function type. Please enter 'sin', 'cos', 'square', or 'sawtooth'." << endl;
        exit(1);
    }
}

vector<double> generate_wave(const string& type, const vector<double>& x, double amplitude, double frequency, double duty_cycle) {
    vector<double> y;

    if (type == "sin") {
        y = transform(x, [amplitude, frequency](double x) { return amplitude * sin(frequency * x); });
    }
    else if (type == "cos") {
        y = transform(x, [amplitude, frequency](double x) { return amplitude * cos(frequency * x); });
    }
    else if (type == "square") {
        y = transform(x, [amplitude, frequency, duty_cycle](double x) { return (fmod(x * frequency, 2 * pi) < 2 * pi * duty_cycle) ? amplitude : -amplitude; });
    }
    else if (type == "sawtooth") {
        y = transform(x, [amplitude, frequency](double x) { return amplitude * (2 * (x * frequency - floor(x * frequency + 0.5))); });
    }

    return y;
}

void plot_wave(const vector<double>& x, const vector<double>& y, const string& type) {
    plot(x, y);
    xlabel("x-axis");
    ylabel("y-axis");
    title(type + string(" Wave"));
    show();
}

void load_and_plot_audio() {
    AudioFile<double> audioFile;
    audioFile.load("../test-audio.wav");

    size_t num_samples = std::min(static_cast<size_t>(1000), audioFile.samples[0].size());

    vector<double> audioData(audioFile.samples[0].begin(), audioFile.samples[0].begin() + num_samples);

    plot(audioData);
    xlabel("Sample Number");
    ylabel("Amplitude");
    title("Audio Signal");
    show();
}

std::vector<float> load_audio_signal(const string& file_path, size_t max_samples = 1000) {
    AudioFile<float> audio_file;
    if (!audio_file.load(file_path)) {
        cout << "Error: Failed to load audio file." << endl;
        exit(1);
    }

    size_t num_samples = std::min(max_samples, audio_file.samples[0].size());
    vector<float> signal(audio_file.samples[0].begin(), audio_file.samples[0].begin() + num_samples);
    return signal;
}

vector<float> low_pass_filter(const vector<float>& signal, float alpha) {
    vector<float> filtered_signal(signal.size());
    filtered_signal[0] = signal[0];
    for (size_t i = 1; i < signal.size(); ++i) {
        filtered_signal[i] = alpha * signal[i] + (1 - alpha) * filtered_signal[i - 1];
    }
    return filtered_signal;
}

vector<float> high_pass_filter(const vector<float>& signal, float alpha) {
    vector<float> filtered_signal(signal.size());
    vector<float> low_pass = low_pass_filter(signal, alpha);
    for (size_t i = 0; i < signal.size(); ++i) {
        filtered_signal[i] = signal[i] - low_pass[i];
    }
    return filtered_signal;
}

void plot_audio_signals(const vector<float>& original_signal, const vector<float>& filtered_signal) {
    figure();
    plot(original_signal);
    title("Original Signal");
    show();

    figure();
    plot(filtered_signal);
    title("Filtered Signal (1D)");
    show();
}

void display_menu() {
    cout << "Select an option:" << endl;
    cout << "1. Generate and plot wave" << endl;
    cout << "2. Load and plot audio file" << endl;
    cout << "3. Load, filter, and plot audio file (high pass)" << endl;
    cout << "4. Load, filter, and plot audio file (low pass)" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    while (true) {
        display_menu();

        int choice;
        cin >> choice;

        if (choice == 1) {
            string type;
            double amplitude = 0.0, frequency = 0.0, range = 0.0, duty_cycle = 0.0;
            get_wave_input(type, amplitude, frequency, range, duty_cycle);

            auto x = linspace(0, range * 2 * pi, 1000);  // Increased number of points for better plot quality
            vector<double> y = generate_wave(type, x, amplitude, frequency, duty_cycle);

            plot_wave(x, y, type);
        }
        else if (choice == 2) {
            load_and_plot_audio();
        }
        else if (choice == 5) {
            break;
        }
        else if (choice == 4) {
            float alpha;
            cout << "give smoothing factor: ";
            cin >> alpha;
            vector<float> signal = load_audio_signal("../test-audio.wav");
            vector<float> filtered_signal = low_pass_filter(signal, alpha);
            plot_audio_signals(signal, filtered_signal);
        }
        else if (choice == 3) {
            float alpha;
            cout << "give smoothing factor: ";
            cin >> alpha;
            vector<float> signal = load_audio_signal("../test-audio.wav");
            vector<float> filtered_signal = high_pass_filter(signal, alpha);
            plot_audio_signals(signal, filtered_signal);
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
