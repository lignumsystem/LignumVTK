#ifndef CIESKY_H
#define CIESKY_H
#include <iostream>
#include <string>
#include <cmath>
#include <numbers>
#include <fstream>
#include <mathsym.h>
#include <LignumVTK.h>
///\file CIESky.h
/// \brief Implement CIE skies for ParaView visualization.

namespace lignumvtk{
  ///\brief  CIE overcast sky (Moon-Spencer)
  ///
  ///Moon-Spencer CIE overcast sky (SOC) discretized using independent, uniform angular
  ///step sizes for inclinations and azimuths.
  class CIESOC{
  public:
    ///\brief Constructor
    ///\param nazim Number of azimuths
    ///\param nincl Number of inclinations
    ///\param tot_rad Peak radiant intensity, i.e. total incoming radiation on a plane
    ///\post The radius of the hemisphere is 1.
    ///\note The hemisphere is divided using the *vtkSphereSource* API.
    ///\par Create Moon-Spencer CIE overcast sky (SOC) 
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
  
  ///\brief  CIE overcast sky (Moon-Spencer)
  ///
  ///Moon-Spencer CIE overcast sky (SOC) discretized using equal surface area sectors.
  ///\note CIESOCEqualArea maps analogously to sky::Firmament.
  ///\sa sky::Firmament
  class CIESOCEqualArea{
  public:
    ///\brief Constructor
    ///\param nazim Number of azimuths
    ///\param nincl Number of inclinations
    ///\param tot_rad Peak radiant intensity, i.e. total incoming radiation at zenith (plane sensor).
    ///\post The radius of the hemisphere is 1.
    ///\post The radiant intensity is assgined to the middle point of a hemisphere sector.
    ///\par Create Moon-Spencer CIE overcast sky (SOC) with equal area sectors
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
    ///    &=& R^2(\int_{0}^{2\pi} d\phi)(\int_{0}^{\theta_k}\sin\theta d\theta) \\
    ///    &=& 2\pi R^2\left[-\cos\theta\right]_{0}^{\theta_k} \\
    ///    &=& 2\pi R^2(-\cos\theta_k -(-cos(0))) \\
    ///    &=& 2\pi R^2(1-\cos\theta_k)
    ///    \f}
    /// + Full hemisphere has an area \f$A_{\mathit{tot}} = 2\pi R^2(1-cos(0)) =  2\pi R^2 \f$.
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
    ///\brief Write \p polydata to a VTP file
    ///\param file_name File name
    ///\retval EXIT_SUCCESS Write success
    ///\retval EXIT_FAILURE Write failure
    int writeHemisphere(const string& file_name);
  private:
    vtkNew<vtkPolyData> polydata;///< Contains *vtkQuads* of the hemisphere
    vtkNew<vtkXMLPolyDataWriter> writer;///< VTP file writer for \p polydata
  };
}

#endif
