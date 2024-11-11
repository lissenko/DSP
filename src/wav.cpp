#include "wav.hpp"

void createWavFile(const std::string filename, const double* samples_left, const double* samples_right, const long unsigned N, const int sampleRate) {
	std::ofstream wav;
	int subchunk2_size = static_cast<int>(N) * NUM_CHANNELS * (SUBCHUNK1_SIZE / 8);
	int chunk_size = INFO_SIZE + subchunk2_size;
	int byte_rate = sampleRate * NUM_CHANNELS * (SUBCHUNK1_SIZE / 2);

	wav.open(filename, std::ios::binary);
	if (wav.is_open()) {
		wav << CHUNK_ID;
		writeAsBytes(wav, chunk_size, 4);
		wav << FORMAT;
		wav << SUBCHUNK1_ID;
		writeAsBytes(wav, SUBCHUNK1_SIZE, 4);
		writeAsBytes(wav, AUDIO_FORMAT, 2);
		writeAsBytes(wav, NUM_CHANNELS, 2);
		writeAsBytes(wav, sampleRate, 4);
		writeAsBytes(wav, byte_rate, 4);
		writeAsBytes(wav, BLOCK_ALIGN, 2);
		writeAsBytes(wav, BITS_PER_SAMPLE, 2);
		wav << SUBCHUNK2_ID;
		writeAsBytes(wav, subchunk2_size, 4);
		for (long unsigned i = 0; i < N; ++i) {
			writeAsBytes(wav, static_cast<int16_t>(samples_left[i] * MAX_AMPLITUDE), 2);
			writeAsBytes(wav, static_cast<int16_t>(samples_right[i] * MAX_AMPLITUDE), 2);
		}
	}
	wav.close();
}

void readWavFile(const std::string &filename, std::vector<double> &samples_left, std::vector<double> &samples_right) {
    std::ifstream wav(filename, std::ios::binary);
    if (!wav.is_open()) {
        std::cerr << "Error opening WAV file: " << filename << std::endl;
        return;
    }
    char chunkID[4], format[4], subchunk1ID[4], subchunk2ID[4];
    int chunkSize, subchunk1Size, subchunk2Size, byteRate;
    int16_t audioFormat, numChannels, blockAlign, bitsPerSample;
	int sampleRate;

    wav.read(chunkID, 4);  // "RIFF"
    readAsBytes(wav, chunkSize, 4);  // Chunk size
    wav.read(format, 4);  // "WAVE"
    wav.read(subchunk1ID, 4);  // "fmt "
    readAsBytes(wav, subchunk1Size, 4);  // Subchunk1 size
    readAsBytes(wav, audioFormat, 2);  // Audio format
    readAsBytes(wav, numChannels, 2);  // Number of channels
    readAsBytes(wav, sampleRate, 4);  // Sample rate
    readAsBytes(wav, byteRate, 4);  // Byte rate
    readAsBytes(wav, blockAlign, 2);  // Block align
    readAsBytes(wav, bitsPerSample, 2);  // Bits per sample
    
    if (std::string(chunkID, 4) != "RIFF" || std::string(format, 4) != "WAVE" || audioFormat != 1 || numChannels != 2 || bitsPerSample != 16) {
        std::cerr << "Unsupported WAV file format." << std::endl;
		std::cout << "Chunk ID: " << std::string(chunkID, 4) << std::endl;  // Convert to std::string
		std::cout << "Format: " << std::string(format, 4) << std::endl;      // Convert to std::string
		std::cout << "Audio Format: " << audioFormat << std::endl;
		std::cout << "Bits per Sample: " << bitsPerSample << std::endl;
        return;
    }
    
    wav.read(subchunk2ID, 4);  // "data"
    readAsBytes(wav, subchunk2Size, 4);  // Subchunk2 size
    
    long unsigned N = subchunk2Size / (numChannels * (bitsPerSample / 8));  // Number of samples per channel
    samples_left.resize(N);
    samples_right.resize(N);
    
    // Read the audio data
    for (long unsigned i = 0; i < N; ++i) {
        int16_t leftSample, rightSample;
        readAsBytes(wav, leftSample, 2);  // Read left channel sample
        readAsBytes(wav, rightSample, 2);  // Read right channel sample
        
        samples_left[i] = leftSample / static_cast<double>(MAX_AMPLITUDE);  // Normalize left channel sample
        samples_right[i] = rightSample / static_cast<double>(MAX_AMPLITUDE);  // Normalize right channel sample
    }
    
    wav.close();
}
