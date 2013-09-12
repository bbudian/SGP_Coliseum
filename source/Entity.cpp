/**************************************************
|	File:		Entity.cpp
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CEntity class is used for a 
|				velocity-based game object. 
|				It is the parent class for all 
|				other objects.
**************************************************/

#include "Entity.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"

#include <cassert>

/*************************************************/
// Constructor
//	- set ALL data members to 'clean' values
CEntity::CEntity( void )
{
	// Set the entity's type
	entity_type		= ENT_BASE;

	image_id	= -1;
	position.fX		= 0;
	position.fY		= 0;
	velocity.fX		= 0;
	velocity.fY		= 0;
	rotation	= 0;
	width	= 0;
	height	= 0;

	reference_count	= 1;	// calling 'new' gives the 'prime' reference
}

/*************************************************/
// Destructor
CEntity::~CEntity( void ){}

/*************************************************/
// Update
//	- move the entity by velocity each frame
//	  (velocity is pixels per second)
/*virtual*/ void CEntity::Update( float fElapsedTime )
{
	position.fX += velocity.fX * fElapsedTime;
	position.fY += velocity.fY * fElapsedTime;
}

/*************************************************/
// Render
//	- render the entity's image at its position
/*virtual*/ void CEntity::Render( void )
{
	// Validate the image
	assert( image_id != -1 && "CEntity::Render - image was not set" );

	// Use the Texture Manager singleton
	CSGD_TextureManager::GetInstance()->Draw(
		image_id, (int)position.fX, (int)position.fY,
		1.0f, 1.0f, nullptr,
		(float)(width/2), (float)(height/2), rotation );
}

/*************************************************/
// GetRect
//	- return the entity's bounding rectangle
/*virtual*/ RECT CEntity::GetRect( void ) const
{
	RECT rSelf = 
	{
		(LONG)position.fX,
		(LONG)position.fY,
		(LONG)(position.fX + width),
		(LONG)(position.fY + height)
	};

	return rSelf;
}

/*************************************************/
// HandleCollision
//	- respond to collision with other entity
/*virtual*/ void CEntity::HandleCollision( const IEntity* pOther )
{
	/* CEntity does not care about collision */
	(void)pOther;
}

/*************************************************/
// AddRef
//	- increase the reference count
/*virtual*/ void CEntity::AddRef( void )
{
	assert( reference_count != UINT_MAX && "CEntity::AddRef - reference count has exceeded max" );
	++reference_count;
}

/*************************************************/
// Release
//	- decrease the reference count
//	- self-destruct when the count reaches 0
/*virtual*/ void CEntity::Release( void )
{
	--reference_count;

	if( reference_count == 0 )
		delete this;
}
