#pragma once

#include <set>
#include <map>

#include "cinder/Cinder.h"
#include "cinder/gl/Texture.h"

#include "Coordinate.h"
#include "MapProvider.h"
#include "Tile.h"

namespace cinder { namespace modestmaps {

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

    // how many things are we currently requesting?
    int getPendingCount();
    
    // how many surfaces await Texture creation?
    int getCompletedCount();
    
    // start loading a texture for the given coord in the background
	void fetchCoord( const Coordinate &coord );
	
    // create Texture for next finished Surface and return a new Tile
	TileRef getNextCompletedTile();
    
    // true if we're loading this tile (false if it's done loading already)
	bool hasCoord( const Coordinate &coord );
    
    // Clear all the pending/complete tiles and set a new map provider
    void setMapProvider( MapProviderRef _provider );
    
};
    
} } // namespace 