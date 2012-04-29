#pragma once

#include <set>
#include <map>

#include "MapProvider.h"
#include "Location.h"
#include "MapExtent.h"
#include "Coordinate.h"
#include "QueueSorter.h"
#include "TileLoader.h"

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Timer.h"

namespace cinder { namespace modestmaps {
	
// TODO: public getter/setting, and/or Settings object for these values?
    
// limit tiles in memory
// 256 would be 64 MB, you may want to lower this quite a bit for your app
// (we'll always keep as many images as needed to fill the screen though)
#define MAX_IMAGES_TO_KEEP 256

// upping this can help appearances when zooming out, but also loads many more tiles
#define GRID_PADDING 0


class Map {
	
private:

	// how big?
	Vec2d size;
		
	// pan and zoom
	Coordinate centerCoordinate;
	
	// angle in radians
	double rotation;
		
	// about the tiles
	MapProviderRef mapProvider;
    
    // loading tiles
    TileLoaderRef tileLoader;

	// loaded tiles
	std::map<Coordinate, gl::Texture> images;
	
	// coords waiting to load
	std::vector<Coordinate> queue;
	
	// a list of the most recent MAX_IMAGES_TO_KEEP images we've seen
	std::vector<Coordinate> recentImages;

    // loaded tiles by time added to screen (parallel with images)
	std::map<Coordinate, float> imageAddedTimes;

	// keep track of what we can see already:
	std::set<Coordinate> visibleKeys;

    Timer timer;
    
	void grabTile(const Coordinate &coord);
	
	void processQueue();

    void drawTile(const ci::gl::Texture &texture, const ci::Matrix44f &mat, const ci::ColorA &color);
    
public:	
	
	Map() {}
	
    void setup( MapProviderRef _provider, Vec2d size );
    
	void update();
	void draw();
    
	void panBy(const double &x, const double &y);
	void panBy(const Vec2d &d);
	void scaleBy(const double &s);
	void scaleBy(const double &s, const double &x, const double &y);
	void scaleBy(const double &s, const Vec2d &c);
	void rotateBy(const double &r, const double &x, const double &y);
	void rotateBy(const double &r, const Vec2d &c);
	void zoomBy(const double &distance);

	void setZoom(const double &zoom);
	double getZoom() const; // TODO: maybe int getZoomLevel as well?
	Location getCenter() const;
	Coordinate getCenterCoordinate() const;
	
	void setCenter(const Coordinate &center);
	void setCenter(const Location &location);
	void panTo(const Location &location); // same as setCenter	
	void setCenterZoom(const Location &location, const double &zoom);
	
    void setExtent(const MapExtent &extent, bool forceIntZoom = true);
    MapExtent getExtent() const;
	
	MapProviderRef getMapProvider() const {
	    return mapProvider;
	}
	
    void setMapProvider( MapProviderRef _provider);
	
	Vec2d coordinatePoint(const Coordinate &coord) const;
	Coordinate pointCoordinate(const Vec2d &point) const;

	Vec2d locationPoint(const Location &location) const;
	Location pointLocation(const Vec2d &point) const;

	void zoomIn();	
	void zoomOut();
		
	void panUp();
	void panDown();
	void panLeft();
	void panRight();	

	void panAndZoomIn(const Location &location);
	
	void setSize(double _width, double _height);
	void setSize(Vec2d size);
	Vec2d getSize() const;
};

} } // namespace