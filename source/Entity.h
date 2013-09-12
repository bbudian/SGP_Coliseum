/**************************************************
|	File:		Entity.h
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CEntity class is used for a 
|				velocity-based game object. 
|				It is the parent class for all 
|				other objects.
**************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include "IEntity.h"
#include "../SGD Wrappers/SGD_Math.h"

/*************************************************/
// CEntity class
//	- concrete base class for game objects
class CEntity : public IEntity
{
public:
	enum	EntityType		{ ENT_BASE }; // Add More as needed

	///////////////////////////////////////////////
	// Destructor MUST be virtual to prevent 
	// memory leaks in dynamically-allocated children
	CEntity( void );
	virtual ~CEntity( void );	

	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void Update			( float fElapsedTime )				override;
	virtual void Render			( void )							override;

	virtual int  GetType		( void ) const						override	{	return entity_type;		}
	virtual RECT GetRect		( void ) const						override;
	virtual void HandleCollision( const IEntity* pOther )			override;

	virtual void AddRef			( void )							final;
	virtual void Release		( void )							final;

	///////////////////////////////////////////////
	// Accessors:
	int		GetImageID	( void ) const	{	return	image_id;	}
	int		GetWidth	( void ) const	{	return	width;	}
	int		GetHeight	( void ) const	{	return	height;	}
	float	GetPosX		( void ) const	{	return	position.fX;	}
	float	GetPosY		( void ) const	{	return	position.fY;	}
	float	GetVelX		( void ) const	{	return	velocity.fX;	}
	float	GetVelY		( void ) const	{	return	velocity.fY;	}
	float	GetRotation	( void ) const	{	return	rotation;}

	///////////////////////////////////////////////
	// Mutators:
	void	SetImageID	( int	id		)	{	image_id	= id;		}
	void	SetWidth	( int	w		)	{	width		= w ;		}
	void	SetHeight	( int	h		)	{	height		= h ;		}
	void	SetPosX		( float	x		)	{	position.fX	= x ;		}
	void	SetPosY		( float	y		)	{	position.fY	= y ;		}
	void	SetVelX		( float	vx		)	{	velocity.fX	= vx;		}
	void	SetVelY		( float	vy		)	{	velocity.fY	= vy;		}
	void	SetRotation	( float r		)	{	rotation	= r;		}

protected:
	///////////////////////////////////////////////
	// entity type
	EntityType			entity_type;

private:
	///////////////////////////////////////////////
	// image ID
	int			image_id;

	// position
	tVector2D position;

	// velocity
	tVector2D velocity;

	// rotation
	float		rotation;	

	// size
	int			width;
	int			height;

	// reference count
	unsigned int	reference_count;

};

#endif //ENTITY_H

