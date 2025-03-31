#include "Game.hpp"
#include "TextureManager.hpp"

#include "Map.hpp"


#include "ECS/Components.hpp"

#include "Vector.hpp"

#include "Collision.hpp"
#include "AssetManager.hpp"

#include<sstream>

Map* map;
SDL_Renderer* Game::renderer = nullptr;
Manager manager;

SDL_Rect Game::camera = {0, 0, 1536,  1024};
AssetManager* Game::assets = new AssetManager(&manager);

auto& Player(manager.addEntity());

auto& Label(manager.addEntity());
auto& HighScore(manager.addEntity());
auto& HealthBar(manager.addEntity());
SDL_Rect playerHealthBar = {10, 10, 200, 20};


auto& startButton(manager.addEntity());
auto& settingsButton(manager.addEntity());
auto& exitButton(manager.addEntity());

auto& background(manager.addEntity());  
auto& gameover (manager.addEntity());

auto& restartButton(manager.addEntity());

auto& volumeButton(manager.addEntity());
auto& fullscreenButton(manager.addEntity());
auto& exitToMenuButton(manager.addEntity());

auto& gameOverLabel(manager.addEntity());
auto& survivedTimeLabel(manager.addEntity());
auto& backToMenuButton(manager.addEntity());

auto& resumeButton(manager.addEntity());
auto& exitToMenuButtonPause(manager.addEntity());

SDL_Rect volumeBar = {250, 270, 0, 20}; 

int score = 0;
int highScore = 0;

int volumeLevel = MIX_MAX_VOLUME / 2;
float volume = 64.0f;


const int maxEnemyID = 2;

std::string enemyName[maxEnemyID] = {"yellowGolem","blueGolem"};
int enemyHealth[maxEnemyID] = {100,20};
int enemySpeed[maxEnemyID] = {1,3};
int enemyDamage[maxEnemyID] = {20,10};
int enemyW[maxEnemyID] = {90,90};
int enemyH[maxEnemyID] = {64,64};
int enemyScale[maxEnemyID] = {7,2};
int enemyScore[maxEnemyID] = {100,50};

SDL_Event Game::event;
bool Game::isRunning = false;

bool Game::start = false;
bool Game::menu = true;
bool Game::exit = false;
bool Game::settingsEnabled = false;
bool Game::setting = false;
bool Game::pause = false;
bool Game::gameOverScene = false;
Uint32 survivedTime = 0;

bool isFullscreen = false;

Uint32 lastHitTime = 0;

Uint32 lastProjectileTime = 0;


Uint32 lastEnemySpawnTime = 0;

Game::Game() {
}

Game::~Game() {
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            std::cout << "Renderer created" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    } else {
        isRunning = false;
        std::cout << "SDL failed to initialize" << std::endl;
    }

    if (TTF_Init() == -1) {
        std::cout << "TTF failed to initialize" << std::endl;
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    initTextures();
    initComponents();

    
    Mix_PlayMusic(assets->getMusic("backgroundMusic"), -1);

    
    Mix_Volume(-1, volumeLevel); 
    Mix_VolumeMusic(volumeLevel);
}

void Game::initTextures(){
    assets->addTexture( "player", "assets/PlayerTexture/Samurai.png");
    assets->addFont("font", "assets/Font/font.ttf", 30);
    assets->addTexture("projectile", "assets/Projectiles/attack/attack.png");
    
    assets->addTexture("background", "assets/MapTexture/background.png");
    assets->addTexture("gameover", "assets/MapTexture/gameover.png");
    
    assets->addTexture("yellowGolem","assets/EnemyTexture/yellowGolem.png");
    assets->addTexture("blueGolem","assets/EnemyTexture/blueGolem.png");
    
    assets->addTexture("Player_projectile", "assets/Projectiles/attack/attack.png");

    //sounds
    assets->addSound("hurt", "assets/Sounds/hurt.mp3");
    assets->addSound("gameOver", "assets/Sounds/gameover.mp3");
    assets->addSound("gameStart", "assets/Sounds/gamestart.wav");
    assets->addSound("attack", "assets/Sounds/attack.mp3");

    assets->addMusic("backgroundMusic", "assets/Sounds/backgroundMusic.mp3");

}

