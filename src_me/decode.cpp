// Find and decode QR codes
#include <main.h>

void decode(int &n, Mat im, vector<decodedObject> &decodedObjects)
{
    int x;
    int y;
    // imshow("Results", im);
    cout << "[Debug] start decode" << endl;
    // Create zbar scanner
    ImageScanner scanner;

    // Configure scanner
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    Mat imGray;
    cvtColor(im, imGray, CV_BGR2GRAY);

    // Wrap image data in a zbar image
    Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

    // Scan number the image for barcodes and QRCodes
    n = scanner.scan(image);
    cout << "Number QR code: \t" << n << endl;

    // Print results
    for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
    {

        decodedObject obj;

        obj.data = symbol->get_data();
        cout << "Data: " << obj.data << endl;

        // Obtain location
        for (int i = 0; i < symbol->get_location_size(); i++)
        {
            obj.location.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));

            x += symbol->get_location_x(i);
            y += symbol->get_location_y(i);
            // circle(im, Point(symbol->get_location_x(i),symbol->get_location_y(i)),3, Scalar(0,0,255),CV_FILLED, 8,0);
        }
        //central of QRcode
        x = x / 4;
        y = y / 4;
        //draw a circle in QR code
        circle(im, Point(x, y), 3, Scalar(0, 0, 255), CV_FILLED, 8, 0);

        decodedObjects.push_back(obj);
    }

    cout << "[Debug] stop decode" << endl;
}