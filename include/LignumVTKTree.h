///\file LignumVTKTree.h
///Convert Lignum tree to VTK/VTPC file format for Paraview 
#ifndef LIGNUMVTK_TREE_H
#define LIGNUMVTK_TREE_H
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <Kite.h>
#include <Lignum.h>
#include <LignumVTK.h>

namespace lignumvtk{

  class TreeSegmentDataCollection;
  class VTKBud;
  class VTKCfBud;
  
  ///\brief VKTHwSegment tree segment for LignumVKT project.
  ///
  ///Currently HwTreeSegment with Kite leaf.
  class VTKHwSegment: public HwTreeSegment<VTKHwSegment,VTKBud,cxxadt::Kite>{
  public:
    ///\brief Constructor, identical in HwTreeSegment
    ///\param p Base point of the segment
    ///\param d Direction of the segment
    ///\param go Gravelius order
    ///\param l Segment length
    ///\param r Segment radius
    ///\param rh Segment heartwood radius
    ///\param tree Tree the segment belongs to
    VTKHwSegment(const Point& p,const PositionVector& d, const LGMdouble go,const METER l,
		 const METER r,const METER rh,Tree<VTKHwSegment,VTKBud>* tree)
      :HwTreeSegment(p,d,go,l,r,rh,tree){}

  };

  ///\brief VKTCfSegment tree segment for LignumVKT project.
  ///
  ///Currently HwTreeSegment with Kite leaf.
  class VTKCfSegment: public CfTreeSegment<VTKCfSegment,VTKCfBud>{
  public:
    ///\brief Constructor, identical in CfTreeSegment
    ///\param p Base point of the segment
    ///\param d Direction of the segment
    ///\param go Gravelius order
    ///\param l Segment length
    ///\param r Segment radius
    ///\param rh Segment heartwood radius
    ///\param tree Tree the segment belongs to
    VTKCfSegment(const Point& p,const PositionVector& d, const LGMdouble go,const METER l,
		 const METER r,const METER rh,Tree<VTKCfSegment,VTKCfBud>* tree)
      :CfTreeSegment(p,d,go,l,r,rh,tree){}

  };
  
  ///\brief VKTBud bud for LignumVKT project.
  ///
  ///Currently Bud with VKTHwSegment and VKTBud.
  class VTKBud: public  Bud<VTKHwSegment,VTKBud>{
  public:
    ///\brief Constructor, identical in Bud
    ///\param p  Position of the bud
    ///\param d Direction of the bud
    ///\param go Gravelius order
    ///\param tree Tree the bud belongs to
    VTKBud(const Point& p, const PositionVector& d, const LGMdouble go, Tree<VTKHwSegment,VTKBud>* tree)
      :Bud(p,d,go,tree){}
  };

  ///\brief VKTCfBud bud for LignumVKT project.
  ///
  ///Currently Bud with VKTCfSegment and VKTCfBud.
  class VTKCfBud: public  Bud<VTKCfSegment,VTKCfBud>{
  public:
    ///\brief Constructor, identical in Bud
    ///\param p  Position of the bud
    ///\param d Direction of the bud
    ///\param go Gravelius order
    ///\param tree Tree the bud belongs to
    VTKCfBud(const Point& p, const PositionVector& d, const LGMdouble go, Tree<VTKCfSegment,VTKCfBud>* tree)
      :Bud(p,d,go,tree){}
  };
  ///\defgroup VTKTypes Type definitions
  ///Type aliases for VTK datatypes

