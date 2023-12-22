#include "myopenglwidget.h"
#include <QTime>
#include <QDebug>
#include <math.h>
#include <fstream>

#define TIME_OUT_INTERVAL 100

float ratio=0.5;
int angle=0;

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    
    connect(&m_timer,&QTimer::timeout,this,&MyOpenGLWidget::on_timeout);
    m_timer.start(TIME_OUT_INTERVAL);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_camera.Position=QVector3D(15.0,25.0,12.0);


    m_MapFileData = new fileStore::MapFile();

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            GetPieceFromMapData(i,j,m_currentMapPiece[i][j]);
            SetPosAndTexcoord(i,j,m_currentMapPiece[i][j],m_Map_vertices[i][j]);
        }
    }


    //ZoomedPaddingHeightAnalyzed();
    PaddingEBOIndices();
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent();
    // glDeleteBuffers(1,&EBO);
    // glDeleteBuffers(2,VBO);
    // glDeleteVertexArrays(2,VAO);
    doneCurrent();

    if(m_MapFileData != nullptr)
        delete m_MapFileData;
}

void MyOpenGLWidget::drawShape(MyOpenGLWidget::Shape shape)
{
    m_shape=shape;
    update();
}

void MyOpenGLWidget::setWireFrame(bool mode)
{
    makeCurrent();
    if(mode){
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    update();
    doneCurrent();
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    InitOpenGLMap();


}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);

    QMatrix4x4 model;   // 模型矩阵
    QMatrix4x4 view;    // 观察矩阵
    QMatrix4x4 projection;  // 投影矩阵

    m_ShaderProgram_Map.bind();
    m_Texture_Map->bind(0);
        
    model.rotate(30,1.0f,0.0f,0.0f);
    model.rotate(angle,0.0f,1.0f,0.0f);
    view=m_camera.getViewMatrix();
    projection.perspective(45,(float)width()/height(),0.1f,100);

    m_ShaderProgram_Map.setUniformValue("model",model);
    m_ShaderProgram_Map.setUniformValue("view",view);
    m_ShaderProgram_Map.setUniformValue("projection",projection);
    switch (m_shape) {
    case Map:  
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                // 绑定VAO
                glBindVertexArray(m_VAO_Map[i][j]);
                if(glGetError() != GL_NO_ERROR)
                    qDebug()<<"Running Bind VAO Failed!!!";
            

                glDrawElements(GL_TRIANGLES,m_Zoomed_EBO_indices.size(),GL_UNSIGNED_INT,m_Zoomed_EBO_indices.data());

                glBindBuffer(GL_ARRAY_BUFFER,0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
           }
        }
        break;
    case Map2:
    {
        // 绑定VAO
        glBindVertexArray(m_VAO_Map[0][0]);
        if(glGetError() != GL_NO_ERROR)
            qDebug()<<"Running Bind VAO Failed!!!";
   
        glDrawElements(GL_TRIANGLES,m_Zoomed_EBO_indices.size(),GL_UNSIGNED_INT,m_Zoomed_EBO_indices.data());

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        break;
    }
    case Map3:
    {
        // 绑定VAO
        glBindVertexArray(m_VAO_Map[0][1]);
        if(glGetError() != GL_NO_ERROR)
            qDebug()<<"Running Bind VAO Failed!!!";
   
        glDrawElements(GL_TRIANGLES,m_Zoomed_EBO_indices.size(),GL_UNSIGNED_INT,m_Zoomed_EBO_indices.data());

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        break;
    }
    case Map4:
    {
        // 绑定VAO
        glBindVertexArray(m_VAO_Map[1][0]);
        if(glGetError() != GL_NO_ERROR)
            qDebug()<<"Running Bind VAO Failed!!!";
   
        glDrawElements(GL_TRIANGLES,m_Zoomed_EBO_indices.size(),GL_UNSIGNED_INT,m_Zoomed_EBO_indices.data());

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        break;
    }
    case Map5:
    {
        // 绑定VAO
        glBindVertexArray(m_VAO_Map[0][0]);
        if(glGetError() != GL_NO_ERROR)
            qDebug()<<"Running Bind VAO Failed!!!";
   
        glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_INT,m_Zoomed_EBO_indices.data());

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        
        break;
    }
    default:
        break;
    }

}

void MyOpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    float deltaTime=TIME_OUT_INTERVAL/1000.0;
    switch (event->key()) {
    case Qt::Key_Q:
        angle=(angle+5)%360;
        break;
    case Qt::Key_E:
        angle=(angle-5)%360;
        break;
    case Qt::Key_W:
        m_camera.processKeybord(FORWARD,deltaTime);
        break;
    case Qt::Key_S:
        m_camera.processKeybord(BACKWARD,deltaTime);
        break;
    case Qt::Key_D:
        m_camera.processKeybord(RIGHT,deltaTime);
        break;
    case Qt::Key_A:
        m_camera.processKeybord(LEFT,deltaTime);
        break;
    default:
        break;
    }

    update();
}

void MyOpenGLWidget::on_timeout()
{
    update();
}

