#pragma once

#include <fstream>
#include <string>

constexpr std::string_view CHUNK_ID = "RIFF";
constexpr std::string_view FORMAT = "WAVE";
constexpr std::string_view SUBCHUNK1_ID = "fmt ";
constexpr int SUBCHUNK1_SIZE = 16;
constexpr int AUDIO_FORMAT = 1;
constexpr int NUM_CHANNELS = 1; // 1 - mono | 2 - stero
constexpr int BLOCK_ALIGN = NUM_CHANNELS * (SUBCHUNK1_SIZE / 8);
constexpr int BITS_PER_SAMPLE = 16;
constexpr int MAX_AMPLITUDE = 32767; // 2^15-1
constexpr std::string_view SUBCHUNK2_ID = "data";
constexpr int INFO_SIZE = 32;

inline void writeAsBytes(std::ofstream& file, int value, int byte_size) {
	file.write(reinterpret_cast<const char*>(&value), byte_size);
}

void createWavFile(const std::string, const double*, const int, const long unsigned);
