#pragma once

#include <QDebug>

#include <fstream>
#include <array>
#include <vector>

namespace fileStore
{
    static constexpr int PIECE_SIZE = 100;
    static constexpr int FILE_SIZE = 12;

    static constexpr int SRTM_SIZE = 1201;

    using MapPiece_t = std::vector<float>;
    using MapFile_t = std::array<std::array<MapPiece_t *, FILE_SIZE> *, FILE_SIZE>;

    class MapFile
    {
    public:
        MapFile();
        ~MapFile();

        MapPiece_t *GetPiece(uint8_t row_index, uint8_t column_index) const;

    private:
        bool InitMapFileMemoryPool();

        bool LoadHgtMapFile();

        void SetPiece(uint8_t row_index, uint8_t column_index,float value);

        float m_zoomedRatio = 100.f;
        MapFile_t *m_MapData;
    };
}
