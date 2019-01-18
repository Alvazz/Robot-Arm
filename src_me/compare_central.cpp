//find position change
#include <main.h>


void compare_central(vector<Point2i> central, vector<Point2i> central_new, vector<int> &change_position)
{
  for (int i = 0; i < central_new.size(); i++)
  {
    cout << "[m_debug] into for of compare" << endl;
    int xx = 0, yy = 0;
    bool flast = true;
    for (int j = 0; j < central.size(); j++)
    {
      cout << "[m_debug] into for of for" << endl;
      xx = central[i].x - central_new[j].x;
      yy = central[i].y - central_new[j].y;
      if (abs(xx) < 5 && abs(yy) < 5)
      {
        cout << "[m_debug] into if of for for" << endl;
        flast = false;
      }
    }
    if (true)
    {
      cout << "[m_debug] into if push_back" << endl;
      change_position.push_back(i);
    }
  }
  cout << "[m_debug] done compare central" << endl;
}