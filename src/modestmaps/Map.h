
#ifndef MODEST_MAP
#define MODEST_MAP

#include <set>
#include <map>

#include "AbstractMapProvider.h"
#include "Location.h"
#include "Coordinate.h"
#include "QueueSorter.h"
#include "TileLoader.h"

#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

using namespace ci;
using namespace ci::app;

#define TILE_SIZE 256.0

// limit tiles in memory
// 256 would be 64 MB, you may want to lower this quite a bit for your app
// (we'll always keep as many images as needed to fill the screen though)
#define MAX_IMAGES_TO_KEEP 256

// upping this can help appearances when zooming out, but also loads many more tiles
#define GRID_PADDING 0


class Map {
	
private:

	// how big?
	double width, height;
		
	// pan and zoom
	Coordinate centerCoordinate;
	
	// angle in radians
	double rotation;
	
	// what kinda maps?
	AbstractMapProvider* provider;
	
	// loading tiles
	TileLoader tileLoader;

	// loaded tiles
	std::map<Coordinate, gl::Texture> images;
	
	// coords waiting to load
	std::vector<Coordinate> queue;
	
	// a list of the most recent MAX_IMAGES_TO_KEEP ofImages we've seen
	std::vector<gl::Texture> recentImages;
	
	// keep track of what we can see already:
	std::set<Coordinate> visibleKeys;

	void grabTile(const Coordinate &coord);
	
	void processQueue();

public:	
	
	Map() {}
	
	void setup(AbstractMapProvider* _provider, double _width, double _height);	
	void update();
	void draw();
		
	void panBy(double x, double y);
	void scaleBy(double s);
	void scaleBy(double s, double x, double y);
	void rotateBy(double r, double x, double y);
	void zoomBy(double distance);

	void setZoom(double zoom);
	int getZoom(); // TODO: maybe double getZoom, int getZoomLevel?
	Location getCenter();
	Coordinate getCenterCoordinate();
	
	void setCenter(const Coordinate &center);
	void setCenter(const Location &location);
	void panTo(const Location &location); // same as setCenter	
	void setCenterZoom(const Location &location, int zoom);
	
	// TODO: extent functions
	//	    public function setExtent(extent:MapExtent):void
	//	    public function getExtent():MapExtent
	
	// TODO: make it so you can safely get and set the provider
	/*AbstractMapProvider getMapProvider() {
	    return provider;
	}
	
	void setMapProvider(AbstractMapProvider _provider) {
		provider = _provider;
		images.clear();
		queue.clear();
		pending.clear();
	}*/
	
	Vec2d coordinatePoint(const Coordinate &coord);
	Coordinate pointCoordinate(const Vec2d &point);

	Vec2d locationPoint(const Location &location);
	Location pointLocation(const Vec2d &point);

	void zoomIn();	
	void zoomOut();
		
	void panUp();
	void panDown();
	void panLeft();
	void panRight();	

	void panAndZoomIn(const Location &location);
	
	void setSize(double _width, double _height);
};

#endif
