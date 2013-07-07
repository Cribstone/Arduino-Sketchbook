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

import processing.core.PImage;

/**
 * CLASS FOR INTERNAL USE ONLY <br>
 * 
 * used to remember the details about the image to prevent multiple copies
 * of same image being loaded and stored.
 * 
 * @author Peter Lager
 *
 */
public class ImageInfo {
	public int nbrRows;
	public int nbrCols;
	public PImage image;
	public PImage alphaMask;
	
	/**
	 * INTERNAL USE ONLY
	 * @param img
	 * @param c
	 * @param r
	 */
	public ImageInfo(PImage img, int c, int r){
		image = img;
		alphaMask = null;
		nbrRows = r;
		nbrCols = c;
	}

	/**
	 * INTERNAL USE ONLY
	 * @param img
	 * @param mask
	 * @param c
	 * @param r
	 */
	public ImageInfo(PImage img, PImage mask, int c, int r){
		image = img;
		alphaMask = mask;
		nbrRows = r;
		nbrCols = c;
	}

}
