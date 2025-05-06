#ifndef LIGNUMVTKXML_H
#define LIGNUMVTKXML_H
#include <string>
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkXMLDataParser.h>
typedef vtkSmartPointer<vtkXMLDataElement> vtkXMLElementPtr;

namespace lignumvtk{
  ///\brief Check Lignum tree type
  ///
  ///Peek into Lignum XML and determine tree type,
  ///coniferous or broadleaved. For broadleaved tree
  ///determine leaf shape, ellipse, triangle or kite.
  class LignumVTKXML{
  public:
    ///\brief Set file name for the XML \p parser
    ///\param file_name Lignum XML File name
    LignumVTKXML& setFileName(const std::string& file_name);
    ///\brief Parse Lignum XML file
    LignumVTKXML& parse();
    ///\brief Query tree type
    ///\return tree type, "Cf" for coniferous or "Hw" for broadleaved
    const std::string treeType()const;
    ///\brief Query leaf type
    ///\return "Ellipse", "Triangle" or "Kite" for broadleaved.
    ///Empty string if applied to coniferous trees.
    const std::string leafType()const;
  private:
    std::string file_name;///< Lignum XML file name
    vtkNew<vtkXMLDataParser> parser;///< XML file parser
  };
}
#endif
