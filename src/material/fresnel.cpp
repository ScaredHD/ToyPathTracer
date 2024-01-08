#include "fresnel.h"

#include "utils.h"

double FresnelDielectrics::reflectance(double cos_theta_in, double eta_out, double eta_in) const {
    clamp(cos_theta_in, -1.0, 1.0);

    // Swap eta_i and eta_t if incident from inside
    auto eta_i = eta_out;
    auto eta_t = eta_in;
    if (cos_theta_in < 0.0) {
        std::swap(eta_i, eta_t);
        cos_theta_in = -cos_theta_in;
    }

    // Calculate cos(theta_t)
    auto sin_theta_i = std::sqrt(1.0 - cos_theta_in * cos_theta_in);
    auto sin_theta_t = eta_i / eta_t * sin_theta_i;
    auto cos_theta_t = std::sqrt(1 - sin_theta_t * sin_theta_t);

    // Total internal reflection occurs
    if (sin_theta_t >= 1.0) {
        return 1.0;
    }

    auto n_para    = eta_t * cos_theta_in - eta_i * cos_theta_t;
    auto d_para    = eta_t * cos_theta_in + eta_i * cos_theta_t;
    auto para_comp = n_para / d_para;

    auto n_perp    = eta_i * cos_theta_in - eta_t * cos_theta_t;
    auto d_perp    = eta_i * cos_theta_in + eta_t * cos_theta_t;
    auto perp_comp = n_perp / d_perp;

    return 0.5 * (para_comp * para_comp + perp_comp * perp_comp);
}
