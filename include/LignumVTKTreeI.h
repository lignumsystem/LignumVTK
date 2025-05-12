#ifndef LIGNUMVTK_TREE_I_H
#define LIGNUMVTK_TREE_I_H
namespace lignumvtk{

  template <class TS, class BUD>
  TSDataVector& CollectTSData<TS,BUD>::operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const
    {

      if (BUD* b = dynamic_cast<BUD*>(tc)){
	//Create new tree segment data collection item
	TSData data;
	Point p = GetPoint(*b);
	//The last end point in the segment points in this branch
	data.vpoints.insert(data.vpoints.begin(),p);
	//Corresponding segment length, radius and heartwood radius
	data.vL.insert(data.vL.begin(),0.0);
	data.vR.insert(data.vR.begin(),MIN_SEGMENT_RADIUS);
	data.vRh.insert(data.vRh.begin(),MIN_SEGMENT_RADIUS);
	data.vRf.insert(data.vRf.begin(),MIN_SEGMENT_RADIUS);
	data.vWf.insert(data.vWf.begin(),0.0);
	data.vname.insert(data.vname.begin(),TUBE_RADIUS_SCALAR);
	v.push_back(data);
      }
      if (TS* ts = dynamic_cast<TS*>(tc)){
	//Collect data of interest
	Point p = GetPoint(*ts);
	double l = GetValue(*ts,LGAL);
	double r = GetValue(*ts,LGAR);
	double rh = GetValue(*ts,LGARh);
	double rf = 0.0;
	if (CfTreeSegment<TS,BUD>* cfts = dynamic_cast<CfTreeSegment<TS,BUD>*>(tc)){
	  rf =  GetValue(*cfts,LGARf);
	}
	double wf = GetValue(*ts,LGAWf);
	//cout << "Foliage radius " << rf <<endl;
	//Insert the point 
	v[0].vpoints.insert(v[0].vpoints.begin(),p);
	//Corresponding segment length, radius and heartwood radius
	v[0].vL.insert(v[0].vL.begin(),l);
	v[0].vR.insert(v[0].vR.begin(),r);
	v[0].vRh.insert(v[0].vRh.begin(),rh);
	v[0].vRf.insert(v[0].vRf.begin(),rf);
	v[0].vWf.insert(v[0].vWf.begin(),wf);
	v[0].vname.insert(v[0].vname.begin(),TUBE_RADIUS_SCALAR);
      }
      return v;
    }

  template <class TS, class BUD,class S>
  TSDataVector& CollectPetioleData<TS,BUD,S>::operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const
  {
    if (TS* ts = dynamic_cast<TS*>(tc)){
	if (GetNumberOfLeaves(*ts) != 0){
	  std::list<BroadLeaf<S>*>& ls = GetLeafList(*ts);
	  for (auto it=ls.begin();it != ls.end();it++){
	    //Each petiole will be independent with two points and radius
	    TSData data;
	    BroadLeaf<S>* leaf = *it;
	    const Petiole& petiole = GetPetiole(*leaf);
	    Point p1 = GetStartPoint(petiole);
	    Point p2 = GetEndPoint(petiole);
	    //Petiole points
	    data.vpoints.push_back(p1);
	    data.vpoints.push_back(p2);
	    //Two points need two radii for point data
	    data.vR.push_back(LINE_WIDTH);
	    data.vR.push_back(LINE_WIDTH);
	    data.vname.push_back(LINE_WIDTH_SCALAR);
	    data.vname.push_back(LINE_WIDTH_SCALAR);
	    //There will be one TSData element for each petiole
	    v.push_back(data);
	  }
	}
    }
    //The result is a list of TSData elements 
    return v;
  }

  template <class TS, class BUD,class S>
  TSDataVector& CollectLeafData<TS,BUD,S>::operator()(TSDataVector& v, TreeCompartment<TS,BUD>* tc)const
  {
    if (TS* ts = dynamic_cast<TS*>(tc)){
	if (GetNumberOfLeaves(*ts) != 0){
	  std::list<BroadLeaf<S>*>& ls = GetLeafList(*ts);
	  for (auto it=ls.begin();it != ls.end();it++){
	    TSData data;
	    BroadLeaf<S>* leaf = *it;
	    const S& shape = GetShape(*leaf);
	    //Each leaf shape should have this method
	    data.vpoints = shape.getVertexVector(data.vpoints);
	    double area = GetValue(*leaf,LGAA);
	    double qin = GetValue(*leaf,LGAQin);
	    double qabs = GetValue(*leaf,LGAQabs);
	    double p = GetValue(*leaf,LGAP);
	    //Set data for each corner point, i.e. in VTK they will be set as point data or cell data 
	    for (unsigned int i=0; i < data.vpoints.size(); i++){
	      data.vA.push_back(area);
	      data.vQin.push_back(qin);
	      data.vQabs.push_back(qabs);
	      data.vP.push_back(p);
	      data.vname.push_back(LEAF_AREA_SCALAR);
	    }
	    //There will be one TSData element for each leaf in the list
	    v.push_back(data);
	  }
	}
    }
    return v;
  }

