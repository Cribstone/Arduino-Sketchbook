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

import java.awt.Point;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.TreeSet;

import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PImage;
import sprites.maths.PointD2D;
import sprites.maths.Vector2D;

/**
 * This class is used to manage the sprites, it keeps track
 * of all bitmap image files loaded to prevent multiple copies
 * of the same image being loaded.
 * 
 * The main methods you are likely to use are update and draw 
 * ALL sprites.
 * 
 * @author Peter Lager
 *
 */
public class S4P implements PConstants {

	// Image loading uses these static attributes and methods
	private static HashMap<Integer, ImageInfo> imageStore = new HashMap<Integer, ImageInfo>() ;

	private static LinkedList<Sprite> sprites = new LinkedList<Sprite>();
	private static PApplet app;

	public static boolean messages = true;

	/**
	 * If you want to see the collision area for the sprites then set to true.<br>
	 * This should only be done when testing collision detection because it 
	 * uses a lot of memory creating images for pixel-pixel collision areas
	 * which is not released even if this is set back to false.
	 */
	public static boolean collisionAreasVisible = false;

	/**
	 * Color used to show collision detect area. Recommend that you use a partially
	 * transparent colour. 
	 */
	public static int colColor = 0x80ffff00;

	/**
	 * Color used to highlight selected sprite Recommend that you use a partially
	 * transparent colour.
	 */
	public static int selColor = 0x8000ff00;

	/**
	 * Defines that area of the world that is being displayed
	 *  World coordinates
	 */
	public static Domain screenDomain;

	/**
	 * World coordinates corresponding to top-left of display
	 */
	protected static double worldX = 0.0f;

	/**
	 * World coordinates corresponding to top-left of display
	 */
	protected static double worldY = 0.0f;

	/**
	 * Ratio of screen to real world coordinates
	 */
	protected static double worldScale = 1.0f;

	/**
	 * Change the world display scale <br>
	 * A value of 1.0 gives 1:1 size correspondence with the screen
	 * 
	 * @param scale
	 */
	public static void resizeWorld(double scale){
		double newX, newY;
		double w = app.width;
		double h = app.height;
		// Calculate new origin so as to centre the image
		newX = worldX + w/(2.0f*worldScale) - w/(2.0f*scale);
		newY = worldY + h/(2.0f*worldScale) - h/(2.0f*scale);
		worldX = newX;
		worldY = newY;
		worldScale = scale;
		calcScreenDomain();
	}

	/**
	 * Calculate the top-left and bottom-right world coordinates
	 * that is currently viewable on the screen. 
	 */
	protected static void calcScreenDomain(){
		screenDomain.left = worldX;
		screenDomain.right = worldX + app.width/worldScale;
		screenDomain.top = worldY;
		screenDomain.bottom = worldY + app.height / worldScale;
	}

	/**
	 * Reset the world so that the world origin is at the top-left corner
	 * of the screen and their is 1:1 correspondence between world and screen
	 */
	public static void resetWorld(){
		worldScale = 1.0f;
		worldX = 0;
		worldY = 0;
		calcScreenDomain();
	}

	/**
	 * Calculate the world position for the given screen position
	 * 
	 * @param px
	 * @param py
	 * @return the world position
	 */
	public static PointD2D pixel2world(double px, double py){
		PointD2D w = new PointD2D();
		w.x = worldX + px / worldScale;
		w.y = worldY + py / worldScale;
		return w;
	}

	/**
	 * Calculate the screen position for a given world position
	 * 
	 * @param wx world X position
	 * @param wy world Y position
	 * @return the screen position
	 */
	public static Point world2pixel(double wx, double wy){
		Point p = new Point();
		p.x = (int)((wx - worldX) * worldScale);
		p.y = (int)((wy - worldY) * worldScale);
		return p;
	}

	/**
	 * Set the world's x/y position that corresponds to the top-left 
	 * position on the screen. 
	 * @param x
	 * @param y
	 */
	public static void moveWorldTo(double x, double y){
		worldX = x;
		worldY = y;
		calcScreenDomain();
	}

	/**
	 * Moves the world's x/y position that corresponds to the top-left 
	 * position on the screen. 
	 * @param x
	 * @param y
	 */
	public static void moveWorldBy(double x, double y){
		worldX += x;
		worldY += y;
		calcScreenDomain();
	}

	/**
	 * INTERNAL USE ONLY <br>
	 * Avoid nulls being included to simplify sorter
	 * 
	 * @param sprite
	 */
	public static void registerSprite(Sprite sprite){
		if(sprite != null){
			sprites.add(sprite);
			sortZorder();
		}
	}

	/**
	 * INTERNAL USE ONLY <br>
	 * 
	 * @param sprite
	 */
	public static void deregisterSprite(Sprite sprite){
		if(sprite != null){
			sprites.remove(sprite);
		}
	}

	/**
	 * Sorts the sprites so they are drawn in z order <br>
	 * Lower Z order values are drawn first. sort
	 */
	public static void sortZorder(){
		Collections.sort(sprites);
	}

