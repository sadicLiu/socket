#ifndef DEMO_DISPLAY_H
#define DEMO_DISPLAY_H

#include <iostream>
#include <opencv2/opencv.hpp>

#define NUM_CLASSES 9

#define R 77
#define G 171
#define B 82

#define TOPK 5


using namespace std;
using namespace cv;

// 9分类
typedef enum
{
    SOLIDER = 0,
    WARSHIP,
    VEHICLE,
    TANK,
    MISSILE,
    WARPLANE,
    RIFLE,
    SUBMARINE,
    CANNON
} LABEL;

void show_result(Mat img, float *preds, int top_k, bool wait_key, bool show_info = false);

void draw_info(Mat &img, string info_str);

void draw_prob(Mat &img, float prob, LABEL label, int rank, int top_k, string title_str);

#endif //DEMO_DISPLAY_H
