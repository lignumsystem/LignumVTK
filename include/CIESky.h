#ifndef CIESKY_H
#define CIESKY_H
#include <iostream>
#include <string>
#include <cmath>
#include <numbers>
#include <map>
#include <fstream>
#include <mathsym.h>
#include <LignumVTK.h>
///\file CIESky.h
/// \brief CIE skies for ParaView visualization.
///
/// \par CIE Standard General Sky
///
/// The CIE Standard General Sky, developed by S. Darula, R. Kittler and R. Perez, is a globally
/// recognized mathematical model. It uses five parameters to define the relative
/// luminance and radiance distributions of the sky. Adjusting these parameters yields
/// the 15 CIE Standard General Sky types, which characterize atmospheres from heavily
/// overcast to clear, from turbid to transparent. Formally defined under *ISO 15469:2004 / CIE S 011/E:2003*,
/// these sky types establish a unified framework for luminance and radiance models,
/// replacing earlier discrete versions.
/// \sa lignumvtk::CIESGS::relativeRadiance(double,double)const
///
/// However, the CIE Standard Overcast Sky (Moon and Spencer) remains a vital legacy metric,
/// classified as CIE Standard General Sky Type I.1 and commonly known as Sky Type 16.
/// It is defined in *CIE S 017:2020 (ILV, term 17-29-111)* as the ratio:
/// \f{eqnarray*}
///   \frac{L_e\gamma}{L_ez} = (1 + 2\sin\gamma) / 3
/// \f}
/// where \f$ L_e\gamma \f$ is the radiance of a sky sector in the direction at an angle
/// \f$ \gamma \f$ above horizon and \f$ L_ez \f$ the radiance at the zenith. 
///
/// \par Radiometric terms
///
/// Imprecise use of radiometric terms often leads to ambiguity. The following table presents the five core concepts.
///
/// | Quantity          | Symbol         | SI Unit                        | Definition       | Informally |
/// | :---------------- | :------------: | :----------------------------- | :--------------- |:----------------------| 
/// | Radiant energy    | \f$ Q_e \f$    | \f$ J \f$                      | Total energy travelling in waves (Joules)                    | Total output        |
/// | Radiant flux      | \f$ \Phi_e \f$ | \f$ W \f$                      | Power, total energy emitted per second (Watt = J/s)          | Speed of energy     |
/// | Radiant intensity | \f$ I_e \f$    | \f$ W\mathit{sr}^{-1} \f$      | Power emitted in a specific direction (Watts per steradian)  | Directional power   | 
/// | Irradiance        | \f$ E_e \f$    | \f$ Wm^{-2} \f$                | Power landing on a flat surface  (Watts per square meter)    | Incoming power      |
/// | Radiance          | \f$ L_e \f$    | \f$ Wm^{-2}\mathit{sr}^{-1} \f$| Power emitted from a surface area in a specific direction (Watts per square meter per steradian)| Visual brightness |
///
/// The subscript *e* (for *energetic*) is often used in litterature to distinguish from photometric terms.
/// A radian is the SI unit for measuring two-dimensional plane angles, while a steradian is the SI unit for
/// measuring three-dimensional solid angles.

namespace lignumvtk{
  ///\brief Available CIE sky paramters
  enum class CIENAMES {ISO2004,DK2002};
  ///\brief CIE Standard General Sky ISO 2004 Standard parameters
  ///
  /// -# CIE Standard Overcast Sky, steep gradation
  /// -# Overcast, steep gradation, slight brightening towards the sun
  /// -# Overcast, moderately graded, azimuthal uniformity
  /// -# Overcast, moderately graded, slight brightening towards the sun
  /// -# Invariably uniform cloudy sky, no vertical gradation or azimuthal variation
  /// -# Partly cloudy, slight brightening towards the sun
  /// -# Partly cloudy, distinctive corona effect near the sun
  /// -# Partly cloudy, no vertical gradation, sharp solar corona
  /// -# Partly cloudy, moderately graded with a distinct solar corona
  /// -# Partly cloudy, moderately graded, bright intense corona
  /// -# White-blue clear sky, low horizon gradation, sharp solar corona
  /// -# CIE Standard Clear Sky, low-turbid clear atmosphere
  /// -# CIE Standard Clear Sky, polluted atmosphere with wide solar corona
  /// -# Clear sky, highly polluted/turbid atmosphere, immense solar corona
  /// -# Very clear sky, crisp clean atmosphere with a blindingly sharp solar corona
  const std::map<int, std::vector<double>> CIE_SKY_ISO_2004_STANDARD_PARAMETERS = {
    {1,  { 4.0, -0.7,   0.0, -1.0, 0.0  }},
    {2,  { 4.0, -0.7,   2.0, -1.5, 0.15 }},
    {3,  { 1.1, -0.8,   0.0, -1.0, 0.0  }},
    {4,  { 1.1, -0.8,   2.0, -1.5, 0.15 }},
    {5,  { 0.0, -1.0,   0.0, -1.0, 0.0  }},
    {6,  { 2.5, -0.15,  2.0, -1.5, 0.15 }},
    {7,  { 2.5, -0.15,  5.0, -2.2, 0.30 }},
    {8,  { 2.5, -0.15, 10.0, -3.0, 0.45 }},
    {9,  { 0.5, -0.55,  2.0, -1.5, 0.15 }},
    {10, { 0.5, -0.55,  5.0, -2.2, 0.30 }},
    {11, { 0.5, -0.55, 10.0, -3.0, 0.45 }},
    {12, {-1.0, -0.32,  5.0, -2.2, 0.30 }},
    {13, {-1.0, -0.32, 10.0, -3.0, 0.45 }},
    {14, {-1.0, -0.32, 16.0, -3.0, 0.30 }},
    {15, {-1.0, -0.32, 24.0, -2.8, 0.15 }}
  };
  
