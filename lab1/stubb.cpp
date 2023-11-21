#include <osg/Version>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/Image>
#include <osg/NodeVisitor>
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osgUtil/IntersectVisitor>
#include <osg/LOD>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Export>
#include <osg/Material>

class IntersectCallback : public osg::NodeCallback
{
  public:
  IntersectCallback(osg::PositionAttitudeTransform* _truck, osg::Vec3 _line_p0, osg::Vec3 _line_p1) :
    truck{_truck},  line_p0{_line_p0}, line_p1{_line_p1} {}

  //Callback method called by the NodeVisitor when visiting a node
  virtual void operator()(osg::Node* root, osg::NodeVisitor* aVisitor) override {

    osgUtil::LineSegmentIntersector* intersector = new osgUtil::LineSegmentIntersector(line_p0,line_p1);

    osgUtil::IntersectionVisitor visitor(intersector);
    root->accept(visitor);
    // osg::AnimationPath* myPath = new osg::AnimationPath();
    // osg::AnimationPath::ControlPoint start = osg::AnimationPath::ControlPoint(osg::Vec3(4.f, 25.f,3.f),osg::Quat(),osg::Vec3(.5f,.5f,.5f));
    // osg::AnimationPath::ControlPoint middle = osg::AnimationPath::ControlPoint(osg::Vec3(40.f, 25.f,3.f)));
    // osg::AnimationPath::ControlPoint finish = osg::AnimationPath::ControlPoint(osg::Vec3(4.f, 25.f,3.f),osg::Quat(6.28, osg::Vec3(0,0,1)), osg::Vec3(.5f,.5f,.5f));
    // myPath->insert(0.0, start);
    // myPath->insert(10.0,middle);
    // myPath->insert(15.0, finish);

    // osg::AnimationPathCallback* imitation = new osg::AnimationPathCallback(myPath);


    if(intersector->containsIntersections()){
      osgUtil::LineSegmentIntersector::Intersections& intersections = intersector->getIntersections();  

      // for (auto &itr : intersections){
      //   for (auto &node : itr.nodePath ){
      //     // if(truck == node){
            
      //     // }
              for (size_t i = 0; i < 4; i++){
              truck->setAttitude(osg::Quat(sin(rand()%180), osg::Vec3(0,0,1)));
              }
              
          
          
      //   }
      // }
    }
    else{

    }
    osg::NodeCallback::operator()(root, aVisitor);
  }
  private:
  //To be used if passing the root through constructor
  //osg::ref_ptr<osg::Group> root;
  osg::ref_ptr<osg::PositionAttitudeTransform> truck;
  osg::Vec3 line_p0, line_p1;
};

