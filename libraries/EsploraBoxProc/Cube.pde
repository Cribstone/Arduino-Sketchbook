class Cube{
  
  PVector[] vertices = new PVector[24];
  
  Cube(float w, float h, float d) {
    float[] whd = {-w,-h,d,w,-h,d,w,h,d,-w,h,d,-w,-h,d,-w,
                    -h,-d,-w,h,-d,-w,h,d,w,-h,d,w,-h,-d,w,h,
                    -d,w,h,d,-w,-h,-d,w,-h,-d,w,h,-d,-w,h,-d,
                    -w,-h,d,-w,-h,-d,w,-h,-d,w,-h,d,-w,h,d,-w,
                    h,-d,w,h,-d,w,h,d};
  
    int k = 0;
    for (int i = 0; i < 24; i++){
      vertices[i] = new PVector(whd[k], whd[k+1], whd[k+2]);
      k+=3;
    }
  }
  
  void create(){
    // Draw cube
    for (int i=0; i<6; i++){
      beginShape(QUADS);
      for (int j=0; j<4; j++){
        vertex(vertices[j+4*i].x, vertices[j+4*i].y, vertices[j+4*i].z);
      }
      endShape();
    }
  }
}


