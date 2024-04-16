#include "gamelib.h"


int main()
{
    // —Œ«ƒ¿Õ»≈ Œ Õ¿ ////////////////////////////////////////////////
    RenderWindow window(VideoMode(scr_wid, scr_hgt), "3D Engine!");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // »Õ»÷»¿À»«¿÷»ﬂ  ¿–“€ ///////////////////////////////////////////
  
    std::vector <RectangleShape> mapObjects;
    VertexArray grid(Lines);

    std::vector <RectangleShape> miniMap;
    int mapWid = scr_wid / 5;
    int mapHgt = scr_hgt / 5;
    float miGridSize = gridSize / 5;


    // —Œ«ƒ¿Õ»≈ Ã»Õ» ¿–“€ //////////////////////////////////////////

    Vector2f pos = Vector2f(gridSize + gridSize / 5, gridSize + gridSize / 5);

    RectangleShape mBack(Vector2f(mapWid, mapHgt));
    mBack.setPosition(0.f, 0.f);
    mBack.setFillColor(Color::Black);

    RectangleShape mPlayer(Vector2f(miGridSize / 2.5f, miGridSize / 2.5f));
    mPlayer.setFillColor(Color::Green);
    mPlayer.setPosition(Vector2f(pos.x / 5, pos.y / 5));
    mPlayer.setOrigin(mPlayer.getSize().x / 2, mPlayer.getSize().y / 2);

    int mi = 0;
    bool map = 0;

    VertexArray miniGrid(Lines);
    for (int i = 0; i < mapWid; i += miGridSize)
    {
        miniGrid.append(Vector2f(i, 0));
        miniGrid[mi].color = Color::Color(195, 195, 195, 150);
        mi++;
        miniGrid.append(Vector2f(i, mapHgt));
        miniGrid[mi].color = Color::Color(195, 195, 195, 150);
        mi++;
    }
    for (int i = 0; i < mapHgt; i += miGridSize)
    {
        miniGrid.append(Vector2f(0, i));
        miniGrid[mi].color = Color::Color(195, 195, 195, 150);
        mi++;
        miniGrid.append(Vector2f(mapWid, i));
        miniGrid[mi].color = Color::Color(195, 195, 195, 150);
        mi++;
    }

    VertexArray rayCast(Lines);

    // Œ“–»—Œ¬ ¿ Õ¿◊¿À‹ÕŒ…  ¿–“€ ///////////////////////////////////
    int i = 0;
    for (float x = 0.f; x < scr_wid; x += gridSize) 
    {
        grid.append(Vector2f(x, 0.f));
        grid[i].color = Color::Color(195, 195, 195, 150);
        i++;
        grid.append(Vector2f(x, scr_hgt));
        grid[i].color = Color::Color(195, 195, 195, 150);
        i++;
        RectangleShape obj(Vector2f(gridSize, gridSize));
        obj.setPosition(Vector2f(x, 0.f));
        RectangleShape obj1(Vector2f(gridSize, gridSize));
        obj1.setPosition(Vector2f(x, scr_hgt - gridSize));
        obj.setFillColor(Color::Red);
        obj1.setFillColor(Color::Red);
        mapObjects.push_back(obj);
        mapObjects.push_back(obj1);
    }

    for (float y = 0.f; y < scr_hgt; y += gridSize)
    {
        grid.append(Vector2f(0.f, y));
        grid[i].color = Color::Color(195, 195, 195, 150);
        i++;
        grid.append(Vector2f(scr_wid, y));
        grid[i].color = Color::Color(195, 195, 195, 150);
        i++;
        RectangleShape obj(Vector2f(gridSize, gridSize));
        obj.setPosition(Vector2f(0.f, y));
        RectangleShape obj1(Vector2f(gridSize, gridSize));
        obj1.setPosition(Vector2f(scr_wid - gridSize, y));
        obj.setFillColor(Color::Red);
        obj1.setFillColor(Color::Red);
        mapObjects.push_back(obj);
        mapObjects.push_back(obj1);
    }
    
    // Õ¿œ–¿¬À≈Õ»≈ » ƒ¬»∆≈Õ»≈ ///////////////////////////////////////
    Vector2i mousePosGrid;

    Vector2f movePlayer;

    double dirX = -1, dirY = 0;
    Vector2f dir(dirX, dirY);

    double cameraX = 0, cameraY = 0.66;
    Vector2f cameraPlane(cameraX, cameraY);
  
    VertexArray lines(Lines);
    VertexArray walls(Lines);
    VertexArray ground(Lines);
    VertexArray sky(Lines);

    Event event;
    Vector2i p(pos.x / (int)gridSize, pos.y / (int)gridSize);
    RectangleShape pShow(Vector2f(gridSize, gridSize));
    pShow.setFillColor(Color::Green);

    while (window.isOpen())
    {
        if (!map)
        {
            miniMap.resize(0);

            mousePosGrid.x = Mouse::getPosition(window).x / (int)gridSize;
            mousePosGrid.y = Mouse::getPosition(window).y / (int)gridSize;

            if (Mouse::isButtonPressed(Mouse::Left))
            {
                bool exists = false;
                
                for (size_t i = 0; i < mapObjects.size() && !exists; i++)
                {
                    if ((mapObjects[i].getPosition().x / (int)gridSize == mousePosGrid.x
                        && mapObjects[i].getPosition().y / (int)gridSize == mousePosGrid.y)
                        || (mapObjects[i].getPosition().x / (int)gridSize == p.x
                        && mapObjects[i].getPosition().y / (int)gridSize == p.y))
                    {
                        exists = true;
                    }
                }
                if (!exists)
                {
                    RectangleShape object(Vector2f(gridSize, gridSize));
                    object.setPosition(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize);
                    object.setFillColor(Color::Color(63, 72, 204));
                    mapObjects.push_back(object);
                }
            }

            if (Mouse::isButtonPressed(Mouse::Right))
            {
                bool exists = false;
               
                int index = 0;
                for (size_t i = 0; i < mapObjects.size() && !exists; i++)
                {
                    if (mapObjects[i].getPosition().x / (int)gridSize == mousePosGrid.x
                        && mapObjects[i].getPosition().y / (int)gridSize == mousePosGrid.y)
                    {
                        if (mousePosGrid.x == 0 && mousePosGrid.y < 100
                            || mousePosGrid.x == (scr_wid / (int)gridSize) - 1 && mousePosGrid.y < 100
                            || mousePosGrid.y == 0 && mousePosGrid.x < 100
                            || mousePosGrid.y == (scr_hgt / (int)gridSize) - 1 && mousePosGrid.x < 100)
                            exists = false;
                        else
                        {
                            exists = true;
                            index = i;
                        }
                    }
                }
                if (exists) mapObjects.erase(mapObjects.begin() + index);
            }

            if (Keyboard::isKeyPressed(Keyboard::P))
            {
                bool exists = false;
                for (size_t i = 0; i < mapObjects.size() && !exists; i++)
                {
                    if (mapObjects[i].getPosition().x / (int)gridSize == mousePosGrid.x
                        && mapObjects[i].getPosition().y / (int)gridSize == mousePosGrid.y)
                        exists = true;
                }
                if (!exists)
                {
                    pShow.setPosition(Vector2f(mousePosGrid.x * gridSize, mousePosGrid.y * gridSize));
                    pos = pShow.getPosition();
                }
            }
            
            window.clear();
            for (auto& i : mapObjects)
                window.draw(i);
            window.draw(pShow);
            window.draw(grid);

            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                map = 1;
                for (auto& walls : mapObjects)
                {
                    RectangleShape mWall(Vector2f(miGridSize, miGridSize));
                    mWall.setPosition(Vector2f(walls.getPosition().x / 5, walls.getPosition().y / 5));
                    mWall.setFillColor(walls.getFillColor());
                    miniMap.push_back(mWall);
                }
            }

        }
        else
        {

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            walls.resize(0);
            sky.resize(0);
            ground.resize(0);
            rayCast.resize(0);

            playerMove(mapObjects, pos, movePlayer, dir);
            linesMove(rayCast, pos, walls, sky, ground, dir, cameraPlane, mapObjects);
            mPlayer.setPosition(pos.x / 5, pos.y / 5);

            window.clear();

            window.draw(sky);
            window.draw(ground);
            window.draw(walls);
            window.draw(mBack);
            for (auto& mimap : miniMap)
                window.draw(mimap);
            window.draw(rayCast);
            window.draw(mPlayer);
            window.draw(miniGrid);
            if (Keyboard::isKeyPressed(Keyboard::M)) map = !map;
        }

        window.display();
    }

    return 0;
}