  ///\ingroup VTKTypes
  ///\brief Lignum tree
  typedef Tree<VTKHwSegment,VTKBud> LignumVTKTree;
  typedef Tree<VTKCfSegment,VTKCfBud> LignumVTKCfTree;
  ///\ingroup VTKTypes
  ///\brief Data collection from tree segments and leaves
  typedef TreeSegmentDataCollection TSData;
  ///\ingroup VTKTypes
  ///\brief Vector containg tree segment and leaf data from in axes and in a tree
  typedef std::vector<TSData> TSDataVector;
  ///\ingroup VTKTypes
  ///\brief VTK petiole line 
  typedef vtkSmartPointer<vtkLine> Lineptr;
  ///\ingroup VTKTypes
  ///\brief VTK Parametric function for splines 
  typedef vtkSmartPointer<vtkParametricFunctionSource>  PFSptr;
  ///\ingroup VTKTypes
  ///\brief VTK Tube generator
  typedef vtkSmartPointer<vtkTubeFilter> TFptr;
  ///\ingroup VTKTypes
  ///\brief VTK polydata mapper
  typedef vtkSmartPointer<vtkPolyDataMapper> PDMptr;
  ///\ingroup VTKTypes
  ///\brief VTK actor
  typedef vtkSmartPointer<vtkActor> Actorptr;
  ///\ingroup VTKTypes
  ///\brief Vector for petiole lines in a tree
  typedef std::vector<vtkSmartPointer<vtkLine>> LineVector;
  ///\ingroup VTKTypes
  ///\brief Vector for parametric function sources generated in a tree
  typedef std::vector<vtkSmartPointer<vtkParametricFunctionSource>> PFSVector;
  ///\ingroup VTKTypes
  ///\brief Vector for tube filters generated in a tree
  typedef std::vector<vtkSmartPointer<vtkTubeFilter>> TubeFilterVector;
  ///\ingroup VTKTypes
  ///\brief Vector for tube mappers generated in a tree
  typedef std::vector<vtkSmartPointer<vtkPolyDataMapper>> TubeMapperVector;
  ///\ingroup VTKTypes
  ///\brief Vector for actors generated in a tree
  typedef std::vector<vtkSmartPointer<vtkActor>> VTKActorVector;
  ///\ingroup VTKTypes
  ///\brief Alias for actors generated for tubes
  typedef VTKActorVector TubeActorVector;
  ///\ingroup VTKTypes
  ///\brief Alias for actors generated for petiole lines
  typedef VTKActorVector LineActorVector;
  ///\ingroup VTKTypes
  ///\brief Alias for actors generated for leaves
  typedef VTKActorVector LeafActorVector;
  
  ///\defgroup VTKconstants Constant definitions
  ///Constants used in LignumVTKTree

  ///\ingroup VTKconstants
  ///\brief Petiole line segment width (in pixels) for visualization.
  ///\note For VTK the minimum value is 1.
  const double LINE_WIDTH = 1.0;
  ///\ingroup VTKconstants
  ///\brief Resolution for spline tube rectangular sides in visualization.
  const int TUBE_NUMBER_OF_SIDES = 30;
  ///\ingroup VTKconstants
  ///\brief Technical minimum segment radius for visualization 
  ///\sa CollectTSData::operator()
  const double MIN_SEGMENT_RADIUS = 1e-05;
  ///\ingroup VTKconstants
  ///\brief Tree segment radius as tube radius scalar
  const std::string TUBE_RADIUS_SCALAR = "LGAR";
  ///\ingroup VTKconstants
  ///\brief Tree segment heartwood radius as tube radius scalar
  const std::string TUBE_HW_RADIUS_SCALAR = "LGARh";
  ///\ingroup VTKconstants
  ///\brief Tree segment radius including foliage radius as tube radius scalar
  const std::string TUBE_FOLIAGE_RADIUS_SCALAR = "LGARf";
  ///\ingroup VTKconstants
  ///\brief Leaf area scalar
  const std::string LEAF_AREA_SCALAR="LGAA";
  ///\ingroup VTKconstants
  ///\brief BroadLeaf petiole scalar name
  const std::string LINE_WIDTH_SCALAR="PetioleWidth";
  ///\ingroup VTKconstants
  ///\brief BroadLeaf leaf color 
  const std::string LEAF_COLOR="Green";
  ///\ingroup VTKconstants
  ///\brief VTK PartitionedDataSetCollection file extension
  const std::string VTPC_EXTENSION=".vtpc";
  ///\ingroup VTKconstants
  ///\brief VTK MultiBlockDataSet file extension
  const std::string VTM_EXTENSION=".vtm";
  ///\ingroup VTKconstants
  ///\brief VTK DataSet block name for tree segments 
  const std::string TREE_SEGMENT_BLOCK="TreeSegmentBlock";
  ///\ingroup VTKconstants
  ///\brief VTK DataSet block name for tree segment heartwood
  const std::string TREE_HWSEGMENT_BLOCK="TreeHwSegmentBlock";
  ///\ingroup VTKconstants
  ///\brief VTK DataSet block name for tree segment foliage
  const std::string TREE_SEGMENT_FOLIAGE_BLOCK="TreeSegmentFoliageBlock";
  ///\ingroup VTKconstants
  ///\brief VTK DataSet block name for leaves
  const std::string LEAF_BLOCK="LeafBlock";
  ///\ingroup VTKconstants
  ///\brief VTK DataSet block name for petioles
  const std::string PETIOLE_BLOCK="PetioleBlock";
  
