/*
 TMC26XMotorTest.pde - - TMC26X Stepper Tester for Processing
 
 Copyright (c) 2011, Interactive Matter, Marcus Nowotny
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

//our graph dimensions
int plotBottom, plotTop;
int plotLeft, plotRight;


//we already know the minima and maxima of certain dates
int stallGuardMin =  0;
int stallGuardMax =1024;

int positionMin = 0;
int positionMax = 1024;

int dataPointsWidth = 3;
int dataLineWidth = 2;
int highLightWidth = 7;

int numberOfDataPoints=1000;

int legendTitleSize = 10;
int legendTextSize = 10;
int currentLabelInterval = 500;
int currentMinorTickInterval = 25;
DecimalFormat currentLabelFormat = new DecimalFormat("#0.0A");

int stallGuardLabelInterval = 100;
int stallGuardMinorTickInterval = 10;
int positionLabelInterval = 64;
int positionMinorTickInterval = 8;

int coolStepActiveStroke = 2;
int coolStepInactiveStroke = 1;
int stallGuardHighLightDistance = 1;
int positionHighLightDistance = 3;
int currentHighLightDistance = 3;

DataTable stallGuardTable = new DataTable(numberOfDataPoints);
DataTable positionTable = new DataTable(numberOfDataPoints);
DataTable currentTable =  new DataTable(numberOfDataPoints);

int logoLeft = 10;
int logoTop = 50;
int logoWidth = 75;
int logoHeight = 75;

void setupData() {
  plotBottom = height-50;
  plotTop = 300;

  plotLeft = 150;
  plotRight= width-50;
  //recalculate the image dimension for the Logo
  logoHeight = (int)((float)logoWidth/(float)TMCLogo.width*(float)TMCLogo.height);
  logoTop = height - 10 - logoHeight;
}

void drawData() {

  if (motor_connected && activeTab!=null && runTab.equals(activeTab)) {
    fill(diagramBackgroundColor);
    noStroke();
    rect(plotLeft, plotTop, plotRight, plotBottom);
    noFill();
    rectMode(CORNERS);
    noStroke();
    //rect(plotLeft, plotBottom, plotRight, plotTop);

    strokeWeight(dataLineWidth);
    stroke(positionColor);
    drawDataLine(positionTable, positionMin, positionMax);
    drawDataHighLight(positionTable, positionMin, positionMax, positionHighLightDistance, labelColor, "Microstep Position", false);

    strokeWeight(dataLineWidth);
    stroke(coolStepColor);
    drawCurrentLine(currentTable);
    drawCurrentHighLight(positionTable, currentHighLightDistance, labelColor, "Current Ratio", false);

    strokeWeight(dataPointsWidth);
    stroke(stallGuardColor);
    drawDataPoints(stallGuardTable, stallGuardMin, stallGuardMax);
    drawDataHighLight(stallGuardTable, stallGuardMin, stallGuardMax, stallGuardHighLightDistance, labelColor, "Stall Guard", true);
    
    if (coolStepActive) {
      strokeWeight(coolStepActiveStroke); 
    } else {
      strokeWeight(coolStepInactiveStroke);
    }
    stroke(coolStepColor);
    float coolStepMinHeight = map(coolStepMin, 0, stallGuardMax, plotBottom, plotTop);
    line(plotLeft,coolStepMinHeight, plotRight, coolStepMinHeight);
    float coolStepMaxHeight = map(coolStepMin+coolStepMax+1, 0, stallGuardMax, plotBottom, plotTop);
    line(plotLeft,coolStepMaxHeight, plotRight, coolStepMaxHeight);

    textSize(legendTitleSize);
    textAlign(RIGHT);
    fill(coolStepColor);
    text("Motor Current", plotLeft - 50, plotTop - 10);
    textSize(legendTextSize);
    textAlign(RIGHT);
    strokeWeight(1);
    stroke(coolStepColor);
    int currentScaleMax = (int)(maxCurrent*1000.0);
    for (int i=0; i<=currentScaleMax; i++) {
      float y = map(i, 0, currentScaleMax, plotBottom, plotTop);
      if (i % currentLabelInterval == 0) {
        if (i==0) {
          textAlign(RIGHT, BOTTOM);
        } 
        else if (i==currentScaleMax) {
          textAlign(RIGHT, TOP);
        } 
        else {
          textAlign(RIGHT, CENTER);
        }        
        text(currentLabelFormat.format((float)i/1000.0), plotLeft-58, y);
        line (plotLeft-55, y, plotLeft-50, y);
      } 
      else if (i % currentMinorTickInterval == 0) {
        line (plotLeft-53, y, plotLeft-50, y);
      }
      
    }

    textSize(legendTitleSize);
    textAlign(LEFT);
    fill(stallGuardColor);
    text("Stall Guard Reading", plotLeft - 30, plotTop - 10);
    textSize(legendTextSize);
    textAlign(RIGHT);
    strokeWeight(1);
    stroke(stallGuardColor);
    for (int i=stallGuardMin; i<=stallGuardMax; i++) {
      float y = map(i, stallGuardMin, stallGuardMax, plotBottom, plotTop);
      if (i % stallGuardLabelInterval == 0) {
        if (i==stallGuardMin) {
          textAlign(RIGHT, BOTTOM);
        } 
        else if (i==stallGuardMax) {
          textAlign(RIGHT, TOP);
        } 
        else {
          textAlign(RIGHT, CENTER);
        }        
        text(i, plotLeft-8, y);
        line (plotLeft-5, y, plotLeft, y);
      } 
      else if (i % stallGuardMinorTickInterval == 0) {
        line (plotLeft-3, y, plotLeft, y);
      }
    }

    textSize(legendTitleSize);
    fill(positionColor);
    textAlign(RIGHT);
    text("Position", plotRight + 30, plotTop - 10);
    textSize(legendTextSize);
    textAlign(LEFT);
    strokeWeight(1);
    stroke(positionColor);
    for (int i=positionMin; i<=positionMax; i++) {
      float y = map(i, positionMin, positionMax, plotBottom, plotTop);
      if (i % positionLabelInterval == 0) {
        if (i==positionMin) {
          textAlign(LEFT, BOTTOM);
        } 
        else if (i==stallGuardMax) {
          textAlign(LEFT, TOP);
        } 
        else {
          textAlign(LEFT, CENTER);
        }        
        text(i, plotRight+8, y);
        line (plotRight, y, plotRight+5, y);
      } 
      else if (i % positionMinorTickInterval == 0) {
        line (plotRight, y, plotRight+3, y);
      }
    }
    //draw the channel status
    textSize(legendTextSize);
    textAlign(CENTER);
    strokeWeight(1);
    int statusY = height - 20;
    int channelAX = width/4;
    int temperatureX = width/2;
    int channelBX = width/4*3;
    if (channelAStatus==null) {
      fill(goodStatusColor);
      text("Channel A: OK", channelAX, statusY);
    } 
    else {
      fill(badStatusColor);
      text("Channel A: "+channelAStatus, channelAX, statusY);
    }    
    if (channelBStatus==null) {
      fill(goodStatusColor);
      text("Channel B: OK", channelBX, statusY);
    } 
    else {
      fill(badStatusColor);
      text("Channel B: "+channelBStatus, channelBX, statusY);
    }    
    if (temperatureStatus==null) {
      fill(goodStatusColor);
      text("Temperature: OK", temperatureX, statusY);
    } 
    else {
      fill(badStatusColor);
      text("Temperature: "+temperatureStatus, temperatureX, statusY);
    }
  }
}
void drawDataPoints(DataTable table, int minValue, int maxValue) {
  int dataCount = table.getSize();
  for (int i=0; i<dataCount; i++) {
    int value = table.getEntry(i);
    float x = map(i, 0, numberOfDataPoints-1, plotLeft+dataPointsWidth, plotRight-dataPointsWidth);
    float y = map(value, minValue, maxValue, plotBottom-dataPointsWidth, plotTop+dataPointsWidth);
    point(x, y);
  }
}

void drawDataLine(DataTable table, int minValue, int maxValue) {
  beginShape();
  int dataCount = table.getSize();
  for (int i=0; i<dataCount; i++) {
    int value = table.getEntry(i);
    float x = map(i, 0, numberOfDataPoints-1, plotLeft+dataPointsWidth, plotRight-dataPointsWidth);
    float y = map(value, minValue, maxValue, plotBottom-dataPointsWidth, plotTop+dataPointsWidth);
    vertex(x, y);
  }
  endShape();
}

void drawDataHighLight(DataTable table, int minValue, int maxValue, int distance, color textColor, String name, boolean top) {
  int dataCount = table.getSize();
  for (int i=0; i<dataCount; i++) {
    int value = table.getEntry(i);
    float x = map(i, 0, numberOfDataPoints-1, plotLeft+dataPointsWidth, plotRight-dataPointsWidth);
    float y = map(value, minValue, maxValue, plotBottom-dataPointsWidth, plotTop+dataPointsWidth);
    if (dist(mouseX, mouseY, x, y) < distance) {
      strokeWeight(highLightWidth);
      point(x, y);
      fill(textColor);
      textSize(10);
      textAlign(CENTER);
      if (top) {
        text(name+": "+value, x, y-8);
      } 
      else {
        text(name+": "+value, x, y+8);
      }
    }
  }
}

void drawCurrentLine(DataTable table) {
  noFill();
  beginShape();
  int dataCount = table.getSize();
  for (int i=0; i<dataCount; i++) {
    float value = (table.getEntry(i)+1)/1000.0;
    float x = map(i, 0, numberOfDataPoints-1, plotLeft+dataPointsWidth, plotRight-dataPointsWidth);
    float y = map(value, 0.0, maxCurrent, (float)plotBottom-dataPointsWidth, (float)plotTop+dataPointsWidth);
    vertex(x, y);
  }
  endShape();
}

void drawCurrentHighLight(DataTable table, int distance, color textColor, String name, boolean top) {
  int dataCount = table.getSize();
  for (int i=0; i<dataCount; i++) {
    float value = (table.getEntry(i)+1)/1000.0;
    float x = map(i, 0, numberOfDataPoints-1, plotLeft+dataPointsWidth, plotRight-dataPointsWidth);
    float y = map(value, 0.0, maxCurrent, plotBottom-dataPointsWidth, plotTop+dataPointsWidth);
    if (dist(mouseX, mouseY, x, y) < distance) {
      strokeWeight(highLightWidth);
      point(x, y);
      fill(textColor);
      textSize(10);
      textAlign(CENTER);
      if (top) {
        text(name+": "+value, x, y-8);
      } 
      else {
        text(name+": "+value, x, y+8);
      }
    }
  }
}

void addStallGuardReading(int value) {
  stallGuardTable.addData(value);
}

void addPositionReading(int value) {
  positionTable.addData(value);
}

void addCurrentReading(int value) {
  currentTable.addData(value);
}

