#include "display.h"
#include "utils.h"

string get_label_string(LABEL label)
{
    string res;
    switch (label)
    {
        case SOLIDER:
            res = "Solider";
            break;
        case WARSHIP:
            res = "Warship";
            break;
        case VEHICLE:
            res = "Vehicle";
            break;
        case TANK:
            res = "Tank";
            break;
        case MISSILE:
            res = "Missile";
            break;
        case WARPLANE:
            res = "Warplane";
            break;
        case RIFLE:
            res = "Rifle";
            break;
        case SUBMARINE:
            res = "Submarine";
            break;
        case CANNON:
            res = "Cannon";
            break;
        default:
            cerr << "invalid label" << endl;
            break;
    }

    return res;
}

void show_result(Mat img, float *pred, int top_k, bool wait_key, bool show_info)
{
    /*
     * img: 待显示图片
     * pred: 图片分类结果
     * top_k: 展示前top k分类结果
     * wait_key: 若为false, 是对视频进行播放, 设置相应帧率; 若为true, 则是对图片进行单独展示
     * show_info: 当对图片进行单独展示时, 绘制开始和结束提示
     *
     * */

    // check input image
    if (img.data == nullptr)
    {
        cerr << "Invalid input image!" << endl;
        return;
    }

    // check input scores
    float sum = 0.0f;
    int i;
    for (i = 0; i < NUM_CLASSES; i++)
        sum += pred[i];
    if (abs(sum - 1.0) > 0.00001)
    {
        cerr << "Wrong input prediction!" << endl;
        return;
    }

    int display_width_img = 640;
    float scale = img.cols * 1.0 / display_width_img;
    int display_height_img = img.rows / scale;
    Mat result(display_height_img, display_width_img / 2 * 3, img.type());

    // show start information
    if (show_info)
    {
        string start_info = "Recognizing object in image...";
        draw_info(result, start_info);

        imshow("", result);
        waitKey(2000);
    }

    // show resized image
    Mat resized(display_height_img, display_width_img, img.type());
    resize(img, resized, Size(display_width_img, display_height_img));

    // show classification result
    Mat cls = Mat::zeros(display_height_img, display_width_img / 2, img.type());
    vector<int> indices = argsort(vector<float>(pred, pred + NUM_CLASSES));
    // draw top-3 results
    int t;
    for (t = 0; t < top_k; t++)
    {
        string title_string = t == 0 ? "Top-" + to_string(top_k) + " Predictions" : "";
        draw_prob(cls, pred[indices[t]], (LABEL) indices[t], t, top_k, title_string);
    }

    // concat resized image and cls result image
    hconcat(resized, cls, result);
    imshow("", result);

    if (!wait_key)
        waitKey(1000 / 25);  // 1000ms / 25fps = 40ms
    else
        waitKey(3000);

    if (show_info)
    {
        string end_info = "Recognition done!";
        draw_info(result, end_info);

        imshow("", result);
        waitKey(2000);
    }
}

void draw_info(Mat &img, string info_str)
{
    int height = img.rows;
    int width = img.cols;
    img.setTo(Scalar(0, 0, 0));

    int font_face = FONT_HERSHEY_COMPLEX;
    double font_scale = 1;
    int thickness = 1;
    int baseline;
    cv::Size text_size = cv::getTextSize(info_str, font_face, font_scale, thickness, &baseline);

    int p_x = (width - text_size.width) / 2;
    int p_y = (height + text_size.height) / 2;

    putText(img, info_str, Point(p_x, p_y), font_face, font_scale, Scalar(255, 255, 255),
            thickness);
}

void draw_prob(Mat &img, float prob, LABEL label, int rank, int top_k, string title_str)
{
    int width = img.cols;
    int height = img.rows;

    int seg_height = height / 4;  // 将整张图分成四行, 在中间两行绘制条形图
    int bar_height = seg_height * 2 / top_k;  // 中间两个seg分成top_k行, 用于绘制topk的score
    int color_bar_height = bar_height / 10.0f * 9.0f;  // 9/10用来画条形图
    int split_bar_height = bar_height - color_bar_height;  // 剩余1/10用黑色像素填充, 用于分割两个相邻的bar

    int split_width = 50;  // 左右两幅图之间隔了10个px
    int cur_bar_width = prob * (width - split_width);  // 根据prob计算当前bar应该画多长
    int start_y = seg_height + rank * bar_height;
    int i;
    for (i = start_y; i < start_y + color_bar_height; i++)
    {
        int j;
        for (j = split_width; j < cur_bar_width + split_width; j++)
        {
            int index = (i * width + j) * 3;
            img.data[index] = B;
            img.data[index + 1] = G;
            img.data[index + 2] = R;
        }
    }

    string label_str = get_label_string(label);
    label_str += " (" + to_string(prob).substr(0, 4) + ")";

    int font_face = FONT_HERSHEY_COMPLEX;
    double font_scale = .6;
    int thickness = 1;
    int baseline;
    cv::Size text_size = cv::getTextSize(label_str, font_face, font_scale, thickness, &baseline);
    int line_height = text_size.height;
    int start_y_text = start_y + (color_bar_height + line_height) / 2;
    putText(img, label_str, Point(split_width, start_y_text), font_face, font_scale, Scalar(255, 255, 255), thickness);

    if (title_str.length() > 0)
    {
        putText(img, title_str, Point(split_width, seg_height / 3 * 2), font_face, font_scale, Scalar(255, 255, 255),
                thickness);
    }
}