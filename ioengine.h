#ifndef CIOENGINE_H
#define CIOENGINE_H

#include "bboxinput.h"

#define MODE_RESIZE_ORIGIN           0
#define MODE_RESIZE_WIDTH_AND_HEIGHT 1
#define MODE_RESIZE_WIDTH_OR_HEIGHT  2

class CIOEngine {
public:
    CBBoxInput               m_bbInput;

    cv::VideoCapture         m_cap;
    std::vector<std::string> m_imNames;
    int                      m_iFrame;
    std::string              m_inType;
    std::string              m_winName;

    cv::Mat                  m_frame;
    cv::Size                 m_szFrame     = cv::Size(640, 480);
    int                      m_resizeMode  = MODE_RESIZE_ORIGIN;

    int                      m_i_start =  0;
    int                      m_i_end   = -1;
    int                      m_i_frame = -1;

    void setResizeMode(int resizeMode = MODE_RESIZE_ORIGIN);

    bool initVideoEngine(std::string vidName, int iStart, int iEnd);
    bool initCameraEngine(int cam);
    bool initImagesEninge(std::string prefix, int start, int end, int l, std::string suffix);

    bool checkBB(int bb[4]);
    bool getFrame();
    bool getFrameFromVideo();
    bool getFrameFromImages();
    bool getBBox(int bb[4]);
    bool getBBoxCam(int bb[4]);
    bool makeNames(std::string prefix, int start, int end, int l, std::string suffix);

    CIOEngine();
    ~CIOEngine();
};

#endif // CIOENGINE_H
