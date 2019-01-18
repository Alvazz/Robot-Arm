#include <src_me/main.h>


/******************main**************************/
int main(int argc, char **argv)
{

  //initialize camera
  nRet = is_InitCamera(&hCam, NULL);
  cout << "Status Init: " << nRet << endl;
  //color mode
  Mode = IS_CM_RGB8_PACKED;
  nRet = is_SetColorMode(hCam, Mode);
  // cout << "Color Mo de: " << nRet << endl;

  UINT formatID = 13;
  nRet = is_ImageFormat(hCam, IMGFRMT_CMD_SET_FORMAT, &formatID, 4);
  cout << "Status Image Format: " << nRet << endl;

  char *pMem = NULL;
  int memID = 0;
  nRet = is_AllocImageMem(hCam, img_width, img_height, 24, &pMem, &memID);
  nRet = is_SetImageMem(hCam, pMem, memID);

  //set display mode
  Mode = IS_SET_DM_DIB;
  nRet = (hCam, Mode);

  //zoom
  // double dZoomValue = 1;
  // nRet = is_Zoom(hCam, ZOOM_CMD_DIGITAL_SET_VALUE, (void*)&dZoomValue, sizeof(dZoomValue));
  // cout << "Zoom: " << dZoomValue << endl;

  //robot arm
  bool fgRun = false;
  std::string host;
  std::condition_variable data_cv;
  std::condition_variable data_cv_rt;

  for (int i = 0; i < argc; i++)
  {
    printf("[DEBUG] arg%d:= %s\n", i, argv[i]);
  }
  host = argv[1];
  printf("[ INFO] host: %s", host.c_str());

  TmDriver TmRobot(data_cv, data_cv_rt, host, 0);

  if (!fgRun)
  {
    print_info("start...");
    fgRun = TmRobot.interface->start();
  }

  //initialize value robot arm
  /* Rotation and Translation from camera to end-effector of robot arm*/
  cv::Mat RT_matrix_endeffector2camera(4, 4, cv::DataType<double>::type, double(0));
  RT_matrix_endeffector2camera.at<double>(0, 0) = -1;
  RT_matrix_endeffector2camera.at<double>(1, 1) = -1;
  RT_matrix_endeffector2camera.at<double>(2, 2) = 1;
  RT_matrix_endeffector2camera.at<double>(1, 3) = 55;
  RT_matrix_endeffector2camera.at<double>(3, 3) = 1;
  cout << "RT_matrix_endeffector2camera: \n"
       << RT_matrix_endeffector2camera << endl;

  string line;
  string start;
  string line_tem;

  double q_inv[6] = {107.73, -0.517, 63.83, 26.79, 89.85, 17.9};
  line = "cmd42 0 107.73 -0.517 63.83 26.79 89.85 17.9";
  cv::Mat forward_kinematic_matrix(3, 4, cv::DataType<double>::type); //forwald matrix of kinematic
  forward_kinematic(q_inv, forward_kinematic_matrix);
  // cout<<"forward kinematic (base -> end effector): \n"<<forward_kinematic_matrix<<endl;

  // Rotation translation in axis-angle form
  cv::Mat rotation_vector(3, 1, cv::DataType<double>::type);
  cv::Mat translation_vector(3, 1, cv::DataType<double>::type);
  cv::Mat translation_matrix(4, 1, cv::DataType<double>::type);
  cv::Mat rotation_matrix(3, 3, cv::DataType<double>::type); //rodrigues rotation matrix
  cv::Mat position_end(3, 1, cv::DataType<double>::type);

  int x = 0;
  int y = 0;
  int n = 0;

  while (fgRun)
  {
    if (is_FreezeVideo(hCam, IS_WAIT) == IS_SUCCESS)
    {
      is_CaptureVideo(hCam, 10);
      cout << "[Debug] into IS_success " << endl;

      void *pMemVoid; //pointer to where the image is stored
      is_GetImageMem(hCam, &pMemVoid);
      //take pictures
      Mat img = Mat(Size(img_width, img_height), CV_8UC3, pMemVoid);
      // read QR code in picture
      // Variable for decoded objects
      start = "cmdc0";
      string gripp = "digop 8 0 0";
      TmRobot.setCommandMsg(gripp);
      TmRobot.setCommandMsg(start);
      if (line != line_tem) //check robot arm at line
      {
        TmRobot.setCommandMsg(line);
        cout << "sleep" << endl;
        sleep(7);
        cout << "end sleep" << endl;
      }
      vector<decodedObject> decodedObjects;
      vector<decodedObject> decodedObjects_tem;
      vector<Point2i> central;
      vector<Point2i> central_new;
      vector<Point2d> QRcode_location;

      // get most QRcode in 10 scans
      while (n == 0) //if don't have QRcode
      {
        decode_most_qrcode(n, img, decodedObjects, central);
        decodedObjects_tem = decodedObjects;
      }
      cout << "[m_debug] central_1:" << central.size() << endl;
      cout << "[m_debug] decodeOjs_tem_1:" << decodedObjects_tem.size() << "\t[m_debug] decodeOjs_1:" << decodedObjects.size() << endl;
      // display(img, decodedObjects_tem,x,y);

      for (uint i = 0; i < decodedObjects_tem.size(); i++)
      {
        cout << "[m_debug] into running" << endl;

        //take drink: take decodeObjects_tem move
        QRcode_location = decodedObjects_tem[i].location;
        solvePnP_extrinsic(QRcode_location, rotation_vector, translation_matrix, x, y);
        // cout<<"rotation vector: \n"<< rotation_vector<<endl;

        // position_end 3x1 = kinematic matrix 3x4 . Rotation Translation of end-effector to camera 4x4 . Translation matrix 4x1
        position_end = forward_kinematic_matrix * RT_matrix_endeffector2camera * translation_matrix;
        position_end.at<double>(2, 0) += 190; //length of the gripping
        cout << "position_end: \t" << decodedObjects_tem[i].data << "\n"
             << position_end << endl;

        //control robot
        string drink = decodedObjects_tem[i].data;
        cout << "provice: " << drink << endl;
        take_drink(drink, TmRobot, position_end, line);
        line_tem = line; // to avoid running the command to start position

        //delete 1 element
        central.erase(central.begin());

        //check QRcode again
        decode_most_qrcode(n, img, decodedObjects, central_new);

        cout << "[m_debug] number QRcode after check again: \t" << n << endl;
        cout << "[m_debug] decodeOjs_tem_2:" << decodedObjects_tem.size() << "\t[m_debug] decodeOjs_2:" << decodedObjects.size() << endl;
        cout << "[m_debug] central new_2:" << central_new.size() << "\t[m_debug] central_2:" << central.size() << endl;
        // Is there any QRcode comparison?
        int c, c_new;
        c = central.size();
        c_new = central_new.size();
        if (c < c_new)
        {
          vector<int> change_position; // variable tem
          cout << "[m_debug] into if" << endl;
          //find position change
          compare_central(central, central_new, change_position);

          cout << "[m_debug] change position" << change_position.size() << endl;
          if (change_position.size() != 0)
          {
            for (int i = 0; i < change_position.size(); i++) //adding placements to assign
            {
              decodedObjects_tem.push_back(decodedObjects[change_position[i]]);
              cout << "[m_debug] change position\n";
            }
            //update for central
            central.clear();
            for (int i = 0; i < decodedObjects_tem.size(); i++)
            {
              int x = 0, y = 0;
              for (int j = 0; j < 4; j++)
              {
                x += decodedObjects_tem[i].location[j].x;
                y += decodedObjects_tem[i].location[j].y;
              }
              x = x / 4;
              y = y / 4;
              central.push_back(Point(x, y));
            }
          }
          cout << "[m_debug] decodeOjs_tem end" << decodedObjects_tem.size() << endl;
          cout << "[m_debug] decodeOjs end\t" << decodedObjects.size() << endl;
          /*
            Compare the center of each central and central_new
            Find out the latest in the decodeObject
            Get the insertion at the end of decodeOject_tem
          */
        }
      }
      if ((int)waitKey(10) >= 0)
      {
        break;
      }
      // if (n==1) break;
    }
  }
  is_FreeImageMem(hCam, 1, MemID);
  is_ExitCamera(hCam);
  cv::destroyAllWindows();
}