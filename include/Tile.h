//
//  Tile.h
//  Modest Maps Cinder
//
//  Created by Tom Carden on 4/29/12.
//  Copyright (c) 2012 Bloom Studio, Inc. All rights reserved.
//

#pragma once

#include "Coordinate.h"
#include "cinder/Cinder.h"
#include "cinder/gl/Texture.h"

namespace cinder { namespace modestmaps {

typedef std::shared_ptr<class Tile> TileRef;
    
class Tile
{
public:
    
    static TileRef create(const Coordinate &coord)
    {
        return TileRef(new Tile(coord));
    }
    
    Coordinate getCoord()
    {
        return coord;
    }
    
    void setCoord( const Coordinate &_coord )
    {
        coord = _coord;
    }

    
    float getLastAddedTime()
    {
        return lastAddedTime;
    }
    
    void setLastAddedTime( const float &_lastAddedTime )
    {
        lastAddedTime = _lastAddedTime;
    }
    
    ci::gl::Texture getTexture()
    {
        return texture;
    }
    
    void setTexture( ci::gl::Texture _texture )
    {
        texture = _texture;
    }    
    
    bool isComplete()
    {
        return texture;
    }
    
private:
    
    Tile( const Coordinate &_coord ): coord(_coord), lastAddedTime(-1) {}
    
    Coordinate coord;
    float lastAddedTime;
    ci::gl::Texture texture;
    
};

} } // namespaces