void Game::initComponents(){
    background.addComponent<TransformComponent>(0, 0, 1536, 1024, 1);
    background.addComponent<SpriteComponent>("background");
    
    gameover.addComponent<TransformComponent>(0, 0, 1536, 1024, 1);
    gameover.addComponent<SpriteComponent>("gameover");


    startButton.addComponent<UI>(255, 305, "Play Game", "font", white);
    settingsButton.addComponent<UI>(255, 355, "Settings", "font", white);
    exitButton.addComponent<UI>(255, 405, "Exit", "font",white);

    SDL_Color color = {23,243,254,205};
    SDL_Color color2 = {233,23,254,205};
    Label.addComponent<UI>(10, 30, "Score: 0", "font",white);
    HighScore.addComponent<UI>(10, 300, "High Score: 0", "font", white);

    volumeButton.addComponent<UI>(255, 305, "Volume", "font", white);
    fullscreenButton.addComponent<UI>(255, 355, "Fullscreen: OFF", "font", white);
    exitToMenuButton.addComponent<UI>(255, 405, "Exit to Menu", "font", white);

    gameOverLabel.addComponent<UI>(255, 300, "Game Over", "font", white); 
    survivedTimeLabel.addComponent<UI>(255, 350, "Survived Time: 0s", "font", white); 
    backToMenuButton.addComponent<UI>(255, 450, "Back to Menu", "font", white); 

    resumeButton.addComponent<UI>(255, 305, "Resume", "font", white);
    exitToMenuButtonPause.addComponent<UI>(255, 355, "Exit To Menu","font", white);
}
void Game::initObject(){
   
    map = new Map(4, 20);
    map->LoadMap(100, 100);
    

    Player.addComponent<TransformComponent>(1000.0f, 1200.0f, 96, 96, 3);
    Player.addComponent<SpriteComponent>("player", true);
    Player.addComponent<Controller>();
    Player.addComponent<ColliderComponent>("player");
    Player.addGroup(groupPlayers);




    // assets->addTexture("projectile", "assets/MapTexture/terrain_ss.png");

    // assets->creatProjectile(Vector2D(1100,1000),Vector2D(2,0),100,1,"projectile" );
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(2,1),200,3,"projectile");
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(2,-1),200,3,"projectile");
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(3,1),200,3,"projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents() {
    SDL_PollEvent(&event);
    
    if (gameOverScene) {
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 450 && mouseY < 500) {
            backToMenuButton.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                std::cout << "Back to Menu button clicked" << std::endl;
                gameOverScene = false;
                menu = true;
            }
        } else {
            backToMenuButton.getComponent<UI>().setColor(white);
        }
    }

    else if (setting) {
        SDL_GetMouseState(&mouseX, &mouseY);


        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 300 && mouseY < 350) {
            volumeButton.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                volume += 12.8f;
                volumeLevel += static_cast<int>(volume);
                if (volume > MIX_MAX_VOLUME+0.5f) {
                    std::cout<<volume<<" "<<volumeLevel<<std::endl;
                    volumeLevel = 0;
                    volume = 0; 
                }

                
                Mix_Volume(-1, volumeLevel);
                Mix_VolumeMusic(volumeLevel); 

                
                std::stringstream volumeText;
                volumeText << "Volume: " <<( static_cast<int>(volume * 100) / MIX_MAX_VOLUME) << "%";
                volumeButton.getComponent<UI>().setText(volumeText.str());

                std::cout << "Volume set to: " <<( static_cast<int>(volume * 100) / MIX_MAX_VOLUME) << "%" << std::endl;
            }
        } else {
            volumeButton.getComponent<UI>().setColor(white);
        }

        // Fullscreen button
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 350 && mouseY < 400) {
            fullscreenButton.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (!isFullscreen) {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

                    int windowWidth, windowHeight;
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    
                    background.getComponent<TransformComponent>().width = windowWidth;
                    background.getComponent<TransformComponent>().height = windowHeight;
                    
                    fullscreenButton.getComponent<UI>().setText("Full Screen: ON");

                    isFullscreen = true;
                } else {
                    
                    SDL_SetWindowFullscreen(window, 0);

                    int initialWidth = 1536;
                    int initialHeight = 1024;
                    SDL_SetWindowSize(window, initialWidth, initialHeight);
                    
                    background.getComponent<TransformComponent>().width = initialWidth;
                    background.getComponent<TransformComponent>().height = initialHeight;
                    
                    fullscreenButton.getComponent<UI>().setText("Full Screen: OFF");

                    isFullscreen = false;
                }
            }
        } else {
            fullscreenButton.getComponent<UI>().setColor(white);
        }

        // Exit to Menu button
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 400 && mouseY < 450) {
            exitToMenuButton.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                std::cout << "Exit to Menu button clicked" << std::endl;
                setting = false;
                menu = true;
            }
        } else {
            exitToMenuButton.getComponent<UI>().setColor(white);
        }
    }

    else if(menu){
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){  
            if (start){ 
                std::cout<<"start"<<std::endl;
                Mix_PlayChannel(-1, assets->getSound("gameStart"), 0);
                score = 0;
                initObject();
                menu = false;
            }
            else if (exit){
                std::cout<<"exit"<<std::endl;
                isRunning = false;
                // menu = false;
            }
            else if (settingsEnabled){
                std::cout<<"settings"<<std::endl;
                    //Setting();
                setting = true;
                menu = false;
                
            }
        }
    }
    else if(start){
        SDL_GetMouseState(&mouseX, &mouseY);

        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            Uint32 currentTime = SDL_GetTicks(); 

            
            if (currentTime - lastProjectileTime >= 500) {
                Vector2D playerPos = Vector2D(Player.getComponent<ColliderComponent>().collider.x, Player.getComponent<ColliderComponent>().collider.y);
                Vector2D mousePos = Vector2D(mouseX - playerPos.x + camera.x, mouseY - playerPos.y + camera.y);
                playerPos.x -=30;
                playerPos.y = playerPos.y - 20;
                mousePos = mousePos.normalize();
                int playerDamage = Player.getComponent<TransformComponent>().damage;
                assets->createProjectile(playerPos, mousePos, 500, 10, playerDamage, "Player_projectile");
                Mix_PlayChannel(-1, assets->getSound("attack"), 0);
                lastProjectileTime = currentTime;
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            pause = true; 
            start = false;
        }
    }
    else if (pause) {
        SDL_GetMouseState(&mouseX, &mouseY);
        //resume
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 305 && mouseY < 355) {
            resumeButton.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                pause = false; 
                start = true;
            }
        } else {
            resumeButton.getComponent<UI>().setColor(white);
        }
        // exit to menu
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 355 && mouseY < 405) {
            exitToMenuButtonPause.getComponent<UI>().setColor(mint);
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                pause = false;
                gameOverScene = true;
                gameOver();
            }
        } else {
            exitToMenuButtonPause.getComponent<UI>().setColor(white);
        }
        return; 
    }
}


