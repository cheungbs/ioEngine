#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "bboxinput.h"

void drawRect(cv::Mat& frame, int bb[], std::string win_name) {
    cv::rectangle(frame, cv::Point(bb[0], bb[1]), cv::Point(bb[2], bb[3]), cv::Scalar(255, 0, 0));
    cv::imshow(win_name, frame);
};

// The definitions of the methods of class CBBoxInput
bool         CBBoxInput::m_bDragStart;
int          CBBoxInput::m_dragStart[2];
int          CBBoxInput::m_sel[4];
std::string  CBBoxInput::m_winName;
cv::Mat*     CBBoxInput::m_pFrame;

CBBoxInput::CBBoxInput() {
    m_winName = "frame";
    m_sel[0] = 0, m_sel[1] = 0, m_sel[2] = 0, m_sel[3] = 0;
    m_bDragStart = false;
    m_pFrame = nullptr;
}

CBBoxInput::~CBBoxInput() {
}

bool CBBoxInput::start(cv::Mat& frame, std::string show_name, int bb[4]) {
    m_bDragStart = false;
    m_sel[0] = 0, m_sel[1] = 0, m_sel[2] = 0, m_sel[3] = 0;

    m_pFrame = &frame;
    m_winName = show_name;

    cv::namedWindow(m_winName);
    cv::setMouseCallback(m_winName, onMouse);
    cv::imshow(m_winName, *m_pFrame);

    int key = cv::waitKey();
    switch(key & 255) {
    case 27:
        cv::destroyWindow(m_winName);
        break;
    default:
        end_bb();
        break;
    }

    bb[0] = m_sel[0];
    bb[1] = m_sel[1];
    bb[2] = m_sel[2];
    bb[3] = m_sel[3];

    return true;
}

int CBBoxInput::end_bb() {
    cv::setMouseCallback(m_winName, callEmpty);
    return 0;
}

void CBBoxInput::callEmpty(int event, int x, int y, int flags, void* param) {
}

void CBBoxInput::onMouse(int event, int x, int y, int flags, void* param) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        m_bDragStart = true;
        m_dragStart[0] = x, m_dragStart[1] = y;
        m_sel[0] = 0, m_sel[1] = 0, m_sel[2] = 0, m_sel[3] = 0;
    } else if (event == cv::EVENT_LBUTTONUP) {
        select_end();
    } else if (m_bDragStart){
        if (flags & cv::EVENT_FLAG_LBUTTON) {
            select_change(x, y);
        } else {
            std::cout<<"Selection is complete."<<std::endl;
            m_bDragStart = false;
        }
    }
}

int CBBoxInput::select_change(int x, int y) {
    int minpos[2], maxpos[2];
    minpos[0] = MIN(m_dragStart[0], x), minpos[1] = MIN(m_dragStart[1], y);
    maxpos[0] = MAX(m_dragStart[0], x), maxpos[1] = MAX(m_dragStart[1], y);
    m_sel[0] = minpos[0], m_sel[1] = minpos[1], m_sel[2] = maxpos[0], m_sel[3] = maxpos[1];

    cv::Mat img = m_pFrame->clone();
    drawRect(img, m_sel, m_winName);

    img.release();

    return 0;
}

int CBBoxInput::select_end() {
    m_bDragStart = false;

    return 0;
}

