//find extrinsic using solvePnP
#include <main.h>

void solvePnP_extrinsic(vector<Point2d> &QRcode_location, Mat &rotation_vector, Mat &translation_matrix, int &dx, int &dy)
{
    Point2d focal_length = cv::Point2d(652.7396, 653.3803);                                                            //FocalLength
    Point2d center = cv::Point2d(321.8104, 259.7124);                                                                  //PrincipalPoint
    cv::Mat intrinsic = (cv::Mat_<double>(3, 3) << focal_length.x, 0, center.x, 0, focal_length.y, center.y, 0, 0, 1); //camera matrix
    cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);                                            // Assuming no lens distortion

    for (int i = 0; i < 4; i++)
    {
        double xx, yy;
        if (QRcode_location[i].x < dx)
            xx = QRcode_location[i].x;
        else
            xx = dx;
        if (QRcode_location[i].y < dy)
            yy = QRcode_location[i].y;
        else
            yy = dy;
        double x = abs(QRcode_location[i].x - dx) / 2 + xx;
        double y = abs(QRcode_location[i].y - dy) / 2 + yy;
        QRcode_location.push_back(Point(x, y));
    }
    // cout<<"Point 2D befor: \n"<<QRcode_location<<endl;
    //read location 3D
    vector<cv::Point3d> Point_QR_3D;
    Point_QR_3D.push_back(cv::Point3d(-50.0f, 50.0f, 0.0f));  //top left
    Point_QR_3D.push_back(cv::Point3d(-50.0f, -50.0f, 0.0f)); //down left
    Point_QR_3D.push_back(cv::Point3d(50.0f, -50.0f, 0.0f));  // down right
    Point_QR_3D.push_back(cv::Point3d(50.0f, 50.0f, 0.0f));   //top right
    Point_QR_3D.push_back(cv::Point3d(-25.0f, 25.0f, 0.0f));  //middle left
    Point_QR_3D.push_back(cv::Point3d(-25.0f, -25.0f, 0.0f)); //middle left
    Point_QR_3D.push_back(cv::Point3d(25.0f, -25.0f, 0.0f));  // middle right
    Point_QR_3D.push_back(cv::Point3d(25.0f, 25.0f, 0.0f));   //middle right

    // cout<<"\nPoint 3D befor: \n"<<Point_QR_3D<<endl;
    Mat translation_vector;
    cv::solvePnP(Point_QR_3D, QRcode_location, intrinsic, dist_coeffs, rotation_vector, translation_vector);

    // cv::Rodrigues(rotation_vector,rotation_matrix); //trans Rotate vec -> rotate matrix
    // cout<<"translation vector (world -> camera): \n"<< translation_vector<<endl;

    translation_matrix.at<double>(0) = translation_vector.at<double>(0);
    translation_matrix.at<double>(1) = translation_vector.at<double>(1);
    translation_matrix.at<double>(2) = translation_vector.at<double>(2);
    translation_matrix.at<double>(3) = 1.0;
    // cout<<"translation vector (world -> camera): \n"<< translation_matrix<<endl;
}