  ///\brief Data to be collected from tree segments.
  ///
  ///Points collected will be used to construct VTK geometric objects representing tree including
  ///tree segment tubes, petioles and leaves. The data of interest can be associated as
  ///scalar values with the points and further in the geometric objects to  be constructed.  
  ///\sa  LignumToVTK::treeToTSData
  ///\sa LignumToVTK::treeToPetioleData
  ///\note Add new vectors for data items to the list as needed
  class TreeSegmentDataCollection{
  public:
    std::vector<cxxadt::Point> vpoints;///< Vector of points of interest to construct geometry of the object
    std::vector<double> vL;///< Vector of (segment) lengths in one axis
    std::vector<double> vR;///< Vector of segment radii in one axis
    std::vector<double> vRh;///< Vector of segment heartwood radii in one axis
    std::vector<double> vRf;///< Vector of segment radii to foliage limit
    std::vector<double> vWf;///< Vector of foliage mass
    std::vector<double> vA;///< Vector for leaf area or needle foliage area
    std::vector<double> vQin;///< Vector for incoming radiation
    std::vector<double> vQabs;///< Vector for absorbed radiation
    std::vector<double> vP;///< Vector for photosynthesis 
    std::vector<string> vname;///< Vector for scalar name
  };
 
  
  ///\brief Append two vectors
  ///\param v1 The first vector
  ///\param v2 The second vector
  ///\retval v1 The vector \p v1 with \p v2 appended
  inline TSDataVector& Append(TSDataVector& v1,TSDataVector& v2)			    
  {
    v1.insert(v1.end(),v2.begin(),v2.end());
    return v1;
  }

  ///\brief VTK Tube actors.
  ///
  ///Create and append new VTK tube actors to a vector of tube actors.
  ///Use the functor with std::accumulate
  class CreateVTKTubeActor{
  public:
    ///\brief Create and append one actor to the vector \p v
    ///
    ///Create a new actor and associate polydata mapper \p pdm with it 
    ///\param v Vector of VTK tube actors
    ///\param pdm VTK polydata mapper
    ///\return TubeActorVector with new tube actor appended
    TubeActorVector& operator()(TubeActorVector& v, PDMptr pdm)const;
  };

  ///\brief VTK Tube mappers.
  ///
  ///Create and append new VTK tube mappers to a vector of tube mappers.
  ///Use the functor with std::accumulate
  class CreateVTKTubeMapper{
  public:
    ///\brief Create and append one tube mapper to the vector \p v
    ///
    ///Create a new  tube mapper and associate tube filter \p tf with it
    ///\param v Vector of VTK tube mappers
    ///\param tf VTK tube filter
    ///\return TubeMapperVector with new tube mapper appended
    TubeMapperVector& operator()(TubeMapperVector& v, TFptr tf)const;
  };

  ///\brief VTK Tube filters.
  ///
  ///Create and append new VTK tube filters to a vector of tube filters.
  ///Use the functor with std::accumulate.
  class CreateVTKTubeFilter{
  public:
    ///Create and append one tube filter to the vector \p v
    ///
    ///Create a new  tube mapper and associate parametric function source \p fs with it
    ///\param v Vector of VTK tube filters
    ///\param fs VTK parametric function source
    ///\return TubeFilterVector with new tube filter appended
    TubeFilterVector& operator()(TubeFilterVector& v,PFSptr fs)const;
  };

  ///\brief Create line actor vector for leaf petioles
  ///
  ///Create lines from petiole points and create line actor for each line
  ///Use the functor with std::accumulate
  class CreateVTKLineActor{
  public:
    ///\brief Create line actor
    ///
    ///Create a line from petiole base and end points and construct line actor. Append line actor to
    ///vector \p v.
    ///\param v Vector for VTK actors
    ///\param data Data for petiole base and end points
    ///\retval v TubeActor vector with new actor appended
    LineActorVector& operator()(LineActorVector& v, TSData& data)const;
  };

