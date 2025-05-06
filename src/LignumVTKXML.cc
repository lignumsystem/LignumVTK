#include <LignumVTKXML.h>
namespace lignumvtk{
  LignumVTKXML& LignumVTKXML::setFileName(const std::string& fname)
  {
    parser->SetFileName(fname.c_str());
    file_name = fname; 
    return *this;
  }
  
  LignumVTKXML& LignumVTKXML::parse()
  {
    parser->Parse();
    return *this;
  }

  const std::string LignumVTKXML::treeType()const
  {
    vtkXMLElementPtr root = parser->GetRootElement();
    const std::string segment_type = root->GetAttribute("SegmentType");
    return segment_type;
  }

  const std::string LignumVTKXML::leafType()const
  {
    vtkXMLElementPtr root = parser->GetRootElement();
    const std::string segment_type = root->GetAttribute("SegmentType");
    if (segment_type == "Hw"){
      vtkXMLElementPtr leaf = root->LookupElementWithName("BroadLeaf");
      std::string leaf_shape = leaf->GetAttribute("Shape");
      return leaf_shape;
    }
    else{
      return std::string("");
    }
  }
}
