#ifndef BBOXINPUT_H
#define BBOXINPUT_H

class CBBoxInput {
public:
    static bool         m_bDragStart;
    static int          m_dragStart[2];
    static int          m_sel[4];
    static std::string  m_winName;
    static cv::Mat*     m_pFrame;

    CBBoxInput();
    ~CBBoxInput();

    bool start(cv::Mat& frame, std::string show_name, int bb[4]);
    int end_bb();

    static void callEmpty(int event, int x, int y, int flags, void* param);
    static void onMouse(int event, int x, int y, int flags, void* param);
    static int select_change(int x, int y);
    static int select_end();
};

#endif // BBOXINPUT_H

