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

/**
 * This class can be used to specify a rectangular domain using
 * world coordinates. <br>
 * 
 * If a Sprite object is assigned a Domain object this is used to 
 * restrict the sprite's movement attempting to leave the domain will
 * cause it to either 'bounce' back or halt. <br>
 * 
 * @author Peter Lager
 *
 */
public class Domain {
	
	public double left,right,top,bottom;

	/**
	 * Create a rectangular domain object using world coordinates.
	 * 
	 * @param left left
	 * @param top top
	 * @param right right
	 * @param bottom bottom
	 */
	public Domain(double left, double top, double right, double bottom) {
		this.left = left;
		this.right = right;
		this.top = top;
		this.bottom = bottom;
	}
}
