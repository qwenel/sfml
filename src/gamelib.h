#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>

using namespace sf;

extern const float DEPTH;
extern const float PI;
extern unsigned int scr_wid;
extern unsigned int scr_hgt;
extern const float movementSpeed;
extern const float cameraRotationSpeed;
extern const float gridSize;

void playerMove(std::vector <RectangleShape>& box, Vector2f& pos, Vector2f& movePlayer, Vector2f& dir);
void linesMove(VertexArray& mLines, Vector2f& pos, VertexArray& lines, VertexArray& sky, VertexArray& ground, Vector2f& dir, Vector2f& cameraPlane, std::vector <RectangleShape>& mapObjects);
Vector2f rotateVec(Vector2f& vec, float value);
bool checkForBox(std::vector <RectangleShape>& v1, Vector2f& curPoint, Vector2f& rayDir);
void checkForCollision(std::vector <RectangleShape>& box, Vector2f& pos, Vector2f& move);
Vector2f lineDirectionX(Vector2f& pos, Vector2f& rayDir);
Vector2f lineDirectionY(Vector2f& pos, Vector2f& rayDir);