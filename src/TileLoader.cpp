/*
 *  TileLoader.cpp
 *  modestmaps-ci
 *
 *  Created by Tom Carden on 8/27/10.
 *  Copyright 2010 Stamen Design. All rights reserved.
 *
 */

#include "TileLoader.h"
#include "cinder/Thread.h"

namespace cinder { namespace modestmaps {

class TileLoaderImpl 
{
public:

    TileLoaderImpl( MapProviderRef _provider ): provider(_provider) {}
    
	std::mutex pendingCompleteMutex;
	std::set<Coordinate> pending;
	std::map<Coordinate, Surface> completed;
    MapProviderRef provider;
    
    void doThreadedPaint( const Coordinate &coord );    
	void processQueue( std::vector<Coordinate> &queue );
	void transferTextures( std::map<Coordinate, gl::Texture> &images);
	bool isPending(const Coordinate &coord);
    void setMapProvider( MapProviderRef _provider );    
};
    
TileLoader::TileLoader( MapProviderRef _provider )
{
    impl = new TileLoaderImpl( _provider );
}

TileLoader::~TileLoader()
{
    delete impl;
}

void TileLoader::processQueue( std::vector<Coordinate> &queue )
{
    impl->processQueue(queue);
}
void TileLoader::transferTextures( std::map<Coordinate, gl::Texture> &images)
{
    impl->transferTextures(images);
}
bool TileLoader::isPending(const Coordinate &coord)
{
    return impl->isPending(coord);
}    
void TileLoader::setMapProvider( MapProviderRef _provider )
{
    impl->setMapProvider(_provider);
}
void TileLoader::doThreadedPaint( const Coordinate &coord )
{
    impl->doThreadedPaint(coord);
}

    
void TileLoaderImpl::doThreadedPaint( const Coordinate &coord )
{
    ThreadSetup threadSetup;

	Surface image;
    
    if (provider) {
        image = provider->createSurface( coord );
    }
    
	pendingCompleteMutex.lock();
    if (pending.count(coord) > 0) {
        if (image) {
            completed[coord] = image;
        }
        pending.erase(coord);  
    } // otherwise clear was called so we should abandon this image to the ether
	pendingCompleteMutex.unlock();
}

void TileLoaderImpl::processQueue(std::vector<Coordinate> &queue )
{
	while (pending.size() < MAX_PENDING && queue.size() > 0) {
		Coordinate key = *(queue.begin());
		queue.erase(queue.begin());

        pendingCompleteMutex.lock();
        pending.insert(key);
        pendingCompleteMutex.unlock();	
        
        // TODO: consider using a single thread and a queue, rather than a thread per load?
        std::thread loaderThread( &TileLoaderImpl::doThreadedPaint, this, key );        
	}
}

void TileLoaderImpl::transferTextures(std::map<Coordinate, gl::Texture> &images)
{
    gl::Texture::Format format;
    format.enableMipmapping( true );
    format.setMinFilter( GL_LINEAR ); //GL_LINEAR OR GL_NEAREST
    format.setMagFilter( GL_LINEAR );
    format.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    
    // use try_lock because we can just wait until next frame if needed
    if (pendingCompleteMutex.try_lock()) {
        if (!completed.empty()) {
            std::map<Coordinate, Surface>::iterator iter = completed.begin();
            if (iter->second) {
                images[iter->first] = gl::Texture(iter->second, format);
            }
            completed.erase(iter);
        }
        pendingCompleteMutex.unlock();
    }
}
    
bool TileLoaderImpl::isPending(const Coordinate &coord)
{
    bool coordIsPending = false;
    pendingCompleteMutex.lock();
    coordIsPending = (pending.count(coord) > 0);
    pendingCompleteMutex.unlock();
    return coordIsPending;
}
    
void TileLoaderImpl::setMapProvider( MapProviderRef _provider )
{
	pendingCompleteMutex.lock();
    completed.clear();
    pending.clear();
	pendingCompleteMutex.unlock();
    provider = _provider;
}

} } // namespace 