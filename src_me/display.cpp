// Display barcode and QR code location
#include <main.h>

void display(Mat im, vector<decodedObject> &decodedObjects, int x, int y)
{
    cout << "[Debug] start display" << endl;
    // Loop over all decoded objects
    for (uint j = 0; j < decodedObjects.size(); j++)
    {
        vector<Point2d> points = decodedObjects[j].location;
        // cout<<points<<endl;

        for (int i = 0; i < 4; i++)
        {
            char str[10];
            sprintf(str, "(%d,%d)", int(points[i].x), int(points[i].y));
            putText(im, str, Point2f(points[i].x, points[i].y), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255, 255), 1);
            circle(im, Point2f(points[i].x, points[i].y), 3, Scalar(0, 0, 255), CV_FILLED, 8, 0);
        }
        //show location x,y on image
        char str[4];
        sprintf(str, "x = %d", x);
        putText(im, str, Point2f(30, 30), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255, 255), 2);
        sprintf(str, "y = %d", y);
        putText(im, str, Point2f(30, 45), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255, 255), 2);
    }

    // Display results
    namedWindow("Results", CV_WINDOW_AUTOSIZE);
    imshow("Results", im);
    // waitKey(0);
    cout << "[Debug] stop display" << endl;
}
