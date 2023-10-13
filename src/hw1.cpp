//zhengyu huang
#include "hw1.h"
#include "hw1_scenes.h"

using namespace hw1;

bool inCircle(Real x, Real y, Vector2 center, Real radius){
    return sqrt(pow(x - center.x, 2) + pow(y - center.y, 2)) < radius; // distance formula
}

bool inRectangle(Real x, Real y, Vector2 p_min, Vector2 p_max){
    return (x <= p_max.x && y <= p_max.y) && (x >= p_min.x && y >= p_min.y);
}

//return true if positive half otherwise false
bool halfPlane (Vector2 p0, Vector2 p1, Vector2 q){
    Vector2 p01 = p1 - p0;
    Vector2 n01 = Vector2{p01.y, -p01.x};
    return dot((q - p0), n01) <= 0;
}

bool inTriangle(Real x, Real y, Vector2 p0, Vector2 p1, Vector2 p2){
    Vector2 q = Vector2{x, y};
    return halfPlane(p0, p1, q) &&  halfPlane(p1, p2, q) &&  halfPlane(p2, p0, q);
}


void paintCanvas(Image3* imgPtr,Real width, Real height, Vector3 color){
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            (*imgPtr)(x, y) = color;
        }
    }
}

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    
    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{0.5, 0.5, 0.5};
    Vector3 canvasColor = color;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }

    paintCanvas(&img, img.width, img.height, canvasColor);
    //if pixel center in circle, paint in in color
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            if (inCircle(x,y,center,radius)){
                img(x, y) = color;
            }
        }
    }
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render multiple circles
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    int scene_id = std::stoi(params[0]);
    const CircleScene &scene = hw1_2_scenes[scene_id];

    Image3 img(scene.resolution.x, scene.resolution.y);
    std::vector<Circle> primitives = scene.objects;

    paintCanvas(&img,scene.resolution.x, scene.resolution.y, scene.background);
    

    //render circles using rasterization style
    for (Circle circle : primitives){
        Vector2 center = circle.center;
        Real radius = circle.radius;
        Vector3 color = circle.color;
        // bounding box
        for (int y = center.y - radius; y < center.y + radius; y++){
            for (int x = center.x - radius; x < center.x + radius; x++){
                if(x >= 0 && y >= 0 && inCircle(x,y,center,radius)){
                    img(x,y) = color;
                }
            }
        }
    }
     return img;
}


Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
    std::vector<Shape> shapes = scene.shapes;
    
    Image3 img(scene.resolution.x, scene.resolution.y);
    paintCanvas(&img,scene.resolution.x, scene.resolution.y, scene.background);


    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (auto shape : shapes){
                if (auto *circle = std::get_if<Circle>(&shape)) {
                    if (inCircle(x, y, circle->center, circle->radius)){
                        img(x,y) = circle->color;
                    }
                } 
                else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
                    if (inRectangle(x, y, rectangle->p_min, rectangle->p_max)){
                        img(x, y) = rectangle->color;
                    }
                }
                else if (auto *triangle = std::get_if<Triangle>(&shape)) {
                    if (inTriangle(x, y, triangle->p0, triangle->p1, triangle->p2)){
                        img(x, y) = triangle->color;
                    }
                }
            }
        }
    }
    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);


    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            //img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}