	/**
	 * Update the position, image animation etc for every sprite based on the
	 * time since the last time this method is called. This method should follow
	 * after a call to updateTime()
	 */
	public static void updateSprites(double deltaTime){
		Iterator<Sprite> iter = sprites.iterator();
		TreeSet<Sprite> toRemove = new TreeSet<Sprite>();
		Sprite s = null;
		while(iter.hasNext()){
			s = iter.next();
			if(s.isDead())
				toRemove.add(s); // track sprites to remove
			else
				s.update(deltaTime);
		}
		// Remove dead sprites
		if(toRemove.size() > 0){
			iter = toRemove.iterator();
			while(iter.hasNext()){
				sprites.remove(iter.next());
			}
		}
	}

	/**
	 * Called from with the draw() method of your sketch. Causes all
	 * visible non-dead sprites to be displayed.
	 */
	public static void drawSprites(){
		app.pushMatrix();
		app.scale((float) worldScale);
		app.translate((float)-worldX, (float)-worldY);
		Iterator<Sprite> iter = sprites.iterator();
		app.imageMode(CENTER);
		while(iter.hasNext()){
			iter.next().draw();
		}
		app.popMatrix();
	}

	/**
	 * INTERNAL USE ONLY  <br>
	 * 
	 * @param theApplet
	 * @param imageFname
	 * @param ncols
	 * @param nrows
	 * @return the ImageInfo object for this image file
	 */
	public static ImageInfo getImageInfo(PApplet theApplet, String imageFname, int ncols, int nrows){
		app = theApplet;
		if(screenDomain == null)
			screenDomain = new Domain(0,0,app.width,app.height);

		Integer kcode = new Integer(imageFname.toLowerCase().hashCode());
		ImageInfo imgInfo = null;

		if(imageStore.containsKey(kcode)) {
			imgInfo = imageStore.get(kcode);
		}
		else {
			PImage image = app.loadImage(imageFname);
			imgInfo = new ImageInfo(image, ncols, nrows);
			imageStore.put(kcode, imgInfo);
		}
		return imgInfo;
	}

	/**
	 * INTERNAL USE ONLY  <br>
	 * 
	 * @param theApplet
	 * @param imageFname
	 * @param alphaMaskFname
	 * @param ncols
	 * @param nrows
	 * @return the ImageInfo object for this image file
	 */
	public static ImageInfo getImageInfo(PApplet theApplet, String imageFname, String alphaMaskFname, int ncols, int nrows){
		app = theApplet;
		if(screenDomain == null)
			screenDomain = new Domain(0,0,app.width,app.height);

		Integer kcode = new Integer((imageFname.toLowerCase() + alphaMaskFname.toLowerCase()).hashCode());
		ImageInfo imgInfo = null;

		if(imageStore.containsKey(kcode)) {
			imgInfo = imageStore.get(kcode);
		}
		else {
			PImage image = app.loadImage(imageFname);
			PImage amask = app.loadImage(alphaMaskFname);
			imgInfo = new ImageInfo(image, amask, ncols, nrows);
			imageStore.put(kcode, imgInfo);
		}
		return imgInfo;
	}

	/**
	 * INTERNAL USE ONLY  <br>
	 * Creates a set of frames based on an image already loaded.
	 * 
	 * @param info image details
	 * @return an array of images from tiled image
	 */
	public static PImage[] getFrames(ImageInfo info){
		int nframes = info.nbrCols * info.nbrRows;
		PImage[] frames = new PImage[nframes];
		int dw = info.image.width / info.nbrCols;
		int dh = info.image.height / info.nbrRows;
		int fn = 0;
		for(int y = 0; y < info.nbrRows; y++){
			for(int x = 0; x < info.nbrCols; x++){
				frames[fn] = new PImage(dw,dh,ARGB);
				frames[fn].copy(info.image, x*dw, y*dh, dw, dh, 0, 0, dw, dh);
				fn++;
			}
		}
		// If an alpha mask exists 
		if(info.alphaMask != null){
			PImage[] aframes = makeAlphaMaskFrames(info, dw, dh);
			int afn = 0;
			for(int i = 0; i < frames.length; i++){
				frames[i].loadPixels();
				aframes[afn].loadPixels();
				for(int p = 0; p < frames[i].pixels.length; p++){
					frames[i].pixels[p] &= 0x00ffffff;
					frames[i].pixels[p] |= (aframes[afn].pixels[p] << 24);
				}
				frames[i].updatePixels();
				afn = (afn + 1)%aframes.length;
			}
		}
		return frames;		
	}

