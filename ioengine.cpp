#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ioengine.h"

CIOEngine::CIOEngine() {
}

CIOEngine::~CIOEngine() {
    m_cap.release();
    m_frame.release();
}

void CIOEngine::setResizeMode(int resizeMode) {
    m_resizeMode = resizeMode;
}

bool CIOEngine::initVideoEngine(std::string vidPath, int iStart, int iEnd) {
    m_inType = "video";
    m_i_start = iStart;
    m_i_end   = iEnd;

    if (!m_cap.open(vidPath)) {
        m_inType = "none";
        m_cap.release();
        return false;
    }

    return true;
}

bool CIOEngine::initCameraEngine(int cam) {
    m_inType = "camera";
    if (!m_cap.open(cam)) {
        m_inType = "none";
        m_cap.release();
        return false;
    }
    return true;
}

bool CIOEngine::initImagesEninge(std::string prefix, int iStart, int iEnd, int l, std::string suffix) {
    m_inType = "images";
    if(!makeNames(prefix, iStart, iEnd, l, suffix)) {
        m_inType = "none";
        return false;
    }
    m_iFrame = 0;

    return true;
}

bool CIOEngine::getFrame() {
    m_i_frame++;
    if (m_inType == "video" || m_inType == "camera") {
        return getFrameFromVideo();
    } else if (m_inType == "images") {
        return getFrameFromImages();
    } else {
        std::cout<<"The source of frames is not supported."<<std::endl;
        return false;
    }
}

bool CIOEngine::getFrameFromVideo() {
    m_frame.release();

    if (m_cap.read(m_frame)) {
        if (m_resizeMode == MODE_RESIZE_WIDTH_OR_HEIGHT) {
            float scale = std::min(float(m_szFrame.width) / m_frame.cols, float(m_szFrame.height) / m_frame.rows);
            cv::resize(m_frame, m_frame,  cv::Size(m_frame.cols * scale, m_frame.rows * scale));
        } else if (m_resizeMode == MODE_RESIZE_WIDTH_AND_HEIGHT) {
            cv::resize(m_frame, m_frame,  m_szFrame);
        }

        std::cout<<"width: "<<m_frame.cols<<" height: "<<m_frame.rows<<std::endl;

        return true;
    } else {
        return false;
    }
}

bool CIOEngine::getFrameFromImages() {
    if (m_iFrame < m_imNames.size()) {
        m_frame.release();
        m_frame = cv::imread(m_imNames[m_iFrame++]);

        float scale = std::min(m_szFrame.width / m_frame.cols, m_szFrame.height / m_frame.rows);
        cv::resize(m_frame, m_frame,  cv::Size(m_frame.cols * scale, m_frame.rows * scale));

        return true;
    } else {
        return false;
    }
}

bool CIOEngine::getBBox(int bb[4]) {
    if (!m_frame.data) {
        std::cout<<"Not image to select target."<<std::endl;
        return false;
    }
    if (m_bbInput.start(m_frame, m_winName, bb)){
        if (!checkBB(bb)) {
            std::cout<<"Please select a valid target box to for tracking."<<std::endl;
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool CIOEngine::getBBoxCam(int bb[4]) {
    return true;
}

bool CIOEngine::makeNames(std::string prefix, int iStart, int iEnd, int l, std::string suffix) {
    if (iStart > iEnd) return false;
    for (int i=iStart; i<=iEnd; i++) {
        std::string nstr = std::to_string(i);
        std::string tmp(l - nstr.length(), '0');
        m_imNames.push_back(prefix + tmp + nstr + suffix);
    }

    return true;
}

bool CIOEngine::checkBB(int bb[]) {
    return (bb[2] - bb[0] > 5 && bb[3] - bb[1] > 5);
}

