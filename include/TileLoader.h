#pragma once

#include <set>
#include <map>

#include "cinder/Cinder.h"
#include "cinder/gl/Texture.h"

#include "Coordinate.h"
#include "MapProvider.h"

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
    
    void doThreadedPaint( const Coordinate &coord );
    
    // using an impl here because cinder/Thread.h was giving Objective-C the hiccups
    TileLoaderImpl *impl;
        
public:
    
    static TileLoaderRef create( MapProviderRef provider )
    {
        return TileLoaderRef( new TileLoader( provider ) );
    }
    
    ~TileLoader();
    
	void processQueue( std::vector<Coordinate> &queue );
	
	void transferTextures( std::map<Coordinate, gl::Texture> &images );

	bool isPending( const Coordinate &coord );
    
    void setMapProvider( MapProviderRef _provider );
    
};
    
} } // namespace 