  ///\brief CIE Standard General Sky Darula & Kittler 2002 parameters
  ///
  /// -# Standard Overcast Sky, steep vertical gradation
  /// -# Overcast, steep gradation, with slight solar brightening
  /// -# Overcast, moderately graded, azimuthally uniform
  /// -# Overcast, moderately graded with slight brightening towards the sun
  /// -# Uniform cloudy sky, sky of uniform radiance
  /// -# Partly cloudy, no vertical gradation, slight solar brightening
  /// -# Partly cloudy, no vertical gradation, brighter solar corona region
  /// -# Partly cloudy, no vertical gradation, distinct solar corona
  /// -# Partly cloudy, with the obscured sun
  /// -# Partly cloudy, with brighter circumsolar region
  /// -# White-blue sky with distinct solar corona
  /// -# CIE Standard Clear Sky, clear blue sky with low turbidity
  /// -# CIE Standard Clear Sky, wide scattering glare, polluted atmosphere
  /// -# Cloudless turbid clear sky with broad solar corona
  /// -# White-blue turbid sky with broad solar corona
  const std::map<int, std::vector<double>> DARULA_KITTLER_2002_PARAMETERS = {
    {1,  { 4.0, -0.70,   0.0, -1.0, 0.0  }},
    {2,  { 4.0, -0.70,   2.0, -1.5, 0.15 }},
    {3,  { 1.1, -0.80,   0.0, -1.0, 0.0  }},
    {4,  { 1.1, -0.80,   2.0, -1.5, 0.15 }},
    {5,  { 0.0, -1.00,   0.0, -1.0, 0.0  }},
    {6,  { 2.0, -0.15,   2.0, -1.5, 0.15 }}, // Note: 'a' is 2.0 instead of 2.5
    {7,  { 2.5, -0.15,   5.0, -2.2, 0.30 }},
    {8,  { 2.5, -0.15,  10.0, -3.0, 0.45 }},
    {9,  { -1.0, -0.55,  2.0, -1.5, 0.15 }}, // Note: 'a' is -1.0 instead of 0.5
    {10, { -1.0, -0.55,  5.0, -2.2, 0.30 }}, // Note: 'a' is -1.0 instead of 0.5
    {11, { -1.0, -0.55, 10.0, -3.0, 0.45 }}, // Note: 'a' is -1.0 instead of 0.5
    {12, { -1.0, -0.32,  5.0, -2.2, 0.30 }},
    {13, { -1.0, -0.32, 10.0, -3.0, 0.45 }},
    {14, { -1.0, -0.32, 16.0, -3.0, 0.30 }},
    {15, { -1.0, -0.32, 24.0, -2.8, 0.15 }}
  };
  
  ///\brief Selection of available CIE sky parameters 
  const std::vector<std::map<int, std::vector<double>>> CIE_PARAMETERS ={CIE_SKY_ISO_2004_STANDARD_PARAMETERS, DARULA_KITTLER_2002_PARAMETERS};
  ///\brief Polar coordinates to cartesian coordinates.
  ///
  ///The function signature and return values follow the VTK style.
  ///\param[in] theta Polar angle
  ///\param[in] phi Azimuth angle
  ///\param[out] cartesian Cartesian (x,y,z) coordinates
  ///\pre Unit sphere, radius \f$ r = 1 \f$.
  inline void SphericalToCartesian(double theta, double phi, double cartesian[3])
  {
    double x = std::sin(theta)*std::cos(phi);
    double y = std::sin(theta)*std::sin(phi);
    double z = std::cos(theta);
    cartesian[0] = x; cartesian[1] = y; cartesian[2]= z;
  }

