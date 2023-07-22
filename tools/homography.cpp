#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <ros/package.h>
#include "cv_bridge/cv_bridge.h"
#include <image_transport/image_transport.h>
int cnt = 0;
int cnt2 = 0;
cv::Mat master_frame,slave_frame;//原始图像
std::vector<cv::Point2f> master_src, master_dst;//六个角点坐标
std::vector<cv::Point2f> slave_src, slave_dst;
cv::Mat master_H,slave_H;//平面间的单应变换矩阵
std::string master_H_file,slave_H_file;
//Camera params 内部参数+畸变系数
cv::Mat master_instrinsic = (cv::Mat_<double>(3,3) << 552.715667942477, 0., 578.733089360504, 
                0.,553.031272490833, 509.815324025131, 
                0., 0., 1. );
cv::Mat master_disortion = (cv::Mat_<double>(1, 5) <<  -0.00202994240321718,-0.0144495139448957,0,0,0.00380485745278735);

cv::Mat slave_instrinsic = (cv::Mat_<double>(3,3) << 551.761226999728, 0, 625.026564672521,
                                                                    0,552.116053613643,491.015888823710,
                                                                    0,0,1);
cv::Mat slave_disortion = (cv::Mat_<double>(1, 5) << -0.003705270824610,-0.015134281146654,0,0,0.003768462291391);

void mouse_callback_1(int event,int x,int y,int flags,void *ustc)
{
    if (event == cv::EVENT_LBUTTONDOWN)//鼠标左键点击
    {
        master_dst.push_back(cv::Point2f(x, y));
        cnt++;
        if(cnt == 6)
        {
            //After 6 clicks， caculate the homography matrix
            master_H = cv::findHomography(master_dst, master_src);
        }
    }
}
void mouse_callback_2(int event,int x,int y,int flags,void *ustc)
{
    if (event == cv::EVENT_LBUTTONDOWN)
    {
        slave_dst.push_back(cv::Point2f(x, y));
        cnt2++;
        if(cnt2 == 6)
        {
            slave_H = cv::findHomography(slave_dst, slave_src);
        }
    }
}

// [0 1]
// [3 2]
std::vector<cv::Point> master_field_corners{cv::Point(0,32), cv::Point(808,32), cv::Point(808,480), cv::Point(0,480)};//场地4个角点的坐标
bool master_recalib = false;
std::vector<cv::Point2f> master_field(4);
std::vector<cv::Point2f> field_src{cv::Point(0,32), cv::Point(808,32), cv::Point(808,480), cv::Point(0,480)};
//The video quality may lead to a poor airview picture,here is a small tool to get a better matrix ？？？没有看懂原理
void mouse_callback_3(int event,int x,int y,int flags,void *ustc)
{
    switch(event)
    {
        case cv::EVENT_LBUTTONDOWN://鼠标左键按下事件
        //Click near the point that want to be recalculate
        {
            master_recalib = true;
            break;
        }
        case cv::EVENT_MOUSEMOVE://鼠标移动事件
        //Get the point coordinate on the pic
        {
            if(master_recalib == true)
            {
                int dis1 = std::sqrt(std::pow(master_field_corners[1].x-x, 2) + std::pow(master_field_corners[1].y-y, 2));
                int dis2 = std::sqrt(std::pow(master_field_corners[2].x-x, 2) + std::pow(master_field_corners[2].y-y, 2));
                if(dis1 < dis2)
                {
                    master_field_corners[1].x = x;
                    master_field_corners[1].y = y;
                }
                else
                {
                    master_field_corners[2].x = x;
                    master_field_corners[2].y = y;
                }
            }
            break;
        }
        case cv::EVENT_LBUTTONUP://鼠标左键释放事件
        {
            if(!master_H.empty())
            {
                for(int i = 0; i < 4; i++)
                {
                    //Recaculate the matrix
                    cv::Mat p = (cv::Mat_<double>(3,1) << master_field_corners[i].x, master_field_corners[i].y, 1);
                    p = master_H.inv() * p;
                    master_field[i] = cv::Point2f(p.at<double>(0,0)/p.at<double>(2,0), p.at<double>(1,0)/p.at<double>(2,0));
                }
                master_H = cv::findHomography(master_field, field_src);
                cv::FileStorage fs(master_H_file , cv::FileStorage::WRITE);//将重新计算后的单应矩阵，以YAML格式存储在目标文件中
                fs << "Homography" << master_H;
                fs.release();
                master_field_corners[1] = field_src[1];
                master_field_corners[2] = field_src[2];
            }
            master_recalib = false;
            break;
        }
        default:;
    }
}

