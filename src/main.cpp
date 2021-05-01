#include <iostream>
#include <string>

#include "merl.h"
#include "powitacq_rgb.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: main [Input RGL file] [Output MERL file]" << std::endl;

    return 1;
  }

  const std::string rgl_file = std::string(argv[1]);
  const std::string merl_file = std::string(argv[2]);

  std::clog << "--------------" << std::endl
            << "RGL file: " << rgl_file << std::endl
            << "MERL file: " << merl_file << std::endl
            << "--------------" << std::endl;

  powitacq_rgb::BRDF rgl(rgl_file);
  std::vector<double> merl(3 * 90 * 90 * 180);

  for (size_t th = 0; th < 90; ++th) {
    double theta_h = static_cast<double>(th) / 90.0;
    theta_h = theta_h * theta_h * M_PI_2;
    for (size_t td = 0; td < 90; ++td) {
      double theta_d = static_cast<double>(td) / 90.0 * M_PI_2;
      for (size_t pd = 0; pd < 180; ++pd) {
        double phi_d = static_cast<double>(pd) / 180.0 * M_PI;

        powitacq_rgb::Vector3f wi, wo;

        half_angle_to_std(theta_h, theta_d, phi_d, wi, wo);

        powitacq_rgb::Vector3f rgb = rgl.eval(wo, wi);

        constexpr size_t k_stride = 90 * 90 * 180;
        const size_t idx = th * 90 * 180 + td * 180 + pd;

        merl[idx] = rgb.x() / wi.z() * 1500.0;
        merl[idx + k_stride] = rgb.y() / wi.z() * 1500.0 / 1.15;
        merl[idx + k_stride + k_stride] = rgb.z() / wi.z() * 1500.00 / 1.66;
      }
    }
  }

  save_merl(merl, merl_file);

  return 0;
}
