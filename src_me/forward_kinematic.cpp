//find kinematic matrix; from base robot arm to end
#include <main.h>

void forward_kinematic(double q[], Mat &T)
{
//TM700_DH_PARAMS
#define PI 3.141592654
    const double _PI_2 = 0.5 * M_PI;

    //Unit of measurement milimeter
    const double d1 = 145.1;
    const double a2 = 329.0;
    const double a3 = 311.5;
    const double d4 = -122.2;
    const double d5 = 106.0;
    const double d6 = 114.4;

    double c1, c2, c3, c4, c5, c6, s1, s2, s3, s4, s5, s6;
    double cp, sp;
    double DEG2RAD = 0.01745329252;
    for (int i = 0; i < 6; i++)
    {
        q[i] = q[i] * DEG2RAD;
    }
    c1 = cos(q[0]);
    s1 = sin(q[0]);
    c2 = cos(q[1] - _PI_2);
    s2 = sin(q[1] - _PI_2);
    c3 = cos(q[2]);
    s3 = sin(q[2]);
    c4 = cos(q[3] + _PI_2);
    s4 = sin(q[3] + _PI_2);
    c5 = cos(q[4]);
    s5 = sin(q[4]);
    c6 = cos(q[5]);
    s6 = sin(q[5]);
    cp = cos(q[1] + q[2] + q[3]);
    sp = sin(q[1] + q[2] + q[3]);

    T.at<double>(0, 0) = c1 * sp * s6 - s1 * s5 * c6 + c1 * cp * c5 * c6;
    T.at<double>(0, 1) = c1 * sp * c6 + s1 * s5 * s6 - c1 * cp * c5 * s6;
    T.at<double>(0, 2) = c1 * cp * s5 + s1 * c5;
    T.at<double>(0, 3) = c1 * (a2 * c2 + a3 * c2 * c3 - a3 * s2 * s3) - d4 * s1 + d6 * c5 * s1 + d5 * c1 * (c4 * (c2 * s3 + c3 * s2) + s4 * (c2 * c3 - s2 * s3)) + d6 * c1 * s5 * (c4 * (c2 * c3 - s2 * s3) - s4 * (c2 * s3 + c3 * s2));

    T.at<double>(1, 0) = s1 * sp * s6 + c1 * s5 * c6 + s1 * cp * c5 * c6;
    T.at<double>(1, 1) = s1 * sp * c6 - c1 * s5 * s6 - s1 * cp * c5 * s6;
    T.at<double>(1, 2) = s1 * cp * s5 - c1 * c5;
    T.at<double>(1, 3) = 1 * (a2 * c2 + a3 * c2 * c3 - a3 * s2 * s3) + d4 * c1 - d6 * c1 * c5 + d5 * s1 * (c4 * (c2 * s3 + c3 * s2) + s4 * (c2 * c3 - s2 * s3)) + d6 * s1 * s5 * (c4 * (c2 * c3 - s2 * s3) - s4 * (c2 * s3 + c3 * s2));

    T.at<double>(2, 0) = cp * s6 - sp * c5 * c6;
    T.at<double>(2, 1) = cp * c6 + sp * c5 * s6;
    T.at<double>(2, 2) = -sp * s5;
    T.at<double>(2, 3) = d1 - a2 * s2 + d5 * (c4 * (c2 * c3 - s2 * s3) - s4 * (c2 * s3 + c3 * s2)) - a3 * c2 * s3 - a3 * c3 * s2 - d6 * s5 * (c4 * (c2 * s3 + c3 * s2) + s4 * (c2 * c3 - s2 * s3));
}