std::vector<cv::Point> slave_field_corners{cv::Point(0,32), cv::Point(808,32), cv::Point(808,480), cv::Point(0,480)};
bool slave_recalib = false;
std::vector<cv::Point2f> slave_field(4);
void mouse_callback_4(int event,int x,int y,int flags,void *ustc)
{
    switch(event)
    {
        case cv::EVENT_LBUTTONDOWN:
        {
            slave_recalib = true;
            break;
        }
        case cv::EVENT_MOUSEMOVE:
        {
            if(slave_recalib == true)
            {
                int dis1 = std::sqrt(std::pow(slave_field_corners[1].x-x, 2)+std::pow(slave_field_corners[1].y-y, 2));
                int dis2 = std::sqrt(std::pow(slave_field_corners[2].x-x, 2)+std::pow(slave_field_corners[2].y-y, 2));
                if(dis1 < dis2)
                {
                    slave_field_corners[1].x = x;
                    slave_field_corners[1].y = y;
                }
                else
                {
                    slave_field_corners[2].x = x;
                    slave_field_corners[2].y = y;
                }
            }
            break;
        }
        case cv::EVENT_LBUTTONUP:
        {
            if(!slave_H.empty())
            {
                for(int i = 0; i < 4; i++)
                {
                    cv::Mat p = (cv::Mat_<double>(3,1) << slave_field_corners[i].x, slave_field_corners[i].y, 1);
                    p = slave_H.inv() * p;
                    slave_field[i] = cv::Point2f(p.at<double>(0,0)/p.at<double>(2,0), p.at<double>(1,0)/p.at<double>(2,0));
                }
                slave_H = cv::findHomography(slave_field, field_src);
                cv::FileStorage fs(slave_H_file , cv::FileStorage::WRITE);
                fs << "Homography" << slave_H;
                fs.release();
                slave_field_corners[1] = field_src[1];
                slave_field_corners[2] = field_src[2];
            }
            slave_recalib = false;
            break;
        }
        default:;
    }
}

