#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

void Map::Draw()
{
    if(!mapLoaded)
        return;

    
    // L04: DONE 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;

   /* int i = 0;
    while (tileset != null) {
        i++;
        app->render->drawtexture(tileset->data->texture,50*i,0);
        tileset = tileset->next;
    }*/
    

    // L05: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL)
    {
        //L06: DONE 7: use GetProperty method to ask each layer if your ?Draw? property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    
                    switch (mapLayerItem->data->id)
                    {
                    case 6: Parallax(tileset, pos, r, 0);   break;  // ABS GB
                    case 5: Parallax(tileset, pos, r, 0.3); break;  // BG
                    case 4: Parallax(tileset, pos, r, 0);   break;  // PlayerPlane 2
                    case 3: Parallax(tileset, pos, r, 0);   break;  // PlayerPlane
                    case 2: Parallax(tileset, pos, r, 0);   break;  // Front
                    case 1: Parallax(tileset, pos, r, 0);   break;  // visuals
                    default: Parallax(tileset, pos, r, 0);  break;
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
}

// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
    iPoint ret;

    ret.x = x / mapData.tileWidth;
    ret.y = y / mapData.tileHeight;

    return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayer*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayer* layer = item->data;

        if (layer->name != "NAVIGATION" )
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    map[i] = tileId == 1027 ? 1 : 0;    // RED - no walk
                    map[i] = tileId == 1026 ? 10 : 0;    // YELLOW - high G cost
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}


// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// L06: DONE 2: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // L05: DONE 2: clean up all layer data
    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }
    mapData.maplayers.Clear();

    mapLoaded = false;

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // L07 DONE 3: Create colliders
    // Later you can create a function here to load and create the colliders from the map
    if (ret == true)
    {
        ret = CreateColliders(mapFileXML.child("map"));
    }


    if(ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// L05: DONE 4: Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);
        
        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

//Deprecated
bool Map::CreateCollidersOLD()
{
    bool ret = true;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL)
    {
        if (mapLayerItem->data->name == "COLLIDERS2")
        {
            int halfTileHeight = mapData.tileHeight / 2;
            int halfTileWidth = mapData.tileWidth / 2;

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    if (mapLayerItem->data->Get(x, y) != 0)
                    {
                        iPoint pos = MapToWorld(x, y);
                        PhysBody* c1 = app->physics->CreateRectangle(pos.x + halfTileHeight, pos.y + halfTileWidth, mapData.tileWidth, mapData.tileHeight, STATIC);

                        switch (mapLayerItem->data->Get(x, y))
                        { 
                        case 3139: c1->ctype = ColliderType::PLATFORM; break;
                        case 3140: c1->ctype = ColliderType::GROUND; break;
                        case 3141: c1->ctype = ColliderType::WALL; break;
                        case 3143: c1->ctype = ColliderType::DEATH; break;
                        default: break;
                        }
                    }                  
                }
            }
        }

        mapLayerItem = mapLayerItem->next;
    }
    return ret;
}

bool Map::CreateColliders(pugi::xml_node mapFile)
{
    bool ret = true;

    pugi::xml_node objectgroup = mapFile.child("objectgroup");

    for (pugi::xml_node objectgroup = mapFile.child("objectgroup"); objectgroup && ret; objectgroup = objectgroup.next_sibling("objectgroup"))
    {
        // TRIGGERS
        if ((SString)objectgroup.attribute("name").as_string() == "TRIGGERS")
        {
            for (pugi::xml_node object = objectgroup.child("object"); object && ret; object = object.next_sibling("object"))
            {
                PhysBody* c1 = app->physics->CreateRectangleSensor(
                    object.attribute("x").as_int() + object.attribute("width").as_int() / 2,
                    object.attribute("y").as_int() + object.attribute("height").as_int() / 2,
                    object.attribute("width").as_int(),
                    object.attribute("height").as_int(), STATIC);

                pugi::xml_node type = object.child("properties").child("property");


                if ((SString)type.attribute("value").as_string() == "TRIG_1A")      { c1->ctype = ColliderType::TRIG_1A; }
                else if ((SString)type.attribute("value").as_string() == "TRIG_1R") { c1->ctype = ColliderType::TRIG_1R; }
                else if ((SString)type.attribute("value").as_string() == "TRIG_2A") { c1->ctype = ColliderType::TRIG_2A; }
                else if ((SString)type.attribute("value").as_string() == "TRIG_2R") { c1->ctype = ColliderType::TRIG_2R; }

            }
        }

        // MAP COLLIDERS
        if ((SString)objectgroup.attribute("name").as_string() == "COLLIDERS")
        {
            for (pugi::xml_node object = objectgroup.child("object"); object && ret; object = object.next_sibling("object"))
            {
                PhysBody* c1 = app->physics->CreateRectangle(
                    object.attribute("x").as_int() + object.attribute("width").as_int() / 2,
                    object.attribute("y").as_int() + object.attribute("height").as_int() / 2,
                    object.attribute("width").as_int(),
                    object.attribute("height").as_int(), STATIC);

                pugi::xml_node type = object.child("properties").child("property");


                     if ((SString)type.attribute("value").as_string() == "GROUND")   { c1->ctype = ColliderType::GROUND; }
                else if ((SString)type.attribute("value").as_string() == "PLATFORM") { c1->ctype = ColliderType::PLATFORM; }
                else if ((SString)type.attribute("value").as_string() == "WALL")     { c1->ctype = ColliderType::WALL; }
                else if ((SString)type.attribute("value").as_string() == "CEILING")  { c1->ctype = ColliderType::CEILING; }
                else if ((SString)type.attribute("value").as_string() == "DEATH")    { c1->ctype = ColliderType::DEATH; }
            }
        }
    }

    return ret;
}

void Map::Parallax(TileSet* tileset, iPoint pos, SDL_Rect r, float x)
{
    app->render->DrawTexture(tileset->texture,
        pos.x - (app->render->camera.x * (x / app->win->GetScale())),
        pos.y,
        SDL_FLIP_NONE,
        &r);
}

// L06: DONE 7: Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}