  ///\brief Vreate leaf actor vector for kite shaped leaves
  class CreateVTKKiteLeafActor{
  public:
    LineActorVector& operator()(LineActorVector& v, TSData& data)const;
  };
  
  ///\brief Create splines from tree segment points.
  ///
  ///Use the functor with std::accumulate.
  class CreateVTKSpline{
  public:
    ///\brief Constructor
    ///\param r Spline tube lengthwise resolution
    CreateVTKSpline(int r):resolution(r){}
    ///Create and append one spline to the vector \p v
    ///\param v Vector of spline tubes
    ///\param data Data collected (points and scalars) from tree segments in one axis
    ///\post The number of spline points is \f$N\times r + 1\f$
    ///where \f$N\f$ is the number of tree segment points and \f$ r \f$ spline resolution. 
    ///\return PFSVector with the new spline appended
    PFSVector& operator()(PFSVector& v, TSData& data)const;
  private:
    int resolution;///< Spline  lengthwise resolution 
  };

  ///\brief Collect petiole data.
  ///
  ///Use this functor with Lignum::AccumulateDown and LignumVTK::Append.
  template<class TS, class BUD,class S>
  class CollectPetioleData{
  public:
    ///\brief Collect petiole data from broadleaved segments
    ///
    ///Collect the two petiole points and set radius for visualisation.
    ///Pass the data with TSData. 
    ///\param v Vector of petiole data
    ///\param tc Tree compartment (tree segment, axis, branching point or bud)
    ///\retval v TSData element with petiole data appended to \p v
    ///\sa PETIOLE_RADIUS
    ///\sa TSData
    TSDataVector& operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const;
  };

  ///\brief Collect Kite leaf data.
  ///
  ///Use this functor with Lignum::AccumulateDown and LignumVTK::Append.
  template<class TS, class BUD,class S>
  class CollectLeafData{
  public:
    ///\brief Collect Kite leaf data from broadleaved segments
    ///
    ///Collect the leaf perimiter points and physiological data of intereset.
    ///The physiological data will appear as scalars in the VTK file.   
    ///\param v Vector of leaf data
    ///\param tc Tree compartment (tree segment, axis, branching point or bud)
    ///\retval v TSData element with the leaf data appended to \p v
    ///\sa TSData
    TSDataVector& operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const;
  };
  
  ///\brief Collect data from tree segments in axes.
  ///
  ///Collect data from tree segments, points and scalar values.
  ///Data from segments is represented in TSData.
  ///Use this functor with Lignum::AccumulateDown and LignumVTK::Append.
  ///\sa LignumToVTK::treeToTSData
  template<class TS, class BUD>
  class CollectTSData{
  public:
    ///\brief Collect data from segments
    ///
    ///Create a new TSData data element in bud and append it to vector \p v.
    ///Collect data from tree segments with Lignum::AccumulateDown.
    ///\param v Vector of tree segment data for axes.
    ///\param tc Tree compartment (tree segment, axis, branching point or bud).
    ///\retval v TSData element appended in bud and updated in subsequent tree segments.
    TSDataVector& operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const;
  };

