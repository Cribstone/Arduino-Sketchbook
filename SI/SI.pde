import sprites.utils.*;
import sprites.maths.*;
import sprites.*;

/* (c) 2008 Dave Gilbert, based on my JavaScript version */
/* si@treblig.org */

int gameWidth=450;
int gameHeight=500;

int topOfPlayArea=70;

// Sizes of sprites
int shipWidth=53;
int shipHeight=32;
int alien1Width=28;
int alien1Height=23;
int mothershipWidth=108;
int mothershipHeight=48;
int mothershipY=topOfPlayArea;
int alienInitialTop=topOfPlayArea+mothershipHeight;
int bombHeight=10;

// I.e. 1 in ....
int mothershipChance=1000;
int mothershipStep=15;

// Spacing between aliens
int alienHspace=alien1Width+10;
int alienVspace=alien1Height+10;
int missileHeight=20;
int missileVstep=10;
int bombStep=10;
int bombDamage=7; // Damage to a base

// Base positons
int[] baseLeftSides = {50,140,230,320};
// Steps/rates of movements
int shipStep=4;
int baseBottom=gameHeight-(10+shipHeight)-missileHeight;
int baseHeight=30;
int baseWidth=70;

int alienRows=4;
int alienCols=7;
int alienHstep=4;
int alienVstep=6;
int totalAliens=alienRows*alienCols;

int bombsMaxInitial=4;
int bombsMax=32;
int bombsPerLevel=4;

// -------------------------------------------------------------
// Current game state
int ships=3;
int level=1;
int score=0;
boolean gameOver=false;
int shipPos=baseLeftSides[2]+shipWidth/2; // Middle of ship
int[] baseBottomArray = new int[gameWidth];
int[] baseTopArray = new int[gameWidth];
boolean[] alienLive = new boolean[totalAliens];
// Note these correspond to the top and left of alien(0,0) - even if that row/column is all dead
int currentAlienArrayLeft;
int currentAlienArrayTop;
int currentAlienHStep; // As Hstep except for sign
int alienLeftNoneEmptyCol;
int alienRightNoneEmptyCol;
int alienLowestNoneEmptyRow;
int mothershipX; // Not running if -1
boolean loseShip;

boolean missileInFlight;
int missileBottom;
int missileX;

PFont mainFont;
PImage shipSprite;
PImage alienSprite;
PImage mothershipSprite;

boolean buttonMoveLeftIsDown=false;
boolean buttonMoveRightIsDown=false;

int bombsInFlight=0;
int bombsMaxThisLevel;
int[] bombsBottomArray=new int[bombsMax];
int[] bombsXArray=new int[bombsMax]; // -ve means not active

void setup () {
  size (gameWidth,gameHeight);
  shipSprite=loadImage("ship.png");
  alienSprite=loadImage("alien1.png");
  mothershipSprite=loadImage("mothership.png");
  bombsMaxThisLevel=bombsMaxInitial;
  missileInFlight=false;
  missileBottom=gameHeight-shipHeight;
  loseShip=false;
  ships=3;

  resetAliens();
  resetBases();
  resetBombs();
  mothershipX=-1;
  
  frameRate(20);
}

void resetBombs()
{
  for (int b=0;b<bombsMax;b++)
  {
    bombsXArray[b]=-1;
  }
  bombsInFlight=0;
}

void resetOneBase(int base)
{
  // This string represents the shape of the top of the base
  // it happens to be our baseWidth.
  // a is lowest, z is highest
  String topProfile=   "ggghijklmnopqrstuvwxyzzzzzzzzzzzzzzzzzzzzzzzzzzzzyxwvutsrqponmlkjihggg";
  // Similarly for the bottom, a is lowest, z highest
  String bottomProfile="aaaaaaaaaaaaaaaaffffffjjotttttttttttttttttttttojjfffffaaaaaaaaaaaaaaaa";
  
  int baseLeft=baseLeftSides[base];
  for(int i=0;i<baseWidth;i++)
  {
    int diff;
    diff=int(bottomProfile.charAt(i))-int('a');
    baseBottomArray[baseLeft+i]=baseBottom-diff;
    diff=int(topProfile.charAt(i))-int('a');
    baseTopArray[baseLeft+i]=(baseBottom-baseHeight)-diff;
  }
}

void resetBases()
{
  for(int i=0;i<gameWidth;i++)
  {
    baseBottomArray[i]=0;
    baseTopArray[i]=0;
  }
  
  resetOneBase(0);
  resetOneBase(1);
  resetOneBase(2);
  resetOneBase(3);
}

void resetAliens()
{
  for(int i=0;i<totalAliens;i++)
  {
    alienLive[i]=true;
  }
  alienLeftNoneEmptyCol=0;
  alienRightNoneEmptyCol=alienCols-1;
  alienLowestNoneEmptyRow=alienRows-1;
  currentAlienHStep=alienHstep;
  currentAlienArrayLeft=0;
  currentAlienArrayTop=alienInitialTop;
}

