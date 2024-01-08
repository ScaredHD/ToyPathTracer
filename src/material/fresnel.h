#pragma once

class Fresnel {
  public:
    virtual ~Fresnel() = default;
    virtual double reflectance(double cos_theta_in, double eta_out, double eta_in) const = 0;
};

class FresnelDielectrics : public Fresnel {
  public:
    /// @brief Return the Fresnel reflectance for certain incident light ray. Both sides of the
    /// surface being hit are dielectrics. When total internal reflection happens, the reflectance
    /// is 1.0
    /// @param cos_theta_in The cosine value of angle between incident direction and surface normal.
    /// This value could be negative. In this case, light comes from the inside of the surface
    /// @param eta_out Refractive index of outside medium where surface normal points to
    /// @param eta_in Refractive index of inside medium
    double reflectance(double cos_theta_in, double eta_out, double eta_in) const override;
};