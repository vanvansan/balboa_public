#include "hw2.h"
#include "hw2_scenes.h"

using namespace hw2;

Vector2 c_space_to_i_space(Vector2 v, Real s, Real a, Real width, Real height){
    Real x_in_i = width * ((v.x + s*a) / (2 * s * a));
    Real y_in_i = height * ((v.y + s) / (2 * s));
    return Vector2{x_in_i, y_in_i};
}

Vector2 projectedPoint(Vector3 v){
    Vector2 projectedPoint{v.x/v.z,v.y/v.z};
    return projectedPoint;
}

Vector3 projectedPoint_v3(Vector3 v){
    Vector3 projectedPoint{v.x/v.z,v.y/v.z, -1.0};
    return projectedPoint;
}

// use b0',b1', b2', p0, p1, p2 to find b0, b1, b2, return {b0, b1, b2} as v3
Vector3 original_coefficients(Vector3 b_prime, Vector3 p0, Vector3 p1, Vector3 p2){
    Real b0p, b1p, b2p;
    b0p = b_prime.x;
    b1p = b_prime.y;
    b2p = b_prime.z;
    Real B = (1 / ((b0p / p0.z) + (b1p / p1.z) + (b2p / p2.z)));
    return Vector3{b0p * B / p0.z, b1p * B / p1.z, b2p * B / p2.z};
}

//returns Vector3{ b0' , b1', b2' }, order of passing in matters
Vector3 unique_coefficient(Vector3 p0p, Vector3 p1p, Vector3 p2p, Vector3 p){
    Real b0, b1, b2;
    Real full_area = length(cross(p1p, p0p)) / 2;
    b0 = (length(cross(p0p, p))/2) / full_area;
    b1 = (length(cross(p1p, p))/2) / full_area;
    b2 = (length(cross(p2p, p))/2) / full_area;
    return Vector3{b0, b1, b2};
}

//return the depth of a point using unique coefficients 
Real original_depth(Vector3 b, Vector3 p0, Vector3 p1, Vector3 p2){
    Real b0, b1, b2;
    b0 = b.x;
    b1 = b.y;
    b2 = b.z;
    return b0*p0.z + b1*p1.z + b2*p2.z;
}

Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single 3D triangle

    Image3 img(640 /* width */, 480 /* height */);

    Vector3 p0{0, 0, -1};
    Vector3 p1{1, 0, -1};
    Vector3 p2{0, 1, -1};
    
    Real s = 1; // scaling factor of the view frustrum
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    Real z = -1;
    Real z_near = 1e-6; // distance of the near clipping plane
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-p0") {
            p0.x = std::stof(params[++i]);
            p0.y = std::stof(params[++i]);
            p0.z = std::stof(params[++i]);
        } else if (params[i] == "-p1") {
            p1.x = std::stof(params[++i]);
            p1.y = std::stof(params[++i]);
            p1.z = std::stof(params[++i]);
        } else if (params[i] == "-p2") {
            p2.x = std::stof(params[++i]);
            p2.y = std::stof(params[++i]);
            p2.z = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        }
    }

    Real aspect_ratio = double(img.width)/img.height; 

    Vector2 p0p{0.0,0.0};
    Vector2 p1p{0.0,0.0};
    Vector2 p2p{0.0,0.0};
    p0p = projectedPoint(p0);
    p1p = projectedPoint(p1);
    p2p = projectedPoint(p2); 



    // change from camera space to image space

    paintCanvas(&img, img.width, img.height, Vector3{1.0, 1.0, 1.0});

    p0p = c_space_to_i_space(p0p, s, aspect_ratio, img.width, img.height);
    p1p = c_space_to_i_space(p1p, s, aspect_ratio, img.width, img.height);
    p2p = c_space_to_i_space(p2p, s, aspect_ratio, img.width, img.height);
    


    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            if (inTriangle(x, y , p0p, p1p, p2p)) img(x, y) = color;
        }
    }
    return img;
}

Image3 hw_2_2(const std::vector<std::string> &params) {


    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    paintCanvas(&img, Vector3{1.0, 1.0, 1.0});

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_2_3(const std::vector<std::string> &params) {
    // Homework 2.3: render a triangle mesh with vertex colors

    Image3 img(640 /* width */, 480 /* height */);

    Real s = 1; // scaling factor of the view frustrum
    Real z_near = 1e-6; // distance of the near clipping plane
    int scene_id = 0;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-s") {
            s = std::stof(params[++i]);
        } else if (params[i] == "-znear") {
            z_near = std::stof(params[++i]);
        } else if (params[i] == "-scene_id") {
            scene_id = std::stoi(params[++i]);
        }
    }

    TriangleMesh mesh = meshes[scene_id];
    UNUSED(mesh); // silence warning, feel free to remove this

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_2_4(const std::vector<std::string> &params) {
    // Homework 2.4: render a scene with transformation
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.camera.resolution.x,
               scene.camera.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

