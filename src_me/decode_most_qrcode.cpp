/*
  Most qrcode
  Return QRcode and centrol of all QRcode
*/

#include <main.h>

void decode_most_qrcode(int &n, Mat &img, vector<decodedObject> &decodedObjects, vector<Point2i> &central)
{
    int n_tem = 0;
    vector<decodedObject> decodedObjects_tem;
    //find all QRcode
    for (int i = 0; i < 10; i++)
    {
        decodedObjects.clear();
        decode(n, img, decodedObjects);
        if (n > n_tem)
        {
            //delete objects
            decodedObjects_tem.clear();
            decodedObjects_tem = decodedObjects;
            n_tem = n;
        }
    }
    decodedObjects = decodedObjects_tem;
    decodedObjects_tem.clear();
    //calculter centrol of all QRcode
    for (int i = 0; i < decodedObjects.size(); i++)
    {
        int x = 0, y = 0;
        for (int j = 0; j < 4; j++)
        {
            x += decodedObjects[i].location[j].x;
            y += decodedObjects[i].location[j].y;
        }
        x = x / 4;
        y = y / 4;
        central.push_back(Point(x, y));
    }
    cout << "[m_debug] done decode_most_qrcode \n";
}