void Game::update() {
    if(pause){

        return;
    }
    
    if (!menu && !setting && !gameOverScene) {
        SDL_Rect playerCol = Player.getComponent<ColliderComponent>().collider;
        Vector2D playerPos = Player.getComponent<TransformComponent>().position;

        std::stringstream ss;
        ss << "Score: " << score;
        Label.getComponent<UI>().setText(ss.str());
        manager.refresh();
        manager.update();
        //update player HP bar
        int maxHealth = Player.getComponent<TransformComponent>().maxHealth;
        int currentHealth = Player.getComponent<TransformComponent>().health;
        playerHealthBar.w = (200 * currentHealth) / maxHealth;


        // Check for collisions
        for (auto& c : colliders) {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::CheckCollision(cCol, playerCol)) {
                // std::cout << "Collision with: " << c->getComponent<ColliderComponent>().tag << std::endl;
                Player.getComponent<TransformComponent>().position = playerPos;
            }
        }

        for (auto& e : enemies) {
            Vector2D playerPos = Vector2D(Player.getComponent<ColliderComponent>().collider.x, Player.getComponent<ColliderComponent>().collider.y);
            Vector2D playerDirection = Vector2D(playerPos.x - e->getComponent<ColliderComponent>().collider.x - 20,
                                                playerPos.y - e->getComponent<ColliderComponent>().collider.y  - 20);
            playerDirection = playerDirection.normalize();
            bool isCollapsed = e->getComponent<TransformComponent>().collapsed;
            if (isCollapsed)
                continue;
            e->getComponent<TransformComponent>().velocity = playerDirection;
            // flip the srpite 
            e->getComponent<TransformComponent>().direction = playerDirection.x <= 0 ? true : false;
            SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
            if (Collision::CheckCollision(eCol, playerCol)) {

                e->getComponent<SpriteComponent>().play("Attack");
                // std::cout << "Collision with: " << e->getComponent<ColliderComponent>().tag << std::endl;
                if (SDL_GetTicks() - lastHitTime >= 2000) {
                    lastHitTime = SDL_GetTicks();
                    //take damage
                    Player.getComponent<TransformComponent>().health -= e->getComponent<TransformComponent>().damage;
                    // show hurt sprite
                    Player.getComponent<Controller>().isHurt = true;
                    Player.getComponent<Controller>().lastHurtTime = SDL_GetTicks();
                    //hurt sound
                    Mix_PlayChannel(-1, assets->getSound("hurt"), 0);
                }
            }
            else{
                e->getComponent<SpriteComponent>().play("Run");
            }
        }

        //player shoot enemy
        for (auto& e : enemies) {
            SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
            for (auto& p : projectiles) {
                if (p->getComponent<ColliderComponent>().tag == "Player_projectile") {
                    SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
                    if (Collision::CheckCollision(pCol, eCol)) {
                        // std::cout << "Collision with: " << p->getComponent<ColliderComponent>().tag << std::endl;
                        p->destroy();
                        e->getComponent<TransformComponent>().health -= p->getComponent<TransformComponent>().damage;
                    }
                }
            }

            if (e->getComponent<TransformComponent>().health <= 0) {
                int timer = e->getComponent<TransformComponent>().timer;
                int currentTime = SDL_GetTicks();
                if(timer == 0){
                    e->getComponent<TransformComponent>().timer = currentTime;
                    e->getComponent<SpriteComponent>().play("Hurt");
                    e->getComponent<TransformComponent>().collapsed = true;
                    e->getComponent<TransformComponent>().velocity.Zero();
                }
                else if( currentTime - timer >= e->getComponent<TransformComponent>().collapseTimer){
                    score += e->getComponent<TransformComponent>().score;
                    e->destroy();
                }
                
            }
        }

        // Spawn new enemies every 5 seconds within 300 pixels around the player
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastEnemySpawnTime >= 5000) {
            Vector2D playerPos = Player.getComponent<TransformComponent>().position;


            int offsetX; 
            int offsetY;
            float distance;

            do {
                offsetX = rand() % 1001 - 500;
                offsetY = rand() % 801 - 400;
                distance = sqrt(pow(offsetX, 2) + pow(offsetY, 2));
            } while (distance <= 300);
            
            auto& newEnemy(manager.addEntity());

            int enemyID = rand() % maxEnemyID;

            newEnemy.addComponent<TransformComponent>(
                playerPos.x + offsetX, 
                playerPos.y + offsetY, 
                enemyW[enemyID], enemyH[enemyID], enemyScale[enemyID], enemySpeed[enemyID]
            );
            newEnemy.getComponent<TransformComponent>().setMaxHealth(enemyHealth[enemyID]);
            newEnemy.getComponent<TransformComponent>().damage = enemyDamage[enemyID];
            newEnemy.getComponent<TransformComponent>().score = enemyScore[enemyID];
            newEnemy.addComponent<SpriteComponent>(enemyName[enemyID], true);
            newEnemy.addComponent<ColliderComponent>(enemyName[enemyID]);
            newEnemy.addGroup(groupEnemies);
            lastEnemySpawnTime = currentTime; 
        }

        camera.x = Player.getComponent<TransformComponent>().position.x - 550;
        camera.y = Player.getComponent<TransformComponent>().position.y - 400;

        if (Player.getComponent<TransformComponent>().health <= 0) {
            gameOver();
        }
    } else if (menu) {
        manager.refresh();
        manager.update();
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 300 +300 && mouseY < 350 + 300) {
            start = true;
            startButton.getComponent<UI>().setColor(mint);
            startButton.getComponent<UI>().setPos(255, 305+300);
        } else {
            start = false;
            startButton.getComponent<UI>().setColor(white);
            startButton.getComponent<UI>().setPos(250, 300+300);
        }
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 350+300 && mouseY < 400+300) {
            settingsEnabled = true;
            settingsButton.getComponent<UI>().setColor(mint);
            settingsButton.getComponent<UI>().setPos(255, 355+300);
        } else {
            settingsEnabled = false;
            settingsButton.getComponent<UI>().setColor(white);
            settingsButton.getComponent<UI>().setPos(250, 350+300);
        }
        if (mouseX >= 250 && mouseX <= 500 && mouseY >= 400+300 && mouseY <= 450+300) {
            exit = true;
            exitButton.getComponent<UI>().setColor(mint);
            exitButton.getComponent<UI>().setPos(255, 405+300);
        } else {
            exit = false;
            exitButton.getComponent<UI>().setColor(white);
            exitButton.getComponent<UI>().setPos(250, 400+300);
        }
    }
    else {
        manager.refresh();
        manager.update();
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    if(pause){
        for (auto& t : tiles) {
            t->draw();
        }
        for (auto& c : colliders) {
            c->draw();
        }
        for (auto& p : players) {
            p->draw();
        }
        for (auto& e : enemies) {
            e->draw();
        }
        for (auto& p : projectiles) {
            p->draw();
        }
        Label.draw();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playerHealthBar);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect border = {playerHealthBar.x - 1, playerHealthBar.y - 1, 202, 22};
        SDL_RenderDrawRect(renderer, &border);

        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
        SDL_RenderFillRect(renderer, &overlay);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        resumeButton.draw();
        exitToMenuButtonPause.draw();
    }
    else if (gameOverScene) {
        //render game over scene
        gameover.draw();

        std::stringstream scoreText;
        scoreText << "Score: " << score;
        gameOverLabel.getComponent<UI>().setText(scoreText.str());
        gameOverLabel.draw();

        std::stringstream timeText;
        timeText << "Survived Time: " << survivedTime << "s";
        survivedTimeLabel.getComponent<UI>().setText(timeText.str());
        survivedTimeLabel.draw();

        backToMenuButton.draw();
    }  else if (menu) {
        //render the main menu
        background.draw();
        startButton.draw();
        settingsButton.draw();
        exitButton.draw();
        std::stringstream hs;
        if (highScore != 0) {
            hs << "High Score: " << highScore;
            HighScore.getComponent<UI>().setText(hs.str());
            HighScore.draw();
        }
    } else if (setting) {

        std::stringstream volumeText;
        volumeText << "Volume: " <<(static_cast<int>(volume * 100 / MIX_MAX_VOLUME)) << "%";
        volumeButton.getComponent<UI>().setText(volumeText.str());
        background.draw();
        volumeButton.draw();
        fullscreenButton.draw();
        exitToMenuButton.draw();
        volumeBar.w = static_cast<int>((250 * volume) / MIX_MAX_VOLUME); 
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
        SDL_RenderFillRect(renderer, &volumeBar);
        SDL_Rect volumeBarBorder = {volumeBar.x - 1, volumeBar.y - 1, 252, 22}; 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        SDL_RenderDrawRect(renderer, &volumeBarBorder);
        
    }
    else {
        //start game
        for (auto& t : tiles) {
            t->draw();
        }
        for (auto& c : colliders) {
            c->draw();
        }
        for (auto& p : players) {
            p->draw();
        }
        for (auto& e : enemies) {
            e->draw();
        }
        for (auto& p : projectiles) {
            p->draw();
        }
        Label.draw();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playerHealthBar);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect border = {playerHealthBar.x - 1, playerHealthBar.y - 1, 202, 22};
        SDL_RenderDrawRect(renderer, &border);
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}


void Game::gameOver() {
    std::cout << "Game Over" << std::endl;
    Mix_PlayChannel(-1, assets->getSound("gameOver"), 0);

    if (score > highScore) {
        highScore = score;
    }

    survivedTime = SDL_GetTicks() / 1000; 


    for (auto& t : tiles) {
        t->destroy();
    }
    for (auto& c : colliders) {
        c->destroy();
    }
    for (auto& e : enemies) {
        e->destroy();
    }
    for (auto& p : projectiles) {
        p->destroy();
    }

    menu = false;
    start = false;
    setting = false;
    settingsEnabled = false;

    gameOverScene = true;

    Player.removeAllComponents();

    camera = {0, 0, 1536, 1024};
}