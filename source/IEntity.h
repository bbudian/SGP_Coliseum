/**************************************************
|	File:		IEntity.h
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD
|	Purpose:	IEntity class is the base INTERFACE
|				for all game objects
**************************************************/

#ifndef IENTITY_H
#define IENTITY_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


/*************************************************/
// IEntity class
//	- interface base class for game objects
//	- declares pure virtual methods & has no data members
//	- interfaces are used to abstract the concrete types out
//	  of the systems that use them (to be more flexible)
class IEntity
{
public:
	///////////////////////////////////////////////
	// PURE VIRTUAL METHOD
	//	- creates an Abstract Base Class
	//	- cannot instantiate an IEntity
	//	- MUST be implemented in the child class
	//	  (else the child will also be abstract)
	virtual ~IEntity( void )	= 0
	{
	}
	
	///////////////////////////////////////////////
	// IEntity Interface:
	virtual void Update( float fElapsedTime )	= 0;
	virtual void Render( void )					= 0;

	virtual int  GetType( void ) const			= 0;
	virtual RECT GetRect( void ) const			= 0;
	virtual void HandleCollision( const IEntity* pOther )	= 0;


	// Reference Counting
	virtual void AddRef ( void )				= 0;
	virtual void Release( void )				= 0;
};

#endif //IENTITY_H
