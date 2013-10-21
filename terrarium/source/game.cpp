/*
 * game.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "../header/game.hpp"
#include "../header/rapidxml.hpp"



Game::Game()
: player(NULL),
visibleArea(0,0,640,480),
map(NULL),
green_box(NULL),
eventQueue(new Engine::Event::Queue()),
font(new Engine::Font("resource/liberation.ttf", 14)),
running(true),
jumping(false),
isKeyUpPressed(false),
isKeyDownPressed(false),
isKeyRightPressed(false),
isKeyLeftPressed(false)
{

	if(eventQueue == null)
		COUT << "DEU ROLO" << ENDL;


	b2Vec2 gravity(0.0f, 10.0f);
	world = new b2World(gravity);
}

void Game::start()
{
	Image* i = new Image("resource/pijamaman-1.png");
	AnimationSet* anim = new AnimationSet(i);
	anim->add("still-left", 56, 84, 1, 1);
	anim->add("still-right", 56, 84, 1, 1);
	anim->add("walk-left", 56, 84, 0.3, 4);
	anim->add("walk-right", 56, 84, 0.3, 4);
	anim->setCurrent("still-right");
	Body* b = new Body(1,1,convertToMeters(16),convertToMeters(80));
	Entity* e = new Entity(anim, b);
	e->body->setDynamic();
	e->body->addBodytoWorld(world);
	e->body->body->SetFixedRotation(true);

	entities.push_back(e);
	player = entities[0];
	green_box = new Image("resource/tileset-dirt.png");

	FileInputStream stream("resource/map.tmx");
	String s = "";
	while (stream.good())
	{
		s += stream.get();
	}

	s = s.substr(0, s.size()-1); //removing EOF

	using namespace rapidxml;

	xml_document<> doc;

	char * c = (char *) s.c_str();

	doc.parse<0>(c);

	COUT << "Name of my first node is: " << doc.first_node()->name() << "\n";
	xml_node<> *node = doc.first_node();
	COUT << "Node foobar has value " << node->value() << "\n";
	for (xml_attribute<> *attr = node->first_attribute();
	     attr; attr = attr->next_attribute())
	{
	    COUT << "Node foobar has attribute " << attr->name() << " ";
	    COUT << "with value " << attr->value() << "\n";
	}


	delete map;
	map = Map::loadRawMapFromFile(string("resource/rawmap.txt"), world);
	map->visibleArea = &visibleArea;


	while(running)
	{
		visibleArea.x = convertToPixels(player->body->getX()) - visibleArea.w/2.0;
		visibleArea.y = convertToPixels(player->body->getY()) - visibleArea.h/2.0;
		handleInput();
		drawScene();
	}

	Map::saveRawMapToFile(string("resource/rawmap.txt"), map);

	delete e;
	delete i;
}

void Game::handleInput()
{

	if(isKeyUpPressed && !jumping) {
		b2Vec2 v = player->body->body->GetLinearVelocity();
		if(v.y >= -2.0f) {
			player->body->body->ApplyLinearImpulse(b2Vec2(0.0f, -0.05f), b2Vec2(player->body->getX(), player->body->getY()));
			jumping = true;
		}
	}
	if(isKeyDownPressed) {
		b2Vec2 v = player->body->body->GetLinearVelocity();
		if(v.y <= 2.0f)
			player->body->body->ApplyForceToCenter(b2Vec2(0.0f, 0.2f));
	}
	if(isKeyRightPressed) {
		player->animation->setCurrent("walk-right");
		b2Vec2 v = player->body->body->GetLinearVelocity();
		if(v.x <= 2.0f)
			player->body->body->ApplyForceToCenter(b2Vec2(0.2f, 0.0f));
	}
	if(isKeyLeftPressed) {
		player->animation->setCurrent("walk-left");
		b2Vec2 v = player->body->body->GetLinearVelocity();
		if(v.x >= -2.0f)
			player->body->body->ApplyForceToCenter(b2Vec2(-0.2f, 0.0f));
	}

	while(! eventQueue->isEmpty() )
	{
		Engine::Event* ev = eventQueue->waitForEvent();

		if(ev->getEventType() == Engine::Event::Type::DISPLAY_CLOSURE)
		{
			running=false;
		}
		else if(ev->getEventType() == Engine::Event::Type::KEY_RELEASE)
		{
			switch(ev->getEventKeyCode())
			{
			case Engine::Event::Key::ARROW_UP:
				isKeyUpPressed = false;
				jumping = false;
				break;
			case Engine::Event::Key::ARROW_DOWN:
				isKeyDownPressed = false;
				break;
			case Engine::Event::Key::ARROW_RIGHT:
				player->animation->setCurrent("still-right");
				isKeyRightPressed = false;
				break;
			case Engine::Event::Key::ARROW_LEFT:
				player->animation->setCurrent("still-left");
				isKeyLeftPressed = false;
				break;
			default:
				break;
			}
		}
		else if(ev->getEventType() == Engine::Event::Type::KEY_PRESS)
		{
			switch(ev->getEventKeyCode())
			{
			case Engine::Event::Key::ARROW_UP:
				isKeyUpPressed = true;
				break;
			case Engine::Event::Key::ARROW_DOWN:
				isKeyDownPressed = true;
				break;
			case Engine::Event::Key::ARROW_RIGHT:
				isKeyRightPressed = true;
				break;
			case Engine::Event::Key::ARROW_LEFT:
				isKeyLeftPressed = true;
				break;
			default:
				break;
			}
		}
		else if(ev->getEventType() == Engine::Event::Type::MOUSE_BUTTON_PRESS)
		{

			if(ev->getEventMouseButton() == Engine::Event::MouseButton::RIGHT)
			{
				unsigned int mx = (visibleArea.x + ev->getEventMouseX())/BLOCK_SIZE;
				unsigned int my = (visibleArea.y + ev->getEventMouseY())/BLOCK_SIZE;

				if(mx < map->grid.capacity() && my < map->grid[0].capacity()) // in case you click outside the map
					if (map->grid[mx][my] == NULL)
					{
						map->grid[mx][my] = new Block(green_box, mx, my);
						map->grid[mx][my]->body->addBodytoWorld(world);
						map->retile(map->grid[mx][my]);
					}

			}
			else if (ev->getEventMouseButton() == Engine::Event::MouseButton::LEFT)
			{
				unsigned int mx = (visibleArea.x + ev->getEventMouseX())/BLOCK_SIZE;
				unsigned int my = (visibleArea.y + ev->getEventMouseY())/BLOCK_SIZE;

				if(mx < 0) mx = 0; // safety
				if(my < 0) my = 0; // safety
				if(mx < map->grid.capacity() && my < map->grid[0].capacity()) // in case you click outside the map
					if (map->grid[mx][my] != NULL)
					{
						map->grid[mx][my]->body->destroyBody(world);
						delete map->grid[mx][my];
						map->grid[mx][my] = NULL;
						map->retileNeighbourhood(mx, my);
					}
			}
		}
	}

}

void Game::drawScene()
{
	Engine::display->clear();
	world->Step((1.0f / 60.0f), 6, 2);
	/* needs to draw HUD */

	map->draw();


	/* drawing all */
	for(vector<Entity*>::iterator it = entities.begin() ; it != entities.end(); ++it){
		if (*it != NULL) (*it)->draw(&visibleArea);
	}

	/* later should be a character class */

	//DEBUG!!!
	drawDebug();


	Engine::display->refresh();
}

void Game::drawDebug()
{
	font->draw_text("## DEBUG BUILD ##", 245, 0, Engine::Color::RED);

	font->draw_text("POSITION", 0, 14, Engine::Color::WHITE);

	font->draw_text(String("x: ")+player->body->getX()+" y:"+player->body->getY(), 0, 28, Engine::Color::WHITE);

	font->draw_text("SPEED", 0, 42, Engine::Color::WHITE);

	font->draw_text(String("x: ")+player->body->getVelocity().x+" y: "+player->body->getVelocity().y, 0, 56, Engine::Color::WHITE);

}
