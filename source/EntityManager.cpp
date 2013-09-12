/**************************************************
|	File:		EntityManager.cpp
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD
|	Purpose:	CEntityManager class stores 
|				& maintains all game entities.
**************************************************/


#include "EntityManager.h"

#include "IEntity.h"

#include <cassert>


/*************************************************/
// Constructor
CEntityManager::CEntityManager( void )
{
	m_nSize = 0;
	m_bIterating = false;
}

/*************************************************/
// Destructor
CEntityManager::~CEntityManager( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "~CEntityManager - cannot be destroyed while iterating!" );
}


/*************************************************/
// AddEntity
//	- add the entity into the table in the specified bucket
//	  & store a reference to the entity
void CEntityManager::AddEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::AddEntity - cannot add while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::AddEntity - pointer cannot be null" );

	// Expand the vector?
	if( unType >= m_tEntities.size() )
		m_tEntities.resize( unType + 1 );


	// Add the entity to the end of the bucket
	m_tEntities[ unType ].push_back( pEntity );
	pEntity->AddRef();

	m_nSize++;
}

/*************************************************/
// RemoveEntity
//	- remove & release the entity from any bucket
void CEntityManager::RemoveEntity( IEntity* pEntity )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );

	// Try to find the entity
	for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
	{
		EntityVector& vec = m_tEntities[ type ];
		for( int i = (int)vec.size() - 1; i >= 0; i-- )
		{
			if( vec[ i ] == pEntity )
			{
				// Remove the entity
				pEntity->Release();
				vec.erase( vec.begin() + i );
				m_nSize--;
				return;
			}
		}
	}

}

/*************************************************/
// RemoveEntity
//	- remove & release an entity from a specific bucket
void CEntityManager::RemoveEntity( IEntity* pEntity, unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveEntity - cannot remove while iterating" );

	// Validate the parameters
	assert( pEntity != nullptr && "CEntityManager::RemoveEntity - pointer cannot be null" );
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveEntity - invalid type" );

	// Try to find the entity
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		if( vec[ i ] == pEntity )
		{
			// Remove the entity
			pEntity->Release();
			vec.erase( vec.begin() + i );
			m_nSize--;
			break;
		}
	}
}

/*************************************************/
// RemoveAll
//	- remove & release all the entities from a specific bucket
void CEntityManager::RemoveAll( unsigned int unType )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );

	// Validate the parameter
	assert( unType < m_tEntities.size() && "CEntityManager::RemoveAll - invalid type" );

	// Release each entity in the bucket
	EntityVector& vec = m_tEntities[ unType ];
	for( int i = (int)vec.size() - 1; i >= 0; i-- )
	{
		vec[ i ]->Release();
	}

	// Collapse the vector
	m_tEntities[ unType ].clear();
	m_nSize = 0;
}

/*************************************************/
// RemoveAll
//	- remove all entities from the table
void CEntityManager::RemoveAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RemoveAll - cannot remove while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Release every entity
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Release();
			}
		}

		// Collapse the table
		m_tEntities.clear();
	}
	// Unlock the iterator
	m_bIterating = false;
	m_nSize = 0;
}

/*************************************************/
// UpdateAll
//	- update each entity
void CEntityManager::UpdateAll( float fElapsedTime )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::UpdateAll - cannot update while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Update( fElapsedTime );
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

/*************************************************/
// RenderAll
//	- render each entity
void CEntityManager::RenderAll( void )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::RenderAll - cannot render while iterating" );


	// Lock the iterator
	m_bIterating = true;
	{
		// Loop through the buckets
		for( int type = (int)m_tEntities.size()-1; type >= 0; type-- )
		{
			// Loop through the entities
			EntityVector& vec = m_tEntities[ type ];
			for( int i = (int)vec.size() - 1; i >= 0; i-- )
			{
				vec[ i ]->Render();
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}

/*************************************************/
// CheckAllCollisions
//	- check collision between the entities within the two buckets
void CEntityManager::CheckAllCollisions( unsigned int unType1, unsigned int unType2 )
{
	// Validate the iteration state
	assert( m_bIterating == false && "CEntityManager::CheckAllCollisions - cannot collide while iterating" );

	// Quietly validate the parameters
	if( unType1 >= m_tEntities.size() 
		|| unType2 >= m_tEntities.size()
		|| m_tEntities[ unType1 ].size() == 0 
		|| m_tEntities[ unType2 ].size() == 0 )
		return;


	// Lock the iterator
	m_bIterating = true;
	{
		// Are they different buckets?
		if( unType1 != unType2 )
		{
			// Which bucket is smaller?
			//	should be the outer loop for less checks (n0)*(n1+1) + 1
			EntityVector* pVec1 = &m_tEntities[ unType1 ];
			EntityVector* pVec2 = &m_tEntities[ unType2 ];

			if( pVec2->size() < pVec1->size() )
			{
				EntityVector* pTemp = pVec1;
				pVec1 = pVec2;
				pVec2 = pTemp;
			}

			// Iterate through the smaller bucket
			for( int i = (int)(pVec1->size()-1); i >= 0; i-- )
			{
				// Iterate through the larger bucket
				for( int j = (int)(pVec2->size()-1); j >= 0; j-- )
				{
					// Helper pointers
					IEntity* pEntity1 = (*pVec1)[ i ];
					IEntity* pEntity2 = (*pVec2)[ j ];

					// Ignore self-collision
					if( pEntity1 == pEntity2 )
						continue;


					// Local variables help with debugging
					RECT rOverlap = { };
					RECT rEntity1 = pEntity1->GetRect();
					RECT rEntity2 = pEntity2->GetRect();

					// Check for collision between the entities
					if( IntersectRect( &rOverlap, &rEntity1, &rEntity2 ) == TRUE )
					{
						pEntity1->HandleCollision( pEntity2 );
						pEntity2->HandleCollision( pEntity1 );
					}
				}
			}
		}
		else // unType1 == unType2
		{
			EntityVector& vec = m_tEntities[ unType1 ];

			// Optimized loop to ensure objects do not collide with
			// each other twice
			for( int i = (int)(vec.size()-1); i >= 1; i-- )
			{
				for( int j = i-1; j >= 0; j-- )
				{
					// Helper pointers
					IEntity* pEntity1 = vec[ i ];
					IEntity* pEntity2 = vec[ j ];

					// Ignore self-collision
					if( pEntity1 == pEntity2 )
						continue;


					// Local variables help with debugging
					RECT rOverlap = { };
					RECT rEntity1 = pEntity1->GetRect();
					RECT rEntity2 = pEntity2->GetRect();

					// Check for collision between the entities
					if( IntersectRect( &rOverlap, &rEntity1, &rEntity2 ) == TRUE )
					{
						pEntity1->HandleCollision( pEntity2 );
						pEntity2->HandleCollision( pEntity1 );
					}
				}
			}
		}
	}
	// Unlock the iterator
	m_bIterating = false;
}