  ///\brief Cartesian coordinates to polar coordinates.
  ///
  ///The function signature and return values follow the VTK style. 
  ///\param[in] cartesian Cartesian (x,y,z) coordinates
  ///\param[out] theta Polar angle
  ///\param[out] phi Azimuth angle
  ///\pre Unit sphere, radius \f$ r = 1 \f$.
  ///\post \p theta value including the correct quadrant defined by *std::atan2*
  ///\note Undefined if x = y = 0.
  inline void CartesianToSpherical(const double cartesian[3], double& theta, double& phi)
  {
    theta = std::atan2(cartesian[1],cartesian[0]);
    phi = std::acos(cartesian[2]);
  }
  
  ///\brief Write CIE sky to a VTK/VTP file
  class CIEFile{
  public:
    ///\brief Write \p polydata to a VTK/VTP file
    ///\param file_name File name
    ///\retval EXIT_SUCCESS Write success
    ///\retval EXIT_FAILURE Write failure
    int writeHemisphere(const string& file_name);
  protected:
    vtkNew<vtkPolyData> polydata;///< Contains *vtkQuads* of the hemisphere
    vtkNew<vtkXMLPolyDataWriter> writer;///< VTP file writer for \p polydata
  };
  
  ///\brief  Traditional CIE Overcast Sky (Moon-Spencer)
  ///
  ///Moon-Spencer Traditional CIE Overcast Sky (SOC) discretized using independent, uniform angular
  ///step sizes for inclinations and azimuths.
  class CIESOC{
  public:
    ///\brief Constructor
    ///\param nazim Number of azimuths
    ///\param nincl Number of inclinations
    ///\param tot_rad Peak radiant intensity, i.e. total incoming radiation on a plane
    ///\post The radius of the hemisphere is 1.
    ///\note The hemisphere is divided using the *vtkSphereSource* API.
    ///\par Create Moon-Spencer Traditional CIE overcast sky (SOC) 
    ///
    ///Breakdown the hemisphere geometry to sectors with equal step size using *vtkSphereSource* API
    ///and assign radiant intensity to each sector. 
    ///
    ///\par Hemisphere breakdown
    ///
    ///Vertical divisions:
    /// + Total vertical span \f$ \left[0,\pi/2 \right] \f$ with \p nincl  divisions
    /// + Step size inclination: \f$\Delta\theta = (\theta_2 - \theta_1)/\mathrm{nincl} \f$
    /// + 1 zenith, and \p nincl - 1 sectors map the vertical span.
    ///.
    ///Horizintal divisions:
    /// + Total horizontal span \f$\ \left[0,2\pi\right] \f$ with \p nazim  divisions
    /// + Step size azimuth:  \f$\Delta\phi = (\phi_2 - \phi_1)/\mathrm{nazim} \f$
    ///.
    ///\par Radiant intensity
    ///
    ///Radiant intensity depends only on the polar angle:
    /// + \f$ I_s = I_z(\frac{1+2\cos\theta}{3}) \f$.
    /// + \f$ I_s \f$: The radiant intensity of a sector.
    /// + \f$ I_z \f$: The peak radiant intensity at zenith.
    /// + \f$ \theta \in \left[0,\pi/2\right] \f$: The polar angle measured from zenith downwards.
    ///.
    CIESOC(int nazim, int nincl, double tot_rad);
    ///\brief Write \p hemisphere to a VTP file
    ///\param file_name File name
    ///\retval EXIT_SUCCESS Write success
    ///\retval EXIT_FAILURE Write failure
    int writeHemisphere(const std::string& file_name);
  private:
    vtkNew<vtkSphereSource> hemisphere;///< Built-in VTK sphere configured as hemisphere 
    vtkNew<vtkXMLPolyDataWriter> writer;///< VTP file writer for \p hemisphere
  };
  