// --------------------------------------------------------------
// ------------------ Drawing -----------------------------------
void draw () {
  background(0);
  image(shipSprite,shipPos-shipWidth/2,gameHeight-shipHeight);
  if (mothershipX>=0)
  {
    image(mothershipSprite,mothershipX,mothershipY);
  }
  
  drawBases();
  drawAliens();
  drawMissile();
  drawBombs();
}

void drawAliens()
{
  for(int y=0;y<alienRows;y++) {
    for(int x=0;x<alienCols;x++) {
      int alienIndex=x+y*alienCols;
      if (alienLive[alienIndex]) {
        image(alienSprite,currentAlienArrayLeft+x*alienHspace,
                          currentAlienArrayTop+y*alienVspace);
      }
    }
  }
}

void drawBases()
{
  stroke(255,255,0);
  for(int i=0;i<gameWidth;i++)
  {
    if (baseBottomArray[i]>baseTopArray[i])
    {
      line(i,baseBottomArray[i],i,baseTopArray[i]);
    }
  }
}

void drawMissile()
{
  int x;
  if (missileInFlight)
  {
    x=missileX;
  } else {
    x=shipPos;
  }
  stroke(255,0,0);
  line(x,missileBottom,x,missileBottom-missileHeight);
}

void drawBombs()
{
  stroke(0,255,0);
  for(int b=0;b<bombsMax;b++)
  {
    if (bombsXArray[b]>=0)
    {
      line(bombsXArray[b], bombsBottomArray[b] ,
           bombsXArray[b], bombsBottomArray[b]-bombHeight);
    }
  }
}
// --------------------------------------------------------------
// ------------------ Update state ------------------------------

void doUpdate () {
  if (!gameOver)
  {
    updateAliens();
    updateMothership();
    updateBombs();
    updateMissile();
    updateShip();

    if (loseShip)
    {
      loseShip=false;
      ships--;
      if (ships==0) {
        // Game over
        gameOver=true;
      } else {
        resetAliens();
        resetBases();
        resetBombs();
      }
    }
  }
}

// Erode the bases and also check to see if the aliens are at  missile level
void erodeBases () {
  int alienArrayBottom=currentAlienArrayTop+(alienLowestNoneEmptyRow+1)*alienVspace-(alienVspace-alien1Height);
  
  for(int x=0;x<gameWidth;x++)
  {
    if (baseTopArray[x]<=alienArrayBottom)
    {
      baseTopArray[x]=alienArrayBottom+1;
    }
  }
  
  if (alienArrayBottom>=(gameHeight-(missileHeight+shipHeight))) {
    loseShip=true;
  }
}

void updateAliens () {
  int actualArrayLeft, actualArrayRight;

  // this is the left most edge of alien(0,0) even if the whole of row 0 is dead.
  currentAlienArrayLeft+=currentAlienHStep;
  actualArrayLeft=currentAlienArrayLeft+alienLeftNoneEmptyCol*alienHspace;  
  actualArrayRight=currentAlienArrayLeft+(alienRightNoneEmptyCol+1)*alienHspace-(alienHspace-alien1Width);
  boolean moveDown;
  
  moveDown=((currentAlienHStep>0) && (actualArrayRight>=gameWidth)) ||
           ((currentAlienHStep<0) && (actualArrayLeft<=0));

  if (moveDown) {
    currentAlienHStep=-currentAlienHStep;
    currentAlienArrayTop+=alienVstep;    
    erodeBases();
  }
}

// Pick an alien to drop a bomb from
// WARNING: Don't call this if there aren't any live aliens
int pickAlien () {
  int a;
  
  a=int(random(0,totalAliens-0.001));
  while (alienLive[a]==false)
  {
    a=int(random(0,totalAliens-0.001));
  };
  
  return a;
}

void updateMothership()
{
  if (mothershipX>=0)
  {
    // already running
    mothershipX+=mothershipStep;
    if (mothershipX>gameWidth) {
      mothershipX=-1;
    }
  } else {
    // not running - should we start it?
    if (random(mothershipChance)<1) {
      mothershipX=0;
    }
  }
}

void updateBombs () {
  // First update ones in flight
  for(int b=0;b<bombsMax;b++) {
    boolean hitSomething=false;
    int x=bombsXArray[b];
    if (x>=0)
    {
      // The bomb is live
      bombsBottomArray[b]+=bombStep;

      // See if it hit a base
      if ((baseTopArray[x]<baseBottomArray[x]) && (baseTopArray[x]<=bombsBottomArray[b]))
      {
        hitSomething=true;
        baseTopArray[x]=baseTopArray[x]+bombDamage;
      }
      
      if ((!hitSomething) && (bombsBottomArray[b]>=(gameHeight-shipHeight)) &&
          (x>(shipPos-shipWidth/2)) && (x<(shipPos+shipWidth/2))) {
            // We've hit the ship
            loseShip=true;
            hitSomething=true;
          }
      if (hitSomething || (bombsBottomArray[b]>=gameHeight))
      {
        bombsXArray[b]=-1;
        bombsInFlight-=1;
      }
    }
  }
  
  // Now consider adding a bomb
  if (bombsInFlight<bombsMaxThisLevel) {
    int b;
    // Find a free slot in the bomb array
    for(b=0;b<bombsMax;b++)
    {
      if (bombsXArray[b]==-1)
      {
        // OK this is free
        int a=pickAlien();
        int col=a % alienCols;
        int row=a/alienCols;
        
        // The random term here accounts for quantisaiton effects due to the step size of the
        // aliens that means you can get regular gaps where bombs don't fall
        bombsXArray[b]=currentAlienArrayLeft+col*alienHspace+(alien1Width/2)+int(random(-2,2));
        bombsBottomArray[b]=currentAlienArrayTop+row*alienVspace+alien1Height+bombHeight;
        bombsInFlight++;
        break;
      }
    }
  }
}

