
/*
 Rotational Antenna Tracking System Software
 Cornell Rocketry Team

 This program receives rocket telemetry, 
 aims the antenna at the rocket's location, 
 and passes the data on to the ground station laptop

 Created SP23 
 by Zach Garcia

 */

#include <Stepper.h>
#include <Servo.h>
#include <ArduinoJson.h>

// change this to fit the number of steps per revolution for the motor
const int motorStepsPerRevolution = 200;
const int platformStepsPerRev = 1280;

bool launched = false;
bool telemetry = false;
long launchTime = 0;

// initialize the stepper library on pins 14 through 17:
Stepper myStepper(motorStepsPerRevolution, 7, 8, 9, 10);

Servo myservo;

DynamicJsonDocument data(1024);

// roundabout
// long double lat1 = 32.984202;
// long double long1 = -106.975268;
// // spaceport america
// long double lat2 = 32.990509;
// long double long2 = -106.975575;
// // directly south
// long double lat3 = 32.988750;
// long double long3 = -106.985446;
// // directly west
// long double lat4 = 32.990509;
// long double long4 = -106.985567;
// // directly north
// long double lat5 = 32.997895;
// long double long5 = -106.975575;
// // directly east
// long double lat6 = 32.990509;
// long double long6 = -106.965575;
// two miles west
long double lat7 = 32.989786;
long double long7 = -107.024810;

long double elevRock = 10000;
long double elevGround = 0;

long double rockLat;
long double rockLong;

long double groundLat = 32.990509;
long double groundLong = -106.975575;

int stepPos = 0;
int num_messages = 0;

// Utility function for printing doubles
void printDouble( double val, unsigned int precision){

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.print(frac,DEC) ;
} 

// Utility function for
// converting degrees to radians
long double toRadians(const long double degree)
{
  long double one_deg = (M_PI) / 180;
  return (one_deg * degree);
}

// Utility function for converting radians to degrees
long double toDegrees(const long double radian)
{
  long double one_rad = 180 / (M_PI);
  return (one_rad * radian);
}

// calculates the distance between two lat/long points via the Haversine Formula
long double distance(long double lat1, long double long1,
                     long double lat2, long double long2)
{
  // Convert the latitudes and longitudes from degree to radians.
  lat1 = toRadians(lat1);
  long1 = toRadians(long1);
  lat2 = toRadians(lat2);
  long2 = toRadians(long2);

  // Haversine Formula
  long double dlong = long2 - long1;
  long double dlat = lat2 - lat1;
  long double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2);
  ans = 2 * asin(sqrt(ans));

  // Radius of Earth in Kilometers
  long double R = 6371;
  ans = ans * R;

  return ans;
}

long double bearing(long double lat1, long double long1, long double lat2, long double long2)
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

void setStepperIdle() {
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
}

void setup() {
  // attaches the servo on pin GI15 to the servo object
  myservo.attach(12, 500, 2500);
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  //TODO: receive telemetry
  rockLat = lat7;
  rockLong = long7;

  if (launched && launchTime == 0) {
    launchTime = millis();
  } else if (launched && !telemetry) {
    //switch to backup data
  }

  //convert coordinates to angles
  long double dist = distance(rockLat, rockLong, groundLat, groundLong);
  Serial.print("Distance: ");
  printDouble(dist, 15);
  Serial.println(" km");
  long double bear = bearing(rockLat, rockLong, groundLat, groundLong);
  Serial.print("Bearing: ");
  printDouble(bear, 15);
  Serial.println(" degrees");
  long double asc = ascension(elevRock, elevGround, dist);
  Serial.print("Ascension: ");
  printDouble(asc, 15);
  Serial.println(" degrees");
  int target = (bear / 360) * platformStepsPerRev;
  int stepsToTake;
  if (target - stepPos > (platformStepsPerRev / 2)) {
    stepsToTake = (target - stepPos) - platformStepsPerRev;
  } else if (target - stepPos < -(platformStepsPerRev / 2)) {
    stepsToTake = (target - stepPos) + platformStepsPerRev;
  } else {
    stepsToTake = target - stepPos;
  }
  stepPos = target;

  //aim antenna
  myservo.write(asc);
  // Serial.println("delaying");
  // delay(1000000);
  myStepper.step(stepsToTake);
  setStepperIdle();
  delay(500);
  Serial.println(stepsToTake);

  //package data in JSON
  data["latitude"] = 0.0;
  data["longitude"] = 0.0;
  data["elevation"] = 0.0;
  data["accel_x"] = 0.0;
  data["accel_y"] = 0.0;
  data["accel_z"] = 0.0;
  data["gyro_x"] = 0.0;
  data["gyro_y"] = 0.0;
  data["gyro_z"] = 0.0;
  data["mag_x"] = 0.0;
  data["mag_y"] = 0.0;
  data["mag_z"] = 0.0;
  data["temp"] = 0.0;
  for (int i = 0; i < num_messages; i++) {
    data["messages"][i] = "PLACEHOLDER";
  }

  //send data to laptop
  serializeJson(data, Serial);
  Serial.println();

  delay(1000);
}