  ///\brief  Traditional CIE Overcast Sky (Moon-Spencer)
  ///
  ///Moon-Spencer Traditional CIE Overcast Sky (SOC) discretized using equal surface area sectors.
  ///\note CIESOCEqualArea maps analogously to sky::Firmament.
  ///\sa sky::Firmament
  class CIESOCEqualArea: public CIEFile{
  public:
    ///\brief Constructor
    ///\param nazim Number of azimuths
    ///\param nincl Number of inclinations
    ///\param tot_rad Peak radiant intensity, i.e. total incoming radiation at zenith (plane sensor).
    ///\post The radius of the hemisphere is 1.
    ///\post The radiant intensity is assgined to the middle point of a hemisphere sector.
    ///\par Create Moon-Spencer Traditional CIE overcast sky (SOC) with equal area sectors
    ///
    ///Breakdown the hemisphere geometry to equal area sectors and
    ///assign radiant intensity to each sector. Each sector is represented as *vtkQuad*.
    ///
    ///\par Hemisphere breakdown
    ///
    ///+ Inclinations: The boundary angle formula for \f$ N \f$ equal area horizontal zone bands:
    ///  \f$ \theta = \arccos(1-\frac{k}{N}), k=0,1,...,N \f$
    ///+ Azimuthal: Span of \f$\left[0,2\pi\right]\f$ azimuth divisions
    ///.
    ///\par Radiant intensity
    ///
    ///The radiant intensity of a hemisphere sector depends only on the inclination angle:
    /// + \f$ I_s = I_z(\frac{1+2\cos\theta}{3})\f$
    /// + \f$ I_s \f$: The radiant intensity of the sector \e s.
    /// + \f$ I_z \f$: The peak radiant intensity at zenith.
    /// + \f$ \theta \f$: The polar angle
    ///.
    ///\par Mathematical details for the boundary angle formula
    ///
    ///Define:
    /// + \f$ \theta \in \left[0,\pi/2\right ] \f$: The polar angle measured from the pole downwards.
    /// + \f$ \phi \in [0,2\pi] \f$: The azimuthal angle.
    ///
    ///Define:
    /// + \f$ Rd\theta \f$: The arc length resulting from a change in \f$ \theta \f$.
    /// + \f$ R\sin\theta d\phi \f$: The arc length resulting from a change in \f$ \phi \f$,
    ///       scaled by \f$ R\sin\theta \f$, the radius of the parallel circle at that latitude.
    /// + The spherical surface area element: \f$dA = R^2\sin\theta d\theta d\phi\f$.
    ///.
    ///Integrate to find the curved surface area for the \f$ k \f$-th sector:
    /// + From \f$0\f$ to \f$ \theta_k \f$ and from \f$0\f$ to \f$ 2\pi \f$:
    ///    \f{eqnarray*}{
    ///    A(\phi_k) &=& \int_{0}^{2\pi} \int_{0}^{\theta_k} R^2\sin\theta d\theta d\phi\\
    ///    &=& R^2\left(\int_{0}^{2\pi} d\phi\right)\left(\int_{0}^{\theta_k}\sin\theta d\theta\right) \\
    ///    &=& 2\pi R^2\left[ -\cos\theta \right]_{0}^{\theta_k} \\
    ///    &=& 2\pi R^2(-\cos\theta_k -(-cos(0))) \\
    ///    &=& 2\pi R^2(1-\cos\theta_k)
    ///    \f}
    /// +  A hemisphere has a curved surface area \f$A_{\mathit{tot}} = 2\pi R^2(1-cos(0)) =  2\pi R^2 \f$.
    ///.
    ///Boundary angle formula:
    /// + The area up to \f$ k \f$-th sector boundary must contain exactly \f$ k \f$-shares of \f$A_{\mathit{tot}}\f$:
    ///    \f$ A(\phi_k) = \frac{k}{N}A_{\mathit{tot}} \f$.
    /// + Substitute: \f$2\pi R^2(1-\cos\theta_k) = \frac{k}{N}2\pi R^2 \f$.
    /// + Cancel out the common  \f$2\pi R^2\f$ and rearrange: \f$ \cos\theta_k = 1-\frac{k}{N} \f$.
    /// + Take the inverse cosine both sides to solve for the boundary angle formula:
    ///   \f$ \theta_k = \arccos(1-\frac{k}{N}) \f$.\f$\fbox{\phantom{.}}\f$
    ///.
    CIESOCEqualArea(int nazim, int nincl, double tot_rad);
  };

