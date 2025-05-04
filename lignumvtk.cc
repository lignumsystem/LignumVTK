#include <cstdlib>
#include <ParseCommandLine.h>
#include <Lignum.h>
#include <XMLTree.h>
#include <LignumVTK.h>
#include <LignumVTKTree.h>
using namespace lignumvtk;

int main(int argc,char* argv[])
{
  XMLDomTreeReader<VTKHwSegment,VTKBud,Kite> tree_reader;
  LignumToVTK lignumvtk;
  Tree<VTKHwSegment,VTKBud> lignum_tree(Point(0,0,0), PositionVector(0,0,1.0));
  if (argc < 3 ){
    cout << "Usage: lignumtree.xml outputfile.vtpc [render]" << endl;
    cout << "Optional render -> Open and use VTK render window" << endl;
    return EXIT_FAILURE;
  }
  bool render = false;
  if (CheckCommandLine(argc,argv,"render")){
    render = true;
  }
  string tree_file = argv[1];
  string vtk_file = argv[2];
  cout << "Reading tree: " << tree_file <<endl; 
  tree_reader.readXMLToTree(lignum_tree, tree_file);
  lignumvtk.createBroadLeafTreeVTKDataSets(lignum_tree,render);
  lignumvtk.writePartitionedDataSetCollection(vtk_file);
  if (!render){
    return EXIT_SUCCESS;
  }
  cout << "Starting Render Window" <<endl;
  
  vtkRenderer* ren = lignumvtk.getRenderer();
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(ren);
  renderWindow->SetWindowName("Lignum");
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  vtkNew<vtkNamedColors> colors;
  ren->SetBackground(colors->GetColor3d("Silver").GetData());
  ren->ResetCamera();
  renderWindow->SetSize(600,600);
  
  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Enable();
  renderWindowInteractor->Start();

  cout << "Done" <<endl;
  return EXIT_SUCCESS;
}
