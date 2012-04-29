#pragma once

#include <set>
#include <map>

#include "cinder/Cinder.h"
#include "cinder/gl/Texture.h"

#include "Coordinate.h"
#include "MapProvider.h"
#include "Tile.h"

namespace cinder { namespace modestmaps {

// limit simultaneous calls to loadImage
#define MAX_PENDING 2

class TileLoader;
typedef std::shared_ptr<TileLoader> TileLoaderRef;

class TileLoaderImpl;
    
class TileLoader
{
	
private:
    
    TileLoader( MapProviderRef _provider );
        
    // using an impl here because cinder/Thread.h was giving Objective-C the hiccups
    TileLoaderImpl *impl;
        
public:
    
    static TileLoaderRef create( MapProviderRef provider )
    {
        return TileLoaderRef( new TileLoader( provider ) );
    }
    
    ~TileLoader();
    
    // pull things off the given queue and start loading them in the background
	void processQueue( std::vector<Coordinate> &queue );
	
    // create Tiles and Textures for finished Surfaces and add them to the given map
	void transferTextures( std::map<Coordinate, TileRef> &tiles );

    // true if we're loading this tile (false if it's done loading already)
	bool hasCoord( const Coordinate &coord );
    
    // Clear all the pending/complete tiles and set a new map provider
    void setMapProvider( MapProviderRef _provider );
    
};
    
} } // namespace 