void MyOpenGLWidget::InitOpenGLMap()
{  
    // Test 着色器程序初始化
    if(!m_ShaderProgram_Map.addShaderFromSourceFile(QOpenGLShader::Vertex,"./MyShapes.vert"))
    {
        qDebug()<<"Load my shader vs failed!!!";
    }
    if(!m_ShaderProgram_Map.addShaderFromSourceFile(QOpenGLShader::Fragment,"./MyShapes.frag"))
    {
        qDebug()<<"Load my shader fs failed!!!";
    }
    if(!m_ShaderProgram_Map.link())
    {
        qDebug()<<"My Shader Link failed!!!";
    }
    
    // 纹理初始化
    m_Texture_Map = new QOpenGLTexture(QImage("./map_test2.png").mirrored());

    // 设置纹理滤波方式为  MipMap纹理过滤（三线性过滤）
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    // glGenerateMipmap(GL_TEXTURE_2D);

    // 如着色器程序中有多个纹理，给纹理添加索引编号
    m_ShaderProgram_Map.setUniformValue("texture0",0);

    // 需先进行bind()才能进行赋值
    m_ShaderProgram_Map.bind();
    m_ShaderProgram_Map.setUniformValue("zoomratio",100.0f);


    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            // 创建VAO VBO EBO
            glGenVertexArrays(1,&m_VAO_Map[i][j]);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Create VAO Failed!!!"<<i<<j;

            glGenBuffers(1,&m_VBO_Map[i][j]);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Create VBO Failed!!!"<<i<<j;

            glGenBuffers(1,&m_EBO_Map);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Create EBO Failed!!!"<<i<<j;

            // 绑定VAO
            glBindVertexArray(m_VAO_Map[i][j]);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Bind VAO Failed!!!"<<i<<j;
            // 绑定VBO，并将数据送至缓冲
            glBindBuffer(GL_ARRAY_BUFFER,m_VBO_Map[i][j]);
            glBufferData(GL_ARRAY_BUFFER,m_Map_vertices[i][j].size() * sizeof(float),m_Map_vertices[i][j].data(),GL_STATIC_DRAW);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Bind VBO Failed!!!"<<i<<j;
            // 绑定EBO，并将EBO的index送至缓冲
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO_Map);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_Zoomed_EBO_indices.size() * sizeof(int),m_Zoomed_EBO_indices.data(),GL_STATIC_DRAW);
            if(glGetError() != GL_NO_ERROR)
                qDebug()<<"Bind EBO Failed!!!"<<i<<j;

            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
            // 设置位置属性
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(decltype(m_Zoomed_heightMap_Analyzed)::value_type),(void*)0);
            glEnableVertexAttribArray(0);

            // 设置纹理属性
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 5 * sizeof(decltype(m_Zoomed_heightMap_Analyzed)::value_type),(void*)(3*sizeof(decltype(m_Zoomed_heightMap_Analyzed)::value_type)));
            glEnableVertexAttribArray(1);

            // 解除绑定
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        }
    }

}

void MyOpenGLWidget::ZoomedPaddingHeightAnalyzed()
{
    std::vector<float>::iterator it = m_currentMapPiece[0][0].begin();
   
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            // 位置坐标
            m_Zoomed_heightMap_Analyzed.push_back(i);
            m_Zoomed_heightMap_Analyzed.push_back(*(it++));
            m_Zoomed_heightMap_Analyzed.push_back(-j);
            
            // 纹理坐标
            m_Zoomed_heightMap_Analyzed.push_back(i / 100.0);
            m_Zoomed_heightMap_Analyzed.push_back(j / 100.0);

        }
    }
    qDebug()<<m_Zoomed_heightMap_Analyzed.size();

    // uint32_t index = 0;
    // for(float val : m_Zoomed_heightMap_Analyzed)
    // {
    //     std::cout<<val<<"  ";

    //     index++;
    //     if(index%3 == 0)
    //         std::cout<<std::endl;
    // }
    
    
}

void MyOpenGLWidget::PaddingEBOIndices()
{
    for (size_t i = 0; i < 9899; i++)
    {   
        for (size_t j = 0; j < 2; j++)
        {
            if((i+1) % 100 == 0)
                continue;
            if(j == 0)
            {
                m_Zoomed_EBO_indices.push_back(i + j);
                m_Zoomed_EBO_indices.push_back(i + j + 1);
                m_Zoomed_EBO_indices.push_back(i + 100);
            }
            else
            {
                m_Zoomed_EBO_indices.push_back(i + j);
                m_Zoomed_EBO_indices.push_back(i + 101);
                m_Zoomed_EBO_indices.push_back(i + 100);
            }
        }
        
    }
    qDebug()<<"EBO indices: "<<m_Zoomed_EBO_indices.size();

    //  uint32_t index = 0;
    // for(uint32_t val : m_Zoomed_EBO_indices)
    // {
    //     std::cout<<val<<"  ";

    //     index++;
    //     if(index%3 == 0)
    //         std::cout<<std::endl;
    // }
    
}

void MyOpenGLWidget::GetPieceFromMapData(uint8_t row_index, uint8_t column_index, MapPiece_t& piece)
{
    if(m_MapFileData->GetPiece(row_index,column_index) != nullptr)
        piece = *m_MapFileData->GetPiece(row_index,column_index);

    // for(int i = 0; i < 100; i++)
    // {
    //     for(int j = 0; j < 100; j++)
    //     {
    //         std::cout<<piece.at(i+j)<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
}

void MyOpenGLWidget::SetPosAndTexcoord(uint8_t row_index,uint8_t column_index,const MapPiece_t& piece, MapPiece_t& vertices)
{
    MapPiece_t::const_iterator it = piece.begin();
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            vertices.push_back(i + row_index * 100 - row_index);
            //vertices.push_back(i );
            vertices.push_back(*(it++));
            vertices.push_back(-j - column_index * 100 + column_index);
            //vertices.push_back(-j);

            vertices.push_back(i / 100.0);
            vertices.push_back(j / 100.0);
        }
    }
    std::cout<<"row :"<<(int)row_index <<"column : "<<(int)column_index<<"size : "<<vertices.size() <<std::endl;

    // for(int i = 0; i < 10; i++)
    // {
    //     for(int j = 0; j < 50; j++)
    //     {  
    //         if(j % 5 == 0)
    //             std::cout<<"  ";
    //         std::cout<<vertices.at(i*100+j)<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
}


