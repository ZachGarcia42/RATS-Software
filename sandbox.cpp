// C++ program to calculate Distance
// Between Two Points on Earth
#include <bits/stdc++.h>
using namespace std;

// Utility function for
// converting degrees to radians
long double toRadians(const long double degree)
{
  // cmath library in C++
  // defines the constant
  // M_PI as the value of
  // pi accurate to 1e-30
  long double one_deg = (M_PI) / 180;
  return (one_deg * degree);
}

long double toDegrees(const long double radian)
{
  long double one_rad = 180 / (M_PI);
  return (one_rad * radian);
}

long double distance(long double lat1, long double long1,
                     long double lat2, long double long2)
{
  // Convert the latitudes
  // and longitudes
  // from degree to radians.
  lat1 = toRadians(lat1);
  long1 = toRadians(long1);
  lat2 = toRadians(lat2);
  long2 = toRadians(long2);

  // Haversine Formula
  long double dlong = long2 - long1;
  long double dlat = lat2 - lat1;

  long double ans = pow(sin(dlat / 2), 2) +
                    cos(lat1) * cos(lat2) *
                        pow(sin(dlong / 2), 2);

  ans = 2 * asin(sqrt(ans));

  // Radius of Earth in
  // Kilometers, R = 6371
  // Use R = 3956 for miles
  long double R = 6371;

  // Calculate the result
  ans = ans * R;

  return ans;
}

long double bearing(long double lat1, long double long1,
                    long double lat2, long double long2)
{
  lat1 = toRadians(lat1);
  long1 = toRadians(long1);
  lat2 = toRadians(lat2);
  long2 = toRadians(long2);

  // Bearing formula
  long double del_L = long2 - long1;
  long double x = cos(lat2) * sin(del_L);
  long double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(del_L);
  long double bearing = atan2(x, y);
  return toDegrees(bearing);
}

long double ascension(long double elev_rocket, long double elev_gs, long double dist)
{
  long double y = elev_rocket - elev_gs;
  return toDegrees(atan(y / (dist * 1000)));
}

// Driver Code
int main()
{
  // roundabout
  long double lat1 = 32.984202;
  long double long1 = -106.975268;
  // spaceport america
  long double lat2 = 32.990509;
  long double long2 = -106.975575;
  // directly south
  long double lat3 = 32.988750;
  long double long3 = -106.985446;
  // directly west
  long double lat4 = 32.990509;
  long double long4 = -106.985567;
  // directly north
  long double lat5 = 32.997895;
  long double long5 = -106.975575;
  // directly east
  long double lat6 = 32.990509;
  long double long6 = -106.965575;
  // two miles west
  long double lat7 = 32.989786;
  long double long7 = -107.024810;

  long double elev1 = 10000;
  long double elev2 = 0;

  // call the distance function
  long double dist = distance(lat7, long7,
                              lat2, long2);
  cout << setprecision(15) << fixed;
  cout << dist
       << " km\n";
  cout
      << bearing(lat7, long7,
                 lat2, long2)
      << " degrees\n";
  cout << ascension(elev1, elev2, dist) << " degrees";

  return 0;
}

// This code is contributed
// by Aayush Chaturvedi