void updateShip () {
  if (buttonMoveLeftIsDown) {
    shipPos-=shipStep;
  }
  if (buttonMoveRightIsDown) {
    shipPos+=shipStep;
  }
  shipPos=constrain(shipPos,shipWidth/2, gameWidth-shipWidth/2);
}

// Update the left/right/lowest alien cols that are active and if necessary move us onto another level
void updateAlienCounts () {
  // Initialise the counts so that they are the in exactly the opposite direction to the edge
  // they represent so will be pulled back
  int newLeftMost=alienCols-1;
  int newRightMost=0;
  int newLowest=0;
  boolean foundAny=false;
  
  for(int a=0;a<totalAliens;a++)
  {
    if (alienLive[a])
    {
      int row=a/alienCols;
      int col=a % alienCols;
    
      foundAny=true;
      if (col<newLeftMost) {
        newLeftMost=col;
      };
      
      if (col>newRightMost) {
        newRightMost=col;
      };
      
      if (row>newLowest) {
        newLowest=row;
      };        
    }
  }
  
  alienLeftNoneEmptyCol=newLeftMost;
  alienRightNoneEmptyCol=newRightMost;
  alienLowestNoneEmptyRow=newLowest;
  
  if (!foundAny)
  {
    resetAliens();
    bombsMaxThisLevel+=2;
    if (bombsMaxThisLevel>bombsMax) {
      bombsMaxThisLevel=bombsMax;
    }
    level++;
  }
  
}

void updateMissile () {
  
  if (missileInFlight) {
    boolean endFlight=false;
    int missileTop;
    
    missileBottom-=missileVstep;
    missileTop=missileBottom-missileHeight;
    // Hit bases?    
    if ((baseBottomArray[missileX]>baseTopArray[missileX]) && (missileTop<baseBottomArray[missileX]))
    {
      baseBottomArray[missileX]=missileTop;
      endFlight=true;
    }

    // Hit an alien? We check the top of the missile (well that's where the warhead is!)
    if ((!endFlight) && 
       (missileX>=currentAlienArrayLeft) &&
       (missileX<(currentAlienArrayLeft+alienCols*alienHspace)) &&
       (missileTop>currentAlienArrayTop) &&
       (missileTop<(currentAlienArrayTop+alienRows*alienVspace)))
    {
      // Well somewhere horizontally within the range of the aliens
      int alienCol=int((missileX-currentAlienArrayLeft)/alienHspace);
      int alienRow=int((missileTop-currentAlienArrayTop)/alienVspace);

      if ((alienCol>=0) && (alienCol<alienCols) &&
          (alienRow>=0) && (alienRow<alienRows) &&
          (alienLive[alienCol+alienRow*alienCols]))
      {
        // Currently live
        alienLive[alienCol+alienRow*alienCols]=false;
        endFlight=true;
        score+=10;
        // We have to update the count of aliens to find out rows that have gone
        // This will also move us onto the next level
        updateAlienCounts();
      }
    }

    // Have we got the mothership?
    if (!endFlight && (missileTop<(mothershipY+mothershipHeight)) &&
        (missileTop>mothershipY) && (missileX>=mothershipX) &&
        (missileX<(mothershipX+mothershipWidth))) {
      // Disable mothership
      mothershipX=-1;
      score+=100;
      
      endFlight=true;
    }
    
    if (missileTop<topOfPlayArea) {
      endFlight=true;
    }
    
    if (endFlight) {
      missileInFlight=false;
      missileBottom=gameHeight-shipHeight;
    }
  }
}

// --------------------------------------------------------------
// ------------------ Keyboard interaction ----------------------
void keyPressed() {
  switch (key) {
    case 'z':
      buttonMoveLeftIsDown=true;
      break;
      
    case 'x':
      buttonMoveRightIsDown=true;
      break;
      
    case ' ':
      if (!missileInFlight) {
        missileInFlight=true;
        missileX=shipPos;
      };
  }
}

void keyReleased() {
  switch (key) {
    case 'z':
      buttonMoveLeftIsDown=false;
      break;
      
    case 'x':
      buttonMoveRightIsDown=false;
      break;
  }
}