  template<typename TREE>
  LignumToVTK& LignumToVTK::createBroadLeafTreeVTKDataSets(TREE& t, bool add_to_renderer)
  {
    //Data collection from the tree
    TSDataVector tsv;
    tsv = treeToHwTSData(t,tsv);
    TSDataVector pv;
    pv = treeToPetioleData(t,pv);
    TSDataVector lv;
    lv = treeToKiteLeafData(t,lv);

    //Create geometric obtects for tree segments
    TubeMapperVector tmv;
    tmv = createTubeMappers(tsv,tmv,TUBE_RADIUS_SCALAR);

    //VTK actors for tree segments, leaves an petioles.
    //Leaf and petiole actor construction creates also
    //corresponding geometric objects
    TubeActorVector tav;
    tav = createTubeActors(tmv,tav);
    LeafActorVector lav;
    lav = createKiteLeafActors(lv,lav);
    LineActorVector lineav;
    lineav = createLineActors(pv,lineav);

    //Add datasets to dataset collection
    addPartitionedDataSet(tav,TREE_SEGMENT_BLOCK);
    addPartitionedDataSet(lav,LEAF_BLOCK);
    addPartitionedDataSet(lineav,PETIOLE_BLOCK);
    if (add_to_renderer == true){
      addActorsToRenderer(tav);
      addActorsToRenderer(lav);
      addActorsToRenderer(lineav);
    }
    return *this;
  }

  template<typename TREE>
  LignumToVTK& LignumToVTK::createBroadLeafTreeVTKMultiBlockDataSets(TREE& t)
  {
    TSDataVector tsv;
    tsv = treeToHwTSData(t,tsv);
    TSDataVector pv;
    pv = treeToPetioleData(t,pv);
    TSDataVector lv;
    lv = treeToKiteLeafData(t,lv);
      
    TubeMapperVector tmv;
    tmv = createTubeMappers(tsv,tmv,TUBE_RADIUS_SCALAR);
      
    TubeActorVector tav;
    tav = createTubeActors(tmv,tav);
    LeafActorVector lav;
    lav = createKiteLeafActors(lv,lav);
    LineActorVector lineav;
    lineav = createLineActors(pv,lineav);

    addMultiBlockDataSet(tav);
    addMultiBlockDataSet(lav);
    addMultiBlockDataSet(lineav);
    return *this;
  }

  template<typename TREE>
  LignumToVTK& LignumToVTK::createConiferTreeVTKDataSets(TREE& t,bool add_to_renderer)
  {
    TSDataVector tsv;
    tsv = treeToCfTSData(t,tsv);
    TubeMapperVector tm_radius_v;
    tm_radius_v = createTubeMappers(tsv,tm_radius_v,TUBE_RADIUS_SCALAR);
    TubeMapperVector tm_hwradius_v;
    tm_hwradius_v = createTubeMappers(tsv,tm_hwradius_v,TUBE_HW_RADIUS_SCALAR);
    TubeMapperVector tm_foliage_radius_v;
    tm_foliage_radius_v = createTubeMappers(tsv,tm_foliage_radius_v,TUBE_FOLIAGE_RADIUS_SCALAR);
    TubeActorVector ta_radius_v;
    ta_radius_v= createTubeActors(tm_radius_v,ta_radius_v);
    TubeActorVector ta_hwradius_v;
    ta_hwradius_v = createTubeActors(tm_hwradius_v,ta_hwradius_v);
    TubeActorVector ta_foliage_radius_v;
    ta_foliage_radius_v = createTubeActors(tm_foliage_radius_v,ta_foliage_radius_v);

    addPartitionedDataSet(ta_foliage_radius_v,TREE_SEGMENT_FOLIAGE_BLOCK);
    addPartitionedDataSet(ta_hwradius_v,TREE_HWSEGMENT_BLOCK);
    addPartitionedDataSet(ta_radius_v,TREE_SEGMENT_BLOCK);
    
    if (add_to_renderer == true){
      addActorsToRenderer(ta_radius_v);
      addActorsToRenderer(ta_hwradius_v);
      addActorsToRenderer(ta_foliage_radius_v);
    }
    return *this;
  }
}
#endif
