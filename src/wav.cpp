#include "wav.hpp"

void createWavFile(const std::string filename, const double* samples, const int sampleRate, const long unsigned N) {
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
			writeAsBytes(wav, static_cast<int>(samples[i] * MAX_AMPLITUDE), 2);
		}
	}
	wav.close();
}
