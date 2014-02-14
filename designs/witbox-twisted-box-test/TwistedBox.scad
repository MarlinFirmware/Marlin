/*
 *  Parametric TwistedBox
 *  Copyright (C) 2009  Philipp Tiefenbacher <wizards23@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
*
 *  http://www.gnu.org/licenses/gpl.html
 */

// generates the bottom half of the box
// TwistedBox(0);
// generates the top half of the box
// TwistedBox(1);


// create both parts next to each other
TwistedBox(1);
//translate([60,0,0]) TwistedBox(1);


// if topNotBottom == 0 it creates the bottom part of the box (with the grip)
// if topNotBottom == 1 it creates the top part of the box
module TwistedBox(topNotBottom)
{
	// the dxf file is expected to have at least 4 layers: 
	// outside_outside, outside_inside, inside_outside, inside_inside
	// which are used for the different walls of the box
	dxfFile = "twistedboxshape7_narrow.dxf";
	
	// the total height of the closed box
	totalH=20;
	// height of grip of the box
	gripH=2;
	// height of the bottom  floor of the box
	bottomFloorH=2;
	// height of the top  floor of the box
	topFloorH=bottomFloorH;
	// in degrees 
	twistPerMM=3.8;
	// -1 for the normal right screwing to open direction
	twistDirection = -1;

	// parameters for level of detail
	// how many linesegments a complete circle is polygonized to
	$fn = 25; 
	// lower this if you have a high twist; should give a integer number when 
	// multiplied with totalH, gripH, topFloorH and bottomFloorH
	slicesPerMM = 1.0001;  

	module basicShape(dxfLayer, zStart, h) {
		translate([0,0,h/2+zStart]) 
			rotate([0, 0, -zStart*twistPerMM*twistDirection])
			dxf_linear_extrude(file =dxfFile, layer = dxfLayer,
				height = h, center = true, convexity= 4, 
				twist=twistDirection*h*twistPerMM, slices=slicesPerMM*h);
	}	

	
	
	module BoxBottom() {
		difference() {
			union() {
				basicShape("outside_outside", 0, gripH);
				basicShape("inside_outside", 0, totalH-topFloorH);
			}
			basicShape("inside_inside", bottomFloorH, totalH);
		}
	}
	module BoxTop() {
		difference() {
			basicShape("outside_outside", 0, totalH-gripH);
			basicShape("outside_inside", bottomFloorH, totalH);
		}
	}

	if (topNotBottom == 0)
		BoxBottom();
	if (topNotBottom == 1)
		 BoxTop();
	if (topNotBottom == 3)
		 basicShape("outside_outside",0, 100);
}



