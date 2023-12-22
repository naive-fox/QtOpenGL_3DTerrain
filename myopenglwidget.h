#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include "Camera.h"

#include <array>
#include <cstring>
#include <vector>
#include <iostream>


#include "FileStore.h"


class MyOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape{None,Map,Map2,Map3,Map4,Map5};
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    void drawShape(Shape shape);
    void setWireFrame(bool mode);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w,int h);
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
    void on_timeout();

private:
    Shape m_shape;

    QTimer m_timer;

    Camera m_camera;


private:
    void InitOpenGLMap();

    QOpenGLTexture* m_Texture_Map;

    QOpenGLShaderProgram m_ShaderProgram_Map;


    GLuint m_VAO_Map[3][3];
    GLuint m_VBO_Map[3][3];
    GLuint m_EBO_Map;
    

    static constexpr int SRTM_SIZE = 1201;
    static constexpr int BufferSize = SRTM_SIZE * SRTM_SIZE;

    using MapPiece_t = std::vector<float>;

    std::vector<float> m_Zoomed_heightMap_Analyzed;
    //  位置坐标            纹理坐标
    // 0.0, 95.5, 1.0,     1.0f, 1.0f
    void ZoomedPaddingHeightAnalyzed();
    
    std::vector<int> m_Zoomed_EBO_indices;
    void PaddingEBOIndices();

    // camera暂时尚未设置，高度会特别高，故将高度进行一个比例的缩放
    float m_HeightZoom = 100.0;

    fileStore::MapFile* m_MapFileData;

    MapPiece_t m_currentMapPiece[3][3];
    void GetPieceFromMapData(uint8_t row_index, uint8_t column_index, MapPiece_t& piece);

    MapPiece_t m_Map_vertices[3][3];
    // 此函数中，处理Piece原数据，添加位置坐标，纹理坐标，注意通过index来进行偏移
    void SetPosAndTexcoord(uint8_t row_index, uint8_t column_index, const MapPiece_t& piece, MapPiece_t& vertices);

};


#endif // MYOPENGLWIDGET_H
