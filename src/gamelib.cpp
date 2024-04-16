#include "gamelib.h"

const float DEPTH = gridSize * 8;
const float PI = 3.14159f;
const unsigned int scr_wid = 800;
const unsigned int scr_hgt = 600;
const float movementSpeed = gridSize / 20.f;
const float cameraRotationSpeed = 0.05f;
const float gridSize = 50.f;


void playerMove(std::vector <RectangleShape>& box, Vector2f& pos, Vector2f& movePlayer, Vector2f& dir)
{
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        movePlayer.y = movementSpeed * dir.y;
        movePlayer.x = movementSpeed * dir.x;
        checkForCollision(box, pos, movePlayer);
        pos.x += movePlayer.x;
        pos.y += movePlayer.y;
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        movePlayer.y = -movementSpeed * dir.y;
        movePlayer.x = -movementSpeed * dir.x;
        checkForCollision(box, pos, movePlayer);
        pos.x += movePlayer.x;
        pos.y += movePlayer.y;
    }
}


bool checkForBox(std::vector <RectangleShape>& v1, Vector2f& curPoint, Vector2f& rayDir)
{
    bool exists = false;
    Vector2i linesPosGrid;
    linesPosGrid.x = (curPoint.x + rayDir.x) / (int)gridSize;
    linesPosGrid.y = (curPoint.y + rayDir.y) / (int)gridSize;
    for (size_t i = 0; i < v1.size() && !exists; i++)
    {
        if (v1[i].getPosition().x / (int)gridSize == linesPosGrid.x
            && v1[i].getPosition().y / (int)gridSize == linesPosGrid.y)
            exists = true;
    }
    return exists;
}


void checkForCollision(std::vector <RectangleShape>& boxes, Vector2f& pos, Vector2f& move)
{
    RectangleShape player(Vector2f(5, 5));
    player.setPosition(pos);

    for (auto& box : boxes)
    {
        FloatRect playerCol = player.getGlobalBounds();
        FloatRect nextPos = playerCol;
        nextPos.top += move.y;
        nextPos.left += move.x;
        FloatRect boxCol = box.getGlobalBounds();
        if (boxCol.intersects(nextPos))
        {
            //Дно
            if (playerCol.top < boxCol.top
                && playerCol.top + playerCol.height < boxCol.top + boxCol.height
                && playerCol.left < boxCol.left + boxCol.width
                && playerCol.left + playerCol.width > boxCol.left)
            {
                move = Vector2f(move.x, 0.f);
            }
            //Верх
            else if (playerCol.top > boxCol.top
                && playerCol.top + playerCol.height > boxCol.top + boxCol.height
                && playerCol.left < boxCol.left + boxCol.width
                && playerCol.left + playerCol.width > boxCol.left)
            {
                move = Vector2f(move.x, 0.f);
            }
            //Право
            if (playerCol.left < boxCol.left
                && playerCol.left + playerCol.width < boxCol.left + boxCol.width
                && playerCol.top < boxCol.top + boxCol.height
                && playerCol.top + playerCol.height > boxCol.top)
            {
                move = Vector2f(0.f, move.y);
            }
            //Лево
            if (playerCol.left > boxCol.left
                && playerCol.left + playerCol.width > boxCol.left + boxCol.width
                && playerCol.top < boxCol.top + boxCol.height
                && playerCol.top + playerCol.height > boxCol.top)
            {
                move = Vector2f(0.f, move.y);
            }
        }
    }
}