  ///\brief Read Lignum XML file and produce VTK VTPC/VTM file for ParaView.
  ///
  ///Each axis will be represented as VTK tubes for sapwood and heartwood, each leaf as a VTK triangular strip
  ///and each petiole as VTK line. These are grouped in different data sets so that final
  ///editing for visualization is easier. For tree segments and leaves designated simulation data
  ///can be added as scalar values for tubes and triangular strips respectively.
  ///\note Current implementation is for broadleaved trees with kite shaped leaves. 
  ///\todo Implement coniferous trees. If necessary impement broad leaved trees
  ///with triangle and ellipse leaves.
  ///\sa createBroadLeafTreeVTKDataSets
  ///\sa writePartitionedDataSetCollection
  class LignumToVTK{
  public:
    ///\brief Constructor
    ///\param r Spline segment lengthwise resolution
    LignumToVTK(int r=10):resolution(r){}
    ///\brief Create VTK geometric representation of the Lignum tree.
    ///
    ///Each axis will be represented as VTK tube, each leaf as a VTK triangular strip
    ///and each petiole as VTK line. These are grouped in different data sets so that final
    ///editing for visualization is easier. For tree segments and leaves designated simulation data
    ///can be added as scalar values for tubes and triangular strips respectively.
    ///\note Current implementation is for Kite shaped leaves. Implement triangle and ellipse leaves
    ///if necessary.
    template <typename TREE>
    LignumToVTK& createBroadLeafTreeVTKDataSets(TREE& t, bool add_to_renderer=false);
    template<typename TREE>
    LignumToVTK& createConiferTreeVTKDataSets(TREE& t,bool add_to_renderer=false);
    ///\brief Write VTK partitioned data sets to a file
    ///\param file_name The file name with *vtpc* file extension 
    LignumToVTK& writePartitionedDataSetCollection(const string& file_name);
    //\brief Create VTK geometric representation of the Lignum tree.
    ///
    ///Each axis will be represented as VTK tube, each leaf as a VTK triangular strip
    ///and each petiole as VTK line. These are grouped in different data sets so that final
    ///editing for visualization is easier. For tree segments and leaves designated simulation data
    ///can be added as scalar values for tubes and triangular strips respectively.
    ///\note Current implementation is for Kite shaped leaves.
    ///\note The use of VTK MultiBlockDataSets is discouraged. Use VTK PartitionedDataSets instead
    ///\sa createBroadLeafTreeVTKDataSets
    template<typename TREE>
    LignumToVTK& createBroadLeafTreeVTKMultiBlockDataSets(TREE& t);
    ///\brief Write VTK multiblock data set to a file
    ///\param file_name The file name with *vtm* extension
    ///\note The use of VTK MultiBlockDataSets is discouraged, use VTK PartitionedDataSets instead.
    ///\sa writePartitionedDataSetCollection
    LignumToVTK& writeMultiBlockDataSet(const string& file_name);
    ///\brief Retrieve the VTK renderer.
    ///
    ///Retrieve the VTK renderer for quick inspection in the VTK render window.
    ///\retval renderer The VTK renderer
    vtkRenderer* getRenderer()const{return renderer;}
  protected:
    ///\brief Collect data from tree segments.
    ///
    ///Collect tree segment points for spline tube representation.
    ///Collect scalar values of interest. Scalar values are assigned
    ///as VTK point data for the each segment point.
    ///Each TSData element in \p tsdv contain data from all segments in a single axis.
    ///\param t Lignum tree
    ///\param tsdv Vector for tree segment data collection
    ///\retval tsdv Vector containing collected tree segment data
    TSDataVector& treeToHwTSData(LignumVTKTree& t, TSDataVector& tsdv);
    TSDataVector& treeToCfTSData(LignumVTKCfTree& t, TSDataVector& tsdv);
    ///\brief Collect petiole data.
    ///
    ///Collect petiole points for VTK line. Each TSData element in \p tsdv represents one petiole
    ///\param t Lignum tree
    ///\param tsdv Vector for petiole data collection
    ///\retval tsdv Vector containing collected petiole data
    ///\sa PETIOL_RADIUS
    ///\sa TSData
    TSDataVector& treeToPetioleData(LignumVTKTree& t, TSDataVector& tsdv);
    ///\brief Collect leaf data.
    ///
    ///Collect leaf corner points for VTK triangular strip and scalar values
    ///for further visualization and analysis.
    ///\param t The Lignum tree
    ///\param tsdv Vector for leaf data
    ///\retval tsdv Vector containing leaf data
    TSDataVector& treeToKiteLeafData(LignumVTKTree& t, TSDataVector& tsdv);
    ///\brief Create petiole VTK lines.
    ///
    ///Each petiole is represented as a VTK line with two end points and 
    ///VTK line mappers and actors are created.
    ///\param tsdv Vector of petiole data
    ///\param lav Vector for VTK line actors
    ///\retval lav Vector containg VTK Line actors.
    LineActorVector& createLineActors(TSDataVector& tsdv,LineActorVector& lav);
    ///\brief Kite leaf VTK triangular strip.
    ///
    ///Each leaf is represented as VTK triangular strip and VTP mappers and actors are created.
    ///Additional leaf data will be represented as VTK scalars
    ///\param tsdv Vector for leaf data, shape and scalar values
    ///\param lav Vector for VTK triangular strip actors
    ///\retval lav Vector containing VTK triangular strip actors
    LineActorVector& createKiteLeafActors(TSDataVector& tsdv,LineActorVector& lav);
    ///\brief Create tree segment splines.
    ///
    ///Create splines from segment points returned by LignumToVTK::treeToTSData.
    ///Each axis will be represented as a VTK tube.
    ///\param tsdv Vector for tree segment points
    ///\param pfsv Vector for splines
    ///\retval psfv Vector containing splines
    ///\note The `vtkParametricSpline` class used creates 1D interpolating splines. 
    PFSVector& vtkPointsToVtkSpline(TSDataVector& tsdv, PFSVector& pfsv)const;
    ///\brief Add tree segment spline tube radius data.
    ///
    ///Use tree segment radius scalar data to set spline tube radius for each spline point.
    ///The \p scalar_name should describe the radius, for example segment radius, segment heartwood radius
    ///or radius to foliage limit. These are returned by LignumToVTK::treeToTSData and
    ///LignumToVtk::vtkPointsToVtkSpline respectively.
    ///\param tsdv Vector containg collected tree segment data from LignumToVTK::treeToTSData
    ///\param pfsv Vector for splines with their radii in each point
    ///\param scalar_name The scalar name for the tube radius
    ///\retval psfv Vector containg splines and radii for each spline point 
    PFSVector& createTubeRadiusScalars(TSDataVector& tsdv, PFSVector& pfsv, const string& scalar_name)const;
    ///\brief Create tree segment spline tube filters.
    ///
    ///Create spline tube filters for each spline returned by LignumToVTK::vtkPointsToVtkScalars.
    ///\param pfsv Vector containing splines and their radii
    ///\param tfv Vector for spline tube filters
    ///\retval tfv Vector containing  spline tube filters
    TubeFilterVector& createTubeFilters(PFSVector& pfsv, TubeFilterVector& tfv)const;
    ///\brief Create spline tube filter mappers.
    ///
    ///Create  spline tube filter mappers for each spline filter
    ///returned by LignumToVTK::createTubeFilters.
    ///\param tfv Vector containing spline tube filters
    ///\param tmv Vector for spline tube filter mappers
    ///\retval tmv Vector containing spline tube filter mappers
    TubeMapperVector& createTubeMappers(TSDataVector& tfv,TubeMapperVector& tmv,const string& scalar_name)const;
    ///\brief Create tree segment tube actors for mappers.
    ///
    ///Create actors for mappers returnd by LignumToVTK::createTubeMappers.
    ///\param tmv Vector containing mappers
    ///\param tav Vector for actors
    ///\retval tav Vector containing actors
    TubeActorVector& createTubeActors(TubeMapperVector& tmv, TubeActorVector& tav)const;
    ///\brief Populate renderer with actors.
    ///
    ///Add the vector \p v of VTK actors to VTK renderer
    ///\param v Vector of VTK actors for geometry objects
    ///\sa LignumToVTK::renderer
    LignumToVTK& addActorsToRenderer(VTKActorVector& v);
    ///\brief Set coloring for an VTK actor
    ///\param color Color name available in VTK
    LignumToVTK& setActorColor(const string& color);
    ///\brief Add vector \p v of actors to \p dataset_collection as VTK partitioned data set block
    ///\param v Vector of actors
    ///\param block_name Name of the dataset block, visible in ParaView
    ///\sa LignumVTK::dataset_collection
    LignumToVTK& addPartitionedDataSet(VTKActorVector& v, const string& block_name);
    ///\brief Populate VTK multiblock data set with actors
    ///\param v Vector of actors
    ///\note The use of VTK MultiBlockDataSets is discouraged, use VTK ParitionedDataSets instead.
    ///\sa createBroadLeafTreeVTKDataSets
    ///\sa writePartitionedDataSetCollection
    LignumToVTK& addMultiBlockDataSet(VTKActorVector& v);
  private:
    vtkNew<vtkPartitionedDataSetCollection> dataset_collection;///< Partitioned data set for VTK geometry models
    vtkNew<vtkXMLPartitionedDataSetCollectionWriter> collection_writer; ///< Fule output for Partitionded data sets
    vtkNew<vtkMultiBlockDataSet> mb_set;///< Multiblock data set for VTK geometry models
    vtkNew<vtkRenderer> renderer;///< VTK renderer for VTK render window for quick testing
    vtkNew<vtkXMLMultiBlockDataWriter> writer;///< File output for VTK MultiBlock data sets
    int resolution;///< Spline resolution, number of spline segments
  };
}
#include <LignumVTKTreeI.h>
#endif