void MastercompressedImageCallback(const sensor_msgs::CompressedImageConstPtr &msg)//将ROS图像信息转换为OpenCV图像
{
    cv_bridge::CvImagePtr cv_ptr_compressed = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);//opencv默认通道是bgr
    cv::Mat image = cv_ptr_compressed->image;
    master_frame = image.clone();
}
void SlavecompressedImageCallback(const sensor_msgs::CompressedImageConstPtr &msg)//将ROS图像信息转换为OpenCV图像
{
    cv_bridge::CvImagePtr cv_ptr_compressed = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv::Mat image = cv_ptr_compressed->image;
    slave_frame = image.clone();
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "calc_homography_node");
    ros::NodeHandle ros_nh_;
    int save_mode = 1;
    bool save_master_flag =false;
    bool save_slave_flag =false;
    //Here you can change the image topic 获得相机的原始图像，存储在 master_frame 中
    ros::Subscriber master_image_sub = ros_nh_.subscribe("/mvsua_cam/image_raw1/compressed",1,MastercompressedImageCallback);
    ros::Subscriber slave_image_sub = ros_nh_.subscribe("/mvsua_cam/image_raw2/compressed",1,SlavecompressedImageCallback);
    int master_sentry_id,slave_sentry_id;
    int color_flag;
    ros_nh_.getParam("color_flag", color_flag);
    //Set the sentry ID
    if(color_flag == 0)//红方设置为0
    {
        master_sentry_id = 4;
        slave_sentry_id = 2;
    }
    if(color_flag == 2)//蓝方设置为2，初始color_flag==2
    {
        master_sentry_id = 3;
        slave_sentry_id = 1;
    }
    cv::Mat master_undisort_frame,slave_undisort_frame;//畸变矫正后的图像
    cv::Mat master_warp_frame,slave_warp_frame;
    master_H_file = ros::package::getPath("tools")+"/homo_mat/master_H.yaml"; //单应变换矩阵H的文件路径
    slave_H_file = ros::package::getPath("tools")+"/homo_mat/slave_H.yaml";
    cv::namedWindow("MatsterCam");
    cv::setMouseCallback("MatsterCam", mouse_callback_1, 0);
    cv::namedWindow("SlaveCam");
    cv::setMouseCallback("SlaveCam", mouse_callback_2, 0);
    cv::namedWindow("Master_out");
    cv::setMouseCallback("Master_out", mouse_callback_3, 0);
    cv::namedWindow("Slave_out");
    cv::setMouseCallback("Slave_out", mouse_callback_4, 0);

    int point_num = 6;
    int pic_num = 0;
    //Set the reference point in the field 六个角点
    // [6 4 2]
    // [5 3 1]
    if(master_sentry_id==3 &&slave_sentry_id == 1)//蓝方机器人 D3=左上，D1=右下
    {
        master_src.push_back(cv::Point2f(808-170,480-100));
        master_src.push_back(cv::Point2f(808-100,480-(448-120)));
        master_src.push_back(cv::Point2f(354,480-93.5));
        master_src.push_back(cv::Point2f(354,480-(448-93.5))); 
        master_src.push_back(cv::Point2f(100,480-100));
        master_src.push_back(cv::Point2f(150,480-(448-100)));


        slave_src.push_back(cv::Point2f(808-170,480-100));
        slave_src.push_back(cv::Point2f(808-100,480-(448-120)));
        slave_src.push_back(cv::Point2f(354,480-93.5));
        slave_src.push_back(cv::Point2f(354,480-(448-93.5))); 
        slave_src.push_back(cv::Point2f(100,480-100));
        slave_src.push_back(cv::Point2f(150,480-(448-100)));
    }
    else if(master_sentry_id==4 &&slave_sentry_id == 2)//红方机器人 D4=左下，D1=右上
    {
        master_src.push_back(cv::Point2f(808-100,480-120));
        master_src.push_back(cv::Point2f(808-170,480-(448-100)));
        master_src.push_back(cv::Point2f(354,480-93.5));
        master_src.push_back(cv::Point2f(354,480-(448-93.5))); 
        master_src.push_back(cv::Point2f(150,480-100));
        master_src.push_back(cv::Point2f(100,480-(448-120))); 

        slave_src.push_back(cv::Point2f(808-100,480-120));
        slave_src.push_back(cv::Point2f(808-170,480-(448-100)));
        slave_src.push_back(cv::Point2f(354,480-93.5));
        slave_src.push_back(cv::Point2f(354,480-(448-93.5))); 
        slave_src.push_back(cv::Point2f(150,480-100));  
        slave_src.push_back(cv::Point2f(100,480-(448-120))); 

    }
    else{
        return 0;
    }
    
    while(ros::ok())
    {
        ros::spinOnce();
        if(save_master_flag && save_slave_flag)
        {
            return 0;
        }
        if(!master_frame.empty())//对图像进行畸变矫正
        {
            cv::undistort(master_frame, master_undisort_frame, master_instrinsic, master_disortion);
            master_warp_frame = master_undisort_frame.clone();
        }
        if(!slave_frame.empty())
        {
            cv::undistort(slave_frame, slave_undisort_frame, slave_instrinsic, slave_disortion);
            slave_warp_frame = slave_undisort_frame.clone();
        }
        // Get Pic From Camera 用圆标记出角点
        for(int i = 0;i < master_dst.size();i++)
            {
                cv::circle(master_undisort_frame, master_dst[i], 10, cv::Scalar(0, 255, 0),-1);
            }
        for(int i = 0;i < slave_dst.size();i++)
            {
                cv::circle(slave_undisort_frame, slave_dst[i], 10, cv::Scalar(0, 255, 0),-1);
            }
        // 将斜视图转为俯视图
        if(!master_H.empty())
        {
            cv::warpPerspective(master_warp_frame, master_warp_frame, master_H, cv::Size(900, 512));//透视变换
            for(int i = 0; i < 3; i++)
                cv::line(master_warp_frame, master_field_corners[i], master_field_corners[i+1], cv::Scalar(0,255,0), 1);//给场地边缘画出线框
            cv::imshow("Master_out", master_warp_frame);//展示单应变换俯视图
        }
        if(!slave_H.empty())
        {
            cv::warpPerspective(slave_warp_frame, slave_warp_frame, slave_H, cv::Size(900, 512));
            for(int i = 0; i < 3; i++)
                cv::line(slave_warp_frame, slave_field_corners[i], slave_field_corners[i+1], cv::Scalar(0,255,0), 1);
            cv::imshow("Slave_out",slave_warp_frame);
        }


        if(!master_frame.empty())
        {
            cv::imshow("MatsterCam",master_undisort_frame);//展示去畸变后的图像
        }
        if(!slave_frame.empty())
        {
            cv::imshow("SlaveCam",slave_undisort_frame);    
        }
        int key = cv::waitKey(1);
        //Push "q" to save a test pic
        if(key == 113)
        {
            cv::imwrite (ros::package::getPath("tools")+"/save_pic/"+std::to_string(pic_num)+"master.jpg",master_warp_frame);
            cv::imwrite (ros::package::getPath("tools")+"/save_pic/"+std::to_string(pic_num)+"slave.jpg",slave_warp_frame);
            pic_num++;
            std::cout<<"save_img"<<std::endl;
        }
       
    return 0;
    }