  ///\brief CIE Standard General Sky (Darula-Kittler-Perez)
  class CIESGS: public CIEFile{
  public:
    ///\brief Constructor.
    ///
    ///Create any of the 15 CIE standard general skies.
    ///Calculate the scalar vectors:
    /// + Relative radiance for each sector
    /// + Absolute radiance for each sector
    ///.
    ///Store the scalar vectors in \p polydata for visualization.
    ///\param nazim Number of azimuths
    ///\param nincl Number of inclinations
    ///\param a Gradation: horizon brightness 
    ///\param b Gradation: rate of radiance change
    ///\param c Indicatrix: solar corona intensity
    ///\param d Indicatrix: solar corona width
    ///\param e Indicatrix: atmospheric backscattering
    ///\param sun_polar Sun polar angle, radians
    ///\param sun_azim Sun azimuth angle, radians
    ///\param Le_z Peak radiance at zenith.
    ///\note To convert \f$ d \f$ degrees to radians: \f$ \mathit{rad} = d\frac{\pi}{180^\circ} \f$ 
    CIESGS(int nazim, int nincl, double a, double b, double c, double d, double e, double sun_polar, double sun_azim, double Le_z);
    ///\brief Relative radiance function.
    ///
    ///Calculate relative radiance for a sky sector. 
    ///\param theta Polar angle \f$ \theta \f$ of the sky sector, radians
    ///\param chi Angular distance \f$ \chi \f$ between the sun and the sky sector, radians
    ///\retval Le_rel Relative radiance \f$ L_e\mathit{rel} \f$ of a sky sector, unitless
    ///
    ///A sky sector center point is defined by two parameters \p theta and \p chi.
    ///Relative radiance is the ratio of sky sector radiance \f$ L_e\mathit{s} \f$ to zenith radiance \f$ L_e\mathit{z} \f$.
    ///It is defined by the scattering indicatrix \f$ f \f$ and radiance gradation \f$ \psi \f$ functions.
    ///\f{eqnarray*}
    ///  L_e\mathit{rel} = \frac{ L_e\mathit{s} }{ L_e\mathit{z} } = \frac{ f(\chi)\psi(\theta) }{ f(\theta_\mathit{sun})\psi(0) }
    ///\f}
    ///where \f$ \theta_\mathit{sun} \f$ is the sun polar angle. 
    ///\sa CIESGS::gradation()
    ///\sa CIESGS::indicatrix()
    double relativeRadiance(double theta, double chi)const;
    ///\brief Radiance gradation function.
    ///
    ///Scale of continuous changes in radiance from polar to horizon.
    ///\param theta Polar angle \f$ \theta \f$ of the sky sector, radians
    ///\return Radiance gradation
    ///
    ///The gradation function \f$ \psi \f$ relates radiance of a sky and its polar angle:
    ///\f{eqnarray*}
    /// \psi(\theta) = 1 + a\exp(b/cos\theta)
    ///\f}
    ///where \f$ a \f$ and \f$ b \f$ are parameters.
    ///\sa CIESGS::a
    ///\sa CIESGS::b
    double gradation(double theta)const;
    ///\brief Scattering indicatrix function.
    ///
    ///Glare and light scattering relative to the sun's position \p chi
    ///\param chi Distance \f$ \chi \f$ between the sun and the sky sector, radians
    ///\return Scattering indicatrix
    ///
    ///The scattering indicatrix function \f$ f \f$ relates relative radiance of a sky sector to its angular
    ///distance from the sun:
    ///\f{eqnarray*}
    /// f(\chi) = 1+c(\exp(d\chi)-exp(d\pi/2))+e\cos^2\chi
    ///\f}
    ///where \f$ c, d \f$ and \f$ e \f$ are parameters.
    ///\note The angular distance \f$ \chi \f$ between the sun and sector \f$ s \f$
    ///can be expressed with the dot product: \f$ \chi = arccos(\vec{v}_{\mathit{sun}} \cdot \vec{v}_s) \f$
    ///\sa CIESGS::c
    ///\sa CIESGS::d
    ///\sa CIESGS::e
    double indicatrix(double chi)const;
  private:
    int N;   ///< Number of hemisphere sectors
    double a;///< Horizon brightness relative to the rest of the sky. CIESGS::gradation() parameter
    double b;///< Rate of radiance change between the horizon and the zenith, CIESGS::gradation() function
    double c;///< The solar corona intensity, CIESGS::indicatrix() parameter
    double d;///< The solar corona glare width, CIESGS::indicatrix() parameter
    double e;///< Backscattering or second-order reflections of radiation. Diffuse atmospheric component, CIESGS::indicatrix() parameter
    double Le_z;///< Peak radiance at zenith.
    double sun_polar;///< Sun polar angle, radians
    double sun_azimuth;///< Sun azimuth angle, radians
    double sun_cartesian[3];///< Sun position, cartesian coordinates
  };
    
}

#endif