int main(int argc, char *argv[]){
  
  osg::ref_ptr<osg::Group> root = new osg::Group;
  std::cout << "This is version : " <<OSGVIEWER_EXPORT::osgGetVersion() << std::endl;
#if 1
  /// Line ---

  osg::Vec3 line_p0 (25, 0, 0);
  osg::Vec3 line_p1 ( 25, 50, 0);
  
  osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
  vertices->push_back(line_p0);
  vertices->push_back(line_p1);
  
  osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
  colors->push_back(osg::Vec4(0.9f,0.2f,0.3f,1.0f));

  osg::ref_ptr<osg::Geometry> linesGeom = new osg::Geometry();
  linesGeom->setVertexArray(vertices);
  linesGeom->setColorArray(colors, osg::Array::BIND_OVERALL);
  
  linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
  
  osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode();
  lineGeode->addDrawable(linesGeom);
  lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
  
  root->addChild(lineGeode);
  
  /// ---
#endif

  
  //Create a heightfield   
  osg::HeightField* heightField = new osg::HeightField();
  heightField->allocate(50.f, 50.f);
  heightField->setOrigin(osg::Vec3(.0f,.0f,.0f));
  heightField->setXInterval(1.f);
  heightField->setYInterval(1.f);
  
  for (size_t r = 0; r < heightField->getNumRows(); r++){
    for (size_t c = 0; c < heightField->getNumColumns(); c++ ){
      heightField->setHeight(c,r,sin(rand()%180));
    }
  }
    
  osg::Geode* myGeode = new osg::Geode(); 

  osg::ref_ptr<osg::Image> image = osgDB::readImageFile("earth.jpg");

  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
  tex->setImage(image.get());

  myGeode->addDrawable(new osg::ShapeDrawable(heightField));
  myGeode->getOrCreateStateSet()->setTextureAttributeAndModes(0,tex);

  osg::PositionAttitudeTransform* truck = new osg::PositionAttitudeTransform();
  osg::PositionAttitudeTransform* ship = new osg::PositionAttitudeTransform();
  ship->setPosition(osg::Vec3(25,25,20));
  ship->setAttitude(osg::Quat(-0.7, osg::Vec3(1,0,0)));
  truck->setPosition(osg::Vec3(4,45, 3));
  truck->setScale(osg::Vec3(0.5,0.5,0.5));

  
  osg::Node* shipNode = osgDB::readNodeFile("spaceship.osg");
  osg::Node* truckNode = osgDB::readNodeFile("dumptruck.osg");


  
  ship->addChild(shipNode);
  truck->addChild(truckNode);
  
  //LOD
  osg::Node* truckNodeLow = (osg::Node*) truckNode->clone(osg::CopyOp::DEEP_COPY_ALL);
  osg::Node* truckNodeMed = (osg::Node*) truckNode->clone(osg::CopyOp::DEEP_COPY_ALL);
  osg::Node* truckNodeHigh = (osg::Node*) truckNode->clone(osg::CopyOp::DEEP_COPY_ALL);

  osgUtil::Simplifier simplifier;

  osg::LOD* lodGroup = new osg::LOD();
  //Low LOD for truck
  simplifier.setSampleRatio(0.4f);
  truckNodeLow->accept(simplifier);
  lodGroup->addChild(truckNodeLow, 1000.f, 10000.f);

  //Medium LOD for truck
  simplifier.setSampleRatio(.6f);
  truckNodeMed->accept(simplifier);
  lodGroup->addChild(truckNodeMed, 400.f,1000.f);


  //High LOD for Truck
  simplifier.setSampleRatio(1.f);
  truckNodeHigh->accept(simplifier);
  lodGroup->addChild(truckNodeHigh, 0.f,400.f);


  osg::PositionAttitudeTransform* truckLOD = new osg::PositionAttitudeTransform();
  truckLOD->setPosition(osg::Vec3(45.f,4.f,3.f));
  truckLOD->setScale(osg::Vec3f(.5f,.5f,.5f));
  truckLOD->addChild(lodGroup);


  
  
  

  osg::AnimationPath* myPath = new osg::AnimationPath();
  osg::AnimationPath::ControlPoint start = osg::AnimationPath::ControlPoint(osg::Vec3(4.f, 25.f,3.f),osg::Quat(),osg::Vec3(.5f,.5f,.5f));
  osg::AnimationPath::ControlPoint middle = osg::AnimationPath::ControlPoint(osg::Vec3(40.f, 25.f,3.f),osg::Quat(3.14, osg::Vec3(0,0,1)),osg::Vec3(.5f,.5f,.5f));
  osg::AnimationPath::ControlPoint finish = osg::AnimationPath::ControlPoint(osg::Vec3(4.f, 25.f,3.f),osg::Quat(6.28, osg::Vec3(0,0,1)), osg::Vec3(.5f,.5f,.5f));


  myPath->insert(0.0, start);
  myPath->insert(20.0,middle);
  myPath->insert(40.0, finish);



  osg::AnimationPathCallback* imitation = new osg::AnimationPathCallback(myPath);
  truck->setUpdateCallback(imitation);


  IntersectCallback* myAction = new IntersectCallback(ship,line_p0,line_p1);
  root->setUpdateCallback(myAction);
  root->addChild(myGeode);  
  root->addChild(ship);
  root->addChild(truck);
  root->addChild(truckLOD);
  
  

  // Optimizes the scene-graph
  // osgUtil::Optimizer optimizer;
  // optimizer.optimize(root);
  
  // Set up the viewer and add the scene-graph root
  osgViewer::Viewer viewer;
  viewer.setSceneData(root);

  // osg::ref_ptr<osg::Camera> camera = new osg::Camera;
  // camera->setProjectionMatrixAsPerspective(60.0, 1.0, 0.1, 100.0);
  // camera->setViewMatrixAsLookAt (osg::Vec3d(0.0, 0.0, 2.0),
  //                                osg::Vec3d(0.0, 0.0, 0.0),
  //                                osg::Vec3d(0.0, 1.0, 0.0));
  // camera->getOrCreateStateSet()->setGlobalDefaults();
  // viewer.setCamera(camera);
  
  return viewer.run();
}