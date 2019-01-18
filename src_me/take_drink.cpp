#include <main.h>

int take_drink(string drink, TmDriver &TmRobot, Mat &position_end, string &line)
{
    if (drink == "tea")
    {
        //take tea
        line = "cmd42 0 25 38.25 40.93 9 89.99 17.92";
        TmRobot.setCommandMsg(line);
        sleep(6);

        // grip_tea();
        line = "cmd41 0 653 140 220 180 0 90";
        TmRobot.setCommandMsg(line);
        sleep(2);
    }
    else if (drink == "coffee")
    {
        //take coffee
        line = "cmd42 0 -1.6 41.56 33.22 14.66 89.84 0";
        TmRobot.setCommandMsg(line);
        sleep(6);

        // grip_coffee
        line = "cmd41 0 653 -140 220 180 0 90";
        TmRobot.setCommandMsg(line);
        sleep(2);
    }
    else
    {
        cout << "Cannot find QRcode of tea or coffee" << endl;
        return 0;
    }

    // grip_0N
    line = "digop 8 1 0";
    TmRobot.setCommandMsg(line);
    sleep(2);
    // steady
    line = "cmd42 0 90 20 20 50 90 0";
    TmRobot.setCommandMsg(line);
    sleep(7);

    // buffer
    line = "cmd41 0";
    for (int d = 0; d < 2; d++)
    {
        double str = position_end.at<double>(d, 0);
        auto x_str = std::to_string(str);
        line.append(" ");
        line.append(x_str);
    }

    line.append(" 320 180 0 180");
    string line_tem;
    line_tem = line;
    TmRobot.setCommandMsg(line);
    sleep(5);

    // release
    line = "cmd40 0";
    for (int d = 0; d < 3; d++)
    {
        double str = position_end.at<double>(d, 0);
        auto x_str = std::to_string(str);
        line.append(" ");
        line.append(x_str);
    }
    line.append(" 180 0 180");
    TmRobot.setCommandMsg(line);
    sleep(3);

    // grip_off
    line = "digop 8 0 0";
    TmRobot.setCommandMsg(line);
    sleep(2);
    // release tem
    TmRobot.setCommandMsg(line_tem);
    sleep(2);
    // after_grip(position_end)
    line = "cmd42 0 107.73 -0.517 63.83 26.79 89.85 17.9";
    TmRobot.setCommandMsg(line);
    sleep(3);
}