Vector2f lineDirectionY(Vector2f& pos, Vector2f& rayDir)
{
    float posX = pos.x / gridSize - (int)(pos.x / gridSize);
    float posY = pos.y / gridSize - (int)(pos.y / gridSize);

    Vector2f sideDistY;

    if (rayDir.x >= 0.f && rayDir.y >= 0.f) {
        sideDistY.y = pos.y + (1.f - posY) * gridSize;
        sideDistY.x = pos.x + (1.f - posY) * (rayDir.x / rayDir.y) * gridSize;
    }
    else if (rayDir.x < 0.f && rayDir.y > 0.f)
    {
        sideDistY.y = pos.y + (1.f - posY) * gridSize;
        sideDistY.x = pos.x + (1.f - posY) * (rayDir.x / rayDir.y) * gridSize;
    }
    else if (rayDir.x < 0.f && rayDir.y < 0.f)
    {
        sideDistY.y = pos.y - posY * gridSize;
        sideDistY.x = pos.x - posY * (rayDir.x / rayDir.y) * gridSize;
    }
    else if (rayDir.x > 0.f and rayDir.y < 0.f)
    {
        sideDistY.y = pos.y - posY * gridSize;
        sideDistY.x = pos.x - posY * (rayDir.x / rayDir.y) * gridSize;
    }
    return sideDistY;
}


Vector2f lineDirectionX(Vector2f& pos, Vector2f& rayDir)
{
    float posX = pos.x / gridSize - (int)(pos.x / gridSize);
    float posY = pos.y / gridSize - (int)(pos.y / gridSize);

    Vector2f sideDistX;

    if (rayDir.x >= 0.f && rayDir.y >= 0.f) {
        sideDistX.y = pos.y + (1.f - posX) * gridSize * (rayDir.y / rayDir.x);
        sideDistX.x = pos.x + (1.f - posX) * gridSize;
    }
    else if (rayDir.x < 0.f && rayDir.y > 0.f)
    {
        sideDistX.y = pos.y - posX * gridSize * (rayDir.y / rayDir.x);
        sideDistX.x = pos.x - posX * gridSize;
    }
    else if (rayDir.x < 0.f && rayDir.y < 0.f)
    {
        sideDistX.y = pos.y - posX * gridSize * (rayDir.y / rayDir.x);
        sideDistX.x = pos.x - posX * gridSize;
    }
    else if (rayDir.x > 0.f and rayDir.y < 0.f)
    {
        sideDistX.y = pos.y + (1.f - posX) * gridSize * (rayDir.y / rayDir.x);
        sideDistX.x = pos.x + (1.f - posX) * gridSize;
    }
    return sideDistX;
}


void linesMove(VertexArray& mLines, Vector2f& pos, VertexArray& lines, VertexArray& sky, VertexArray& ground, Vector2f& dir, Vector2f& cameraPlane, std::vector <RectangleShape>& mapObjects)
{
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        float d_anglePlayer = -cameraRotationSpeed;
        dir = rotateVec(dir, d_anglePlayer);
        cameraPlane = rotateVec(cameraPlane, d_anglePlayer);
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        float d_anglePlayer = cameraRotationSpeed;
        dir = rotateVec(dir, d_anglePlayer);
        cameraPlane = rotateVec(cameraPlane, d_anglePlayer);
    }

    int linesIndex = 0;
    int skyInd = 0;
    int groundInd = 0;

    for (float x = 0; x < scr_wid; x++)
    {
        float cameraX = 2 * x / (float)scr_wid - 1.f;
        Vector2f rayDir = dir + cameraPlane * cameraX;

        Vector2f sideDistX = lineDirectionX(pos, rayDir);
        Vector2f sideDistY = lineDirectionY(pos, rayDir);
        Vector2f range = Vector2f(sqrt(pow(sideDistX.x - pos.x, 2) + pow(sideDistX.y - pos.y, 2)), sqrt(pow(sideDistY.x - pos.x, 2) + pow(sideDistY.y - pos.y, 2)));

        double distToWall = 0;
        bool hit = false;

        while (!hit)
        {
            range = Vector2f(sqrt(pow(sideDistX.x - pos.x, 2) + pow(sideDistX.y - pos.y, 2)), sqrt(pow(sideDistY.x - pos.x, 2) + pow(sideDistY.y - pos.y, 2)));

           /*if (range.x > DEPTH and range.y > DEPTH)
            {
                lines1.append(pos);
                lines1.append(Vector2f(pos.x + DEPTH * cos(atan(rayDir.y / rayDir.x)) * (rayDir.x / abs(rayDir.x)), pos.y + DEPTH * sin(atan(rayDir.y / rayDir.x)) * (rayDir.x / abs(rayDir.x))));
                distToWall = DEPTH;
                hit = true;
            }*/

            if (range.x < range.y)
            {
                if (checkForBox(mapObjects, sideDistX, rayDir))
                {
                    if ((int)x % 10 == 0)
                    {
                        mLines.append(Vector2f(pos.x / 5, pos.y / 5));
                        mLines.append(Vector2f(sideDistX.x / 5, sideDistX.y / 5));
                    }
                    distToWall = range.x;
                    hit = true;
                }
                else
                {
                    sideDistX.x += gridSize * (rayDir.x / abs(rayDir.x));
                    sideDistX.y += gridSize * (rayDir.y / rayDir.x) * (rayDir.x / abs(rayDir.x));
                }
            }

            else
            {
                if (checkForBox(mapObjects, sideDistY, rayDir))
                {
                    if ((int)x % 10 == 0)
                    {
                        mLines.append(Vector2f(pos.x / 5, pos.y / 5));
                        mLines.append(Vector2f(sideDistY.x / 5, sideDistY.y / 5));
                    }
                    distToWall = range.y;
                    hit = true;
                }
                else
                {
                    sideDistY.y += gridSize * (rayDir.y / abs(rayDir.y));
                    sideDistY.x += gridSize * (rayDir.x / rayDir.y) * (rayDir.y / abs(rayDir.y));
                }
            }
        }

        Vector3f rgb = Vector3f(63, 72, 204);

        rgb.x = rgb.x * (1 - (distToWall / 500)) < 29 ? 29 : rgb.x * (1 - (distToWall / 500));
        rgb.y = rgb.y * (1 - (distToWall / 500)) < 35 ? 35 : rgb.y * (1 - (distToWall / 500));
        rgb.z = rgb.z * (1 - (distToWall / 500)) < 107 ? 107 : rgb.z * (1 - (distToWall / 500));

        distToWall = distToWall * cos(atan(rayDir.y / rayDir.x) - atan(dir.y / dir.x));

        double lineH = scr_hgt / distToWall;
        double drawStart = 0.5 * scr_hgt -  15 * (scr_wid / scr_hgt) * lineH;
        double drawEnd = 0.5 * scr_hgt +  15* (scr_wid / scr_hgt) * lineH;

        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= scr_hgt) drawEnd = scr_hgt - 1;

        //SKY
        sky.append(Vector2f(x, 0.f));
        sky[skyInd].color = Color::Color(0, 242, 242);
        skyInd++;
        sky.append(Vector2f(x, scr_hgt / 2));
        sky[skyInd].color = Color::Color(0, 160, 160);
        skyInd++;

        //WALLS
        lines.append(Vector2f((float)x, drawStart));
        
        lines[linesIndex].color = Color::Color(rgb.x, rgb.y, rgb.z);  //10 12 35
        linesIndex++;
        lines.append(Vector2f((float)x, drawEnd));
        lines[linesIndex].color = Color::Color(rgb.x, rgb.y, rgb.z);;
        linesIndex++;

        //GROUND
        ground.append(Vector2f(x, scr_hgt / 2));
        ground[groundInd].color = Color::Color(0, 80, 0);
        
        groundInd++;
        ground.append(Vector2f(x, scr_hgt));
        ground[groundInd].color = Color::Color(0, 128, 0);
        groundInd++;
    }
}

Vector2f rotateVec(sf::Vector2f& vec, float value)
{
    return sf::Vector2f(
        vec.x * std::cos(value) - vec.y * std::sin(value),
        vec.x * std::sin(value) + vec.y * std::cos(value)
    );
}