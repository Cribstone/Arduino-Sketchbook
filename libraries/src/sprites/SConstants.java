/*
  Part of the Sprites for Processing library 
  	http://sprites4processing.lagers.org.uk
	http://code.google.com/p/sprites4processing/svn/trunk

  Copyright (c) 2009 Peter Lager

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */

package sprites;

public interface SConstants {

	public final static int DRAGGED = 			0x00000203;
	
	public final static int CLICKED = 			0x00000301;
	public final static int PRESSED = 			0x00000302;
	public final static int RELEASED = 			0x00000303;

	
	// ### Error MessageTypes ###
	public final static int RUNTIME_ERROR = 	0xf0000000;
	// Event method handler errors
	public final static int MISSING = 			0x01000001;	// Can't find standard handler
	public final static int NONEXISTANT = 		0x01000002;
	public final static int EXCP_IN_HANDLER =	0x81000003;	// Exception in event handler

}
