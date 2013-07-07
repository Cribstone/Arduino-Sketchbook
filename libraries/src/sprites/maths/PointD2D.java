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

package sprites.maths;

/**
 * This class is provided to assist when converting world <> screen
 * coordinates.
 * 
 * It means we don't have to use the Point2D.Double class - bit messy
 * for newbies to Processing
 * 
 * @author Peter Lager
 *
 */
public class PointD2D {

	public double x, y;
	
	/**
	 * Create a PointF2D object to store x/y coordinates
	 * and initialise to 0.0/0.0
	 */
	public PointD2D() {
		x = y = 0.0f;
	}

	
	/**
	 * Create a PointF2D object to store x/y coordinates
	 * 
	 * @param x
	 * @param y
	 */
	public PointD2D(float x, float y) {
		super();
		this.x = x;
		this.y = y;
	}

	public String toString(){
		return "["+x+", "+y+"]";
	}
}
