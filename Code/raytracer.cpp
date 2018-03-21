#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "shapes/plane.h"
#include "shapes/quad.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"
#include "shapes/mesh.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node, const std::string& sceneDirPath)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere") {
        Point pos(node["position"]);
        double radius = node["radius"];
        Rotation rot;
        if ( node.count("rotation") != 0 ) {
            rot = Rotation(node["rotation"]);
        } else {
            rot = Rotation(0,0,0);
        }

        obj = ObjectPtr(new Sphere( pos, radius, rot ));
    } else if ( node["type"] == "triangle") {
        Point v1(node["position1"]);
        Point v2(node["position2"]);
        Point v3(node["position3"]);
        obj = ObjectPtr(new Triangle(v1,v2,v3));
    } else if ( node["type"] == "quad") {
        Point v1(node["position1"]);
        Point v2(node["position2"]);
        Point v3(node["position3"]);
        Point v4(node["position4"]);
        obj = ObjectPtr(new Quad(v1,v2,v3,v4));
    } else if ( node["type"] == "plane") {
        Point point(node["position"]);
        Point normal(node["normal"]);
        normal.normalize( ); // normal is normalized here !
        obj = ObjectPtr(new Plane(point,normal));
    } else if ( node["type"] == "cylinder") {
        Point position(node["position"]);
        double height = node["height"];
        double radius = node["radius"];
        obj = ObjectPtr(new Cylinder(position,height,radius));
    } else if ( node["type"] == "cone") {
        Point position(node["position"]);
        double height = node["height"];
        double radius = node["radius"];
        obj = ObjectPtr(new Cone(position,height,radius));
    } else if ( node["type"] == "mesh" ) {
        Point position(node["position"]);
        double scale = node["scale"];
        std::string modelFile = sceneDirPath + node["model"].get< std::string >( );
        obj = ObjectPtr(new Mesh(position,scale,modelFile));
    } else {
        cerr << "Unknown object type: " << node["type"] << ".\n";
        return false;
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"],sceneDirPath);
    scene.addObject(obj);
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node, const std::string& sceneDirPath) const
{
    bool isFlat = ( node.count( "is_flat" ) != 0 && node[ "is_flat" ] );
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];

    if ( node.count("color") != 0 && !node["color"].is_null( ) ) {
        return Material(Color(node["color"]), ka, kd, ks, n, isFlat);
    } else {
        std::string filename = sceneDirPath + node["texture"].get<std::string>( );
        return Material(std::make_shared< Image >(filename), ka, kd, ks, n, isFlat);
    }
}

bool Raytracer::readScene(string const &ifname)
try
{
    std::string sceneDirPath = ifname.substr( 0, ifname.find_last_of( "/" ) + 1 );

    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);
    if ( jsonscene.count( "EyePitch" ) > 0 ) {
      scene.setEyePitch( jsonscene["EyePitch"] );
    } else {
      scene.setEyePitch( 0 );
    }
    if ( jsonscene.count( "AmbientLight" ) > 0 ) {
      scene.setAmbientLight( Color( jsonscene[ "AmbientLight" ] ) );
    }
    scene.setHasShadows( jsonscene["Shadows"].is_boolean( ) && jsonscene["Shadows"] );
    if ( jsonscene["MaxRecursionDepth"].is_number( ) ) {
        scene.setMaxRecursionDepth( jsonscene["MaxRecursionDepth"] );
    } else {
        scene.setMaxRecursionDepth( 0 );
    }
    if ( jsonscene["SuperSamplingFactor"].is_number( ) ) {
        scene.setSuperSamplingFactor( jsonscene["SuperSamplingFactor"] );
    } else {
        scene.setSuperSamplingFactor( 0 );
    }

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode,sceneDirPath))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
