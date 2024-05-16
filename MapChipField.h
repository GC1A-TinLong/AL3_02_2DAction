#pragma once
#include <stdlib.h>
#include <cstdint>
#include <vector>
#include <map>
#include <string>

static inline const float kBlockWidth = 2.0f;
static inline const float kBlockWidth = 2.0f;

static inline const uint32_t kNumBlockVertical = 20;
static inline const uint32_t kNumBlockHorizontal = 100;

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

class MapChipField {
public:
	MapChipData mapChipData_;


private:

};
