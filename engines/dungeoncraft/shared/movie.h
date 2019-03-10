/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef DUNGEONCRAFT_SHARED_MOVIE
#define DUNGEONCRAFT_SHARED_MOVIE

namespace DungeonCraft {

/*** Include files **************************************************/

/*** Defines ********************************************************/

/*** Macros *********************************************************/

/*** Data types definitions and typedefs ****************************/

/*** External class declarations ************************************/
//class CDXSurface;
//class CDXScreen;

/*** External interface declarations ********************************/
//struct IMultiMediaStream; 
//struct IMediaStream;
//struct IDirectDrawMediaStream;
//struct IDirectDrawStreamSample;
//struct IDirectDrawSurface;

/********************************************************************/
/*** Class definitions **********************************************/
/********************************************************************/
class Movie  
{
/*** Public Member Functions ****************************************/
public:
	Movie(CDXScreen* pScreen);
	virtual ~Movie();

	bool Create(const char* szFilename, CDXSurface *pSurface);
	bool Play();
	bool Stop();
	bool Destroy();
	bool Update(RECT *pRect=NULL);
  RECT GetSrcRect() const { return m_srcRect; }

/*** Protected Member Functions *************************************/
protected:

/*** Protected Member Variables *************************************/
protected:

/*** Private Member Functions ***************************************/
private:
	bool OpenMMStream(const char* szFilename);

/*** Private Member Variables ***************************************/
private:
	CDXScreen*						    m_pScreen;
	CDXSurface*						    m_pSurface;
	IMultiMediaStream*			  m_pMMStream;
	IMediaStream*					    m_pPrimaryVidStream;
	IDirectDrawMediaStream*		m_pDDStream;
  IDirectDrawStreamSample*	m_pDDStreamSample;
  IDirectDrawSurface*			  m_pDDSurface;
	RECT								      m_rectClipping;
  RECT                      m_srcRect;
};

} // End of namespace DungeonCraft

#endif
