#include "merl.h"

#include <cmath>
#include <fstream>

// cross product of two vectors
void cross_product(const double v1[3], const double v2[3], double (&out)[3]) {
  out[0] = v1[1] * v2[2] - v1[2] * v2[1];
  out[1] = v1[2] * v2[0] - v1[0] * v2[2];
  out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// normalize vector
void normalize(double v[3]) {
  // normalize
  double len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] = v[0] / len;
  v[1] = v[1] / len;
  v[2] = v[2] / len;
}

// rotate vector along one axis
void rotate_vector(const double vector[3], const double axis[3], double angle,
                   double (&out)[3]) {
  double temp;
  double cross[3];
  double cos_ang = std::cos(angle);
  double sin_ang = std::sin(angle);

  out[0] = vector[0] * cos_ang;
  out[1] = vector[1] * cos_ang;
  out[2] = vector[2] * cos_ang;

  temp = axis[0] * vector[0] + axis[1] * vector[1] + axis[2] * vector[2];
  temp = temp * (1.0 - cos_ang);

  out[0] += axis[0] * temp;
  out[1] += axis[1] * temp;
  out[2] += axis[2] * temp;

  cross_product(axis, vector, cross);

  out[0] += cross[0] * sin_ang;
  out[1] += cross[1] * sin_ang;
  out[2] += cross[2] * sin_ang;
}

void half_angle_to_std(double theta_h, double theta_d, double phi_d,
                       powitacq_rgb::Vector3f &wi_v,
                       powitacq_rgb::Vector3f &wo_v) {
  double wi_z = std::cos(theta_d);
  double wi_xy = std::sin(theta_d);
  double wi_x = wi_xy * std::cos(phi_d);
  double wi_y = wi_xy * std::sin(phi_d);
  double wi[] = {wi_x, wi_y, wi_z};
  double wo[] = {-wi_x, -wi_y, wi_z};
  normalize(wi);
  normalize(wo);

  double bi_normal[] = {0.0, 1.0, 0.0};
  double normal[] = {0.0, 0.0, 1.0};
  double wi_o[3], wo_o[3];

  rotate_vector(wi, bi_normal, theta_h, wi_o);
  rotate_vector(wo, bi_normal, theta_h, wo_o);

  wi_v = powitacq_rgb::Vector3f(wi_o[0], wi_o[1], wi_o[2]);
  wo_v = powitacq_rgb::Vector3f(wo_o[0], wo_o[1], wo_o[2]);
}

void save_merl(const std::vector<double> &x, const std::string &path) {
  std::ofstream out_file(path, std::ios::out | std::ios::binary);

  int32_t merl_shape[] = {90, 90, 180};

  out_file.write(reinterpret_cast<const char *>(merl_shape),
                 sizeof(merl_shape));

  out_file.write(reinterpret_cast<const char *>(x.data()),
                 x.size() * sizeof(double));
}
