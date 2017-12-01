#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
  pid.Init(0.4, 0.0, 2, 1000);

  PID pid_v;
  pid_v.Init(0.2, 0.0, 0.04, 0);

  h.onMessage([&pid, &pid_v](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          ////////////////////////steering PID control //////////////////////////////////////
          // update the p, i and d error
          pid.UpdateError(cte);

          //steer according to PID-controll
          steer_value = pid.TotalError();
          //make sure the values stay in possible bounds
          steer_value = std::min(steer_value, 1.0);
          steer_value = std::max(steer_value, -1.0);

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << " angle: " << angle << " speed: " << speed << std::endl;
          std::cout << "Kp: " << pid.Kp << " Kp error: " << -pid.Kp * pid.p_error << std::endl;
          std::cout << "Kd: " << pid.Kd << " Kd error: " << -pid.Kd * pid.d_error << std::endl;
          std::cout << "Ki: " << pid.Ki << " Ki error: " << -pid.Ki * pid.i_error << std::endl;

          ///////////////////////// speed PID control ///////////////////////////////////////
          double throttle = 0.3;
          double des_speed = 30;

          // update p, i, d error
          double v_err = speed - des_speed;
          pid_v.UpdateError(v_err);

          //speed with PID-control
          throttle = pid_v.TotalError();//-pid_v.Kp * pid_v.p_error - pid_v.Kd * pid_v.d_error - pid_v.Ki * pid_v.i_error;
          //make sure throttle stays in bounds
          throttle = std::min(throttle, 1.0);
          throttle = std::max(throttle, 0.0);

          //DEBUG
          std::cout << "throttle: " << throttle << " des_speed " << des_speed << " speed: " << speed << std::endl;
          std::cout << "Kp: " << pid_v.Kp << " speed Kp error: " << -pid_v.Kp * pid_v.p_error << std::endl;
          std::cout << "Kd: " << pid_v.Kd << " speed Kd error: " << -pid_v.Kd * pid_v.d_error << std::endl;
          std::cout << "Ki: " << pid_v.Ki << " speed Ki error: " << -pid_v.Ki * pid_v.i_error << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