	/**
	 * INTERNAL USE ONLY  <br>
	 * Modifies frames by applying alpha mask
	 * 
	 * @param info
	 * @param dw
	 * @param dh
	 * @return
	 */
	private static PImage[] makeAlphaMaskFrames(ImageInfo info, int dw, int dh){
		int a,r,g,b;

		info.alphaMask.loadPixels();
		int[] org = info.alphaMask.pixels;
		int[] p = new int[org.length];
		for(int i = 0; i < p.length; i++){
			b = org[i] & 0xff;
			g = (org[i] >> 8) & 0xff;
			r = (org[i] >> 16) & 0xff;
			a = (r+g+b)/3;
			p[i] = a; // << 24;
		}
		// Calculate number of alpha masks based on the main image height and width
		int cols = info.alphaMask.width / dw;
		int rows = info.alphaMask.height / dh;
		int nframes = cols * rows;
		PImage[] aframes = new PImage[nframes];
		int fn = 0;
		for(int y = 0; y < rows; y++){
			for(int x = 0; x < cols; x++){
				aframes[fn] = new PImage(dw,dh,ARGB);
				aframes[fn].copy(info.alphaMask, x*dw, y*dh, dw, dh, 0, 0, dw, dh);
				fn++;
			}
		}
		return aframes;		
	}

	/**
	 * Collision detection <br>
	 * Calculates new velocities for the 2 game objects based on elastic collision.
	 * 
	 * 
	 * It can also apply variable elasticity and friction to the collision. 
	 * 
	 * @param mover0 a moving sprite
	 * @param mover1 a moving sprite
	 * @return
	 */
	public static boolean rebound(Sprite mover0, Sprite mover1){
		// Calculate mass ratios
		double mr1 = mover1.mass / (mover0.mass + mover1.mass);
		double mr2 = 1 - mr1;

		// Calculate normal to plane of collision
		Vector2D collNorn = Vector2D.sub(mover0.pos, mover1.pos);
//		Vector2D collNorn = new Vector2D(0, mover0.pos.y - mover1.pos.y - mover1.halfHeight);
//		Vector2D collNorn = new Vector2D(0, 1);

		// Calculate relative velocities
		Vector2D relVel = Vector2D.sub(mover0.vel, mover1.vel);

		// Calculate impact velocity (if nv < then they are moving apart)
		double nv = Vector2D.dot(collNorn, relVel);
		// Calculate collision plane normal length squared
		double n2 = Vector2D.dot(collNorn, collNorn);

		// Either moving apart or the size of the collision plane normal
		// is too small to have any effect so don't reflect objects but still
		// return true because it is part of the object reflection code.
		if(nv > 0.0f || n2 < 0.00001f)
			return false;

		// Relative velocity along collision plane normal
		Vector2D vN = Vector2D.mult(collNorn, nv / n2);
		// Tangential velocity along the collision plane
		Vector2D vT = Vector2D.sub(relVel, vN);

		// Calculate changes in velocity for each object
		Vector2D vN1 = Vector2D.mult(vN, (1 + mover0.elasticity) * mr1);
		Vector2D vN2 = Vector2D.mult(vN, (1 + mover1.elasticity) * mr2);
		Vector2D vT1 = Vector2D.mult(vT, mover0.friction);
		Vector2D vT2 = Vector2D.mult(vT, mover1.friction);

		// Apply changes to the game objects
		mover0.vel.sub(vN1);
		mover0.vel.sub(vT1);		
		mover1.vel.add(vN2);
		mover1.vel.add(vT2);

		return true;
	}

	public static boolean rebound(Sprite mover0, Vector2D norm){
		// Calculate mass ratios
		double mr1 = 1;
		double mr2 = 0;

		// Calculate normal to plane of collision
//		Vector2D collNorn = Vector2D.sub(mover0.pos, mover1.pos);
//		Vector2D collNorn = new Vector2D(0, mover0.pos.y - mover1.pos.y - mover1.halfHeight);
//		Vector2D collNorn = new Vector2D(0, 1);

		// Calculate relative velocities
		Vector2D relVel = mover0.vel;

		// Calculate impact velocity (if nv < then they are moving apart)
		double nv = Vector2D.dot(norm, relVel);
		// Calculate collision plane normal length squared
		double n2 = Vector2D.dot(norm, norm);

		// Either moving apart or the size of the collision plane normal
		// is too small to have any effect so don't reflect objects but still
		// return true because it is part of the object reflection code.
		if(nv > 0.0f || n2 < 0.00001f)
			return false;

		// Relative velocity along collision plane normal
		Vector2D vN = Vector2D.mult(norm, nv / n2);
		// Tangential velocity along the collision plane
		Vector2D vT = Vector2D.sub(relVel, vN);

		// Calculate changes in velocity for each object
		Vector2D vN1 = Vector2D.mult(vN, (1 + mover0.elasticity) * mr1);
//		Vector2D vN2 = Vector2D.mult(vN, (1 + mover1.elasticity) * mr2);
		Vector2D vT1 = Vector2D.mult(vT, mover0.friction);
//		Vector2D vT2 = Vector2D.mult(vT, mover1.friction);

		// Apply changes to the game objects
		mover0.vel.sub(vN1);
		mover0.vel.sub(vT1);		
//		mover1.vel.add(vN2);
//		mover1.vel.add(vT2);

		return true;
	}

	/**
	 * S4P has a range of support messages eg <br>if you create a sprite component 
	 * without an event handler. <br>
	 * 
	 * This method allows the user to enable (default) or disable this option. If
	 * disable.
	 * 
	 * @param enable
	 */
	public static void messagesEnabled(boolean enable){
		messages = enable;
	}


}
