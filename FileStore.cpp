
#include "FileStore.h"

namespace fileStore
{
    MapFile::MapFile()
    {
        InitMapFileMemoryPool();
        LoadHgtMapFile();
    }
    MapFile::~MapFile()
    {
        
    }

    bool MapFile::InitMapFileMemoryPool()
    {
        m_MapData = new MapFile_t;
        if(m_MapData == nullptr)
        {
            qDebug()<<"New MapFile Memory Pool Failed!!!";
            return false;
        }

        for(auto& row_piece : *m_MapData)
        {
            row_piece = new std::array<MapPiece_t*,FILE_SIZE>;
            if(row_piece == nullptr)
            {
                qDebug()<<"New Row Piece Memory Pool Failed!!!";
                return false;
            }
            for(auto& column_piece : * row_piece)
            {
                column_piece = new MapPiece_t;
                column_piece->reserve(10100);
                if(column_piece == nullptr)
                {
                    qDebug()<<"New Column Piece Memory Pool Failed!!!";
                    return false;
                }
            }
        }
        return true;
    }

    bool MapFile::LoadHgtMapFile()
    {
        std::ifstream file("N26E119.hgt", std::ios::in | std::ios::binary);
        if(!file.is_open())
        {
            qDebug()<<"Open file failed!!!\n";
            return false;
        }

        unsigned char buffer[2];
        for (int i = 0; i < SRTM_SIZE; i++)
        {
            for (int j = 0; j < SRTM_SIZE; j++)
            {
                if(!file.read(reinterpret_cast<char*>(buffer),2))
                {
                    qDebug() << "Error read file!";
                    return false;
                }
                //m_heightMap[i][j] = (float)(buffer[0] << 8 | buffer[1]);

                int row_index = static_cast<int>(i / 100);
                int column_index = static_cast<int>(j / 100);
                if(row_index > 11 || column_index > 11)
                    continue;
    
                SetPiece(row_index, column_index,static_cast<float>((buffer[0] << 8 | buffer[1])/m_zoomedRatio));
            }
        }
        file.close();
        
        return true;
    }

    MapPiece_t* MapFile::GetPiece(uint8_t row_index, uint8_t column_index) const
    {
        if(m_MapData == nullptr)
        {
            qDebug()<<"MapData is nullptr";
            return  nullptr;
        }

        return m_MapData->at(row_index)->at(column_index);
    }

    void MapFile::SetPiece(uint8_t row_index, uint8_t column_index, float value)
    {
        if(m_MapData == nullptr)
        {
            qDebug()<<"MapData is nullptr";
        }

        m_MapData->at(row_index)->at(column_index)->push_back(value);
    }
}