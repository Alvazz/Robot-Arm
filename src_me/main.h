//library
  #include "src/tm_driver/tm_driver.h"
  #include "src_me/"
  #include <iostream>
  #include <fstream>
  #include "uEye.h"
  #include "zbar.h"
  #include "time.h"
  #include "opencv2/highgui.hpp"
  #include "opencv2/imgproc.hpp"
  #include "opencv2/core.hpp"
  #include "opencv2/videoio.hpp"
  #include "opencv2/opencv.hpp"
  #include <opencv2/core/core.hpp>
  #include <opencv2/highgui/highgui.hpp>

  using namespace zbar;
  using namespace std;
  using namespace cv;

// innit variable
HIDS hCam = 1;
SENSORINFO sensor_info;
CAMINFO camera_info;
int nRet;
int Mode;
char strCamFileName[256];
int img_width = 640;
int img_height = 480;

int h = 17;
int counter = 0;

typedef struct
{
  string type;
  string data;
  vector<Point2d> location;
} decodedObject;

void decode(int &n, Mat im, vector<decodedObject> &decodedObjects);
void display(Mat im, vector<decodedObject> &decodedObjects, int x, int y);
void solvePnP_extrinsic(vector<Point2d> &QRcode_location, Mat &rotation_vector, Mat &translation_matrix, int &dx, int &dy);
int take_drink(string drink, TmDriver &TmRobot, Mat &position_end, string &line);
void decode_most_qrcode(int &n, Mat &img, vector<decodedObject> &decodedObjects, vector<Point2i> &central);

