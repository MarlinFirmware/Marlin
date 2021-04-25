 /**************************************\
 *                                      *
 *   OpenSCAD Mesh Display              *
 *   by Thinkyhead - April 2017         *
 *                                      *
 *   Copy the grid output from Marlin,  *
 *   paste below as shown, and use      *
 *   OpenSCAD to see a visualization    *
 *   of your mesh.                      *
 *                                      *
 \**************************************/

$t = 0.15; // comment out during animation!
X = 0; Y = 1;
L = 0; R = 1; F = 2; B = 3;

//
// Sample Mesh - Replace with your own
//
measured_z = [
  [ -1.20, -1.13, -1.09, -1.03, -1.19 ],
  [ -1.16, -1.25, -1.27, -1.25, -1.08 ],
  [ -1.13, -1.26, -1.39, -1.31, -1.18 ],
  [ -1.09, -1.20, -1.26, -1.21, -1.18 ],
  [ -1.13, -0.99, -1.03, -1.06, -1.32 ]
];

//
// An offset to add to all points in the mesh
//
zadjust     = 0;

//
// Mesh characteristics
//
bed_size = [ 200, 200 ];

mesh_inset  = [ 10, 10, 10, 10 ]; // L, F, R, B

mesh_bounds = [
  [ mesh_inset[L], mesh_inset[F] ],
  [ bed_size[X] - mesh_inset[R], bed_size[Y] - mesh_inset[B] ]
];

mesh_size = mesh_bounds[1] - mesh_bounds[0];

                      // NOTE: Marlin meshes already subtract the probe offset
NAN         = 0;      // Z to use for un-measured points

//
// Geometry
//

max_z_scale   = 100;   // Scale at Time 0.5
min_z_scale   = 10;    // Scale at Time 0.0 and 1.0
thickness     = 0.5;   // thickness of the mesh triangles
tesselation   = 1;     // levels of tesselation from 0-2
alternation   = 2;     // direction change modulus (try it)

//
// Appearance
//

show_plane    = true;
show_labels   = true;
show_coords   = true;
arrow_length  = 5;

label_font_lg = "Arial";
label_font_sm = "Arial";
mesh_color    = [1,1,1,0.5];
plane_color   = [0.4,0.6,0.9,0.6];

//================================================ Derive useful values

big_z = max_2D(measured_z,0);
lil_z = min_2D(measured_z,0);

mean_value = (big_z + lil_z) / 2.0;

mesh_points_y = len(measured_z);
mesh_points_x = len(measured_z[0]);

xspace = mesh_size[X] / (mesh_points_x - 1);
yspace = mesh_size[Y] / (mesh_points_y - 1);

// At $t=0 and $t=1 scale will be 100%
z_scale_factor = min_z_scale + (($t > 0.5) ? 1.0 - $t : $t) * (max_z_scale - min_z_scale) * 2;

//
// Min and max recursive functions for 1D and 2D arrays
// Return the smallest or largest value in the array
//
function some_1D(b,i) = (i<len(b)-1) ? (b[i] && some_1D(b,i+1)) : b[i] != 0;
function some_2D(a,j) = (j<len(a)-1) ? some_2D(a,j+1) : some_1D(a[j], 0);
function min_1D(b,i) = (i<len(b)-1) ? min(b[i], min_1D(b,i+1)) : b[i];
function min_2D(a,j) = (j<len(a)-1) ? min_2D(a,j+1) : min_1D(a[j], 0);
function max_1D(b,i) = (i<len(b)-1) ? max(b[i], max_1D(b,i+1)) : b[i];
function max_2D(a,j) = (j<len(a)-1) ? max_2D(a,j+1) : max_1D(a[j], 0);

//
// Get the corner probe points of a grid square.
//
// Input  : x,y grid indexes
// Output : An array of the 4 corner points
//
function grid_square(x,y) = [
  [x * xspace, y * yspace, z_scale_factor * (measured_z[y][x] - mean_value)],
  [x * xspace, (y+1) * yspace, z_scale_factor * (measured_z[y+1][x] - mean_value)],
  [(x+1) * xspace, (y+1) * yspace, z_scale_factor * (measured_z[y+1][x+1] - mean_value)],
  [(x+1) * xspace, y * yspace, z_scale_factor * (measured_z[y][x+1] - mean_value)]
];

// The corner point of a grid square with Z centered on the mean
function pos(x,y,z) = [x * xspace, y * yspace, z_scale_factor * (z - mean_value)];

//
// Draw the point markers and labels
//
module point_markers(show_home=true) {
  // Mark the home position 0,0
  if (show_home)
    translate([1,1]) color([0,0,0,0.25])
      cylinder(r=1, h=z_scale_factor, center=true);

  for (x=[0:mesh_points_x-1], y=[0:mesh_points_y-1]) {
    z = measured_z[y][x] - zadjust;
    down = z < mean_value;
    xyz = pos(x, y, z);
    translate([ xyz[0], xyz[1] ]) {

      // Show the XY as well as the Z!
      if (show_coords) {
        color("black")
        translate([0,0,0.5]) {
          $fn=8;
          rotate([0,0]) {
            posx = floor(mesh_bounds[0][X] + x * xspace);
            posy = floor(mesh_bounds[0][Y] + y * yspace);
            text(str(posx, ",", posy), 2, label_font_sm, halign="center", valign="center");
          }
        }
      }

      translate([ 0, 0, xyz[2] ]) {
        // Label each point with the Z
        v = z - mean_value;
        if (show_labels) {

          color(abs(v) < 0.1 ? [0,0.5,0] : [0.25,0,0])
          translate([0,0,down?-10:10]) {

            $fn=8;
            rotate([90,0])
              text(str(z), 6, label_font_lg, halign="center", valign="center");

            if (v)
              translate([0,0,down?-6:6]) rotate([90,0])
                text(str(down || !v ? "" : "+", v), 3, label_font_sm, halign="center", valign="center");
          }
        }

        // Show an arrow pointing up or down
        if (v) {
          rotate([0, down ? 180 : 0]) translate([0,0,-1])
            cylinder(
              r1=0.5,
              r2=0.1,
              h=arrow_length, $fn=12, center=1
            );
        }
        else
          color([1,0,1,0.4]) sphere(r=1.0, $fn=20, center=1);
      }
    }
  }
}

//
// Split a square on the diagonal into
// two triangles and render them.
//
//     s : a square
//   alt : a flag to split on the other diagonal
//
module tesselated_square(s, alt=false) {
  add = [0,0,thickness];
  p1 = [
    s[0], s[1], s[2], s[3],
    s[0]+add, s[1]+add, s[2]+add, s[3]+add
  ];
  f1 = alt
      ? [ [0,1,3], [4,5,1,0], [4,7,5], [5,7,3,1], [7,4,0,3] ]
      : [ [0,1,2], [4,5,1,0], [4,6,5], [5,6,2,1], [6,4,0,2] ];
  f2 = alt
      ? [ [1,2,3], [5,6,2,1], [5,6,7], [6,7,3,2], [7,5,1,3] ]
      : [ [0,2,3], [4,6,2,0], [4,7,6], [6,7,3,2], [7,4,0,3] ];

  // Use the other diagonal
  polyhedron(points=p1, faces=f1);
  polyhedron(points=p1, faces=f2);
}

/**
 * The simplest mesh display
 */
module simple_mesh(show_plane=show_plane) {
  if (show_plane) color(plane_color) cube([mesh_size[X], mesh_size[Y], thickness]);
  color(mesh_color)
    for (x=[0:mesh_points_x-2], y=[0:mesh_points_y-2])
      tesselated_square(grid_square(x, y));
}

/**
 * Subdivide the mesh into smaller squares.
 */
module bilinear_mesh(show_plane=show_plane,tesselation=tesselation) {
  if (show_plane) color(plane_color) translate([-5,-5]) cube([mesh_size[X]+10, mesh_size[Y]+10, thickness]);

  if (some_2D(measured_z, 0)) {

    tesselation = tesselation % 4;
    color(mesh_color)
    for (x=[0:mesh_points_x-2], y=[0:mesh_points_y-2]) {
      square = grid_square(x, y);
      if (tesselation < 1) {
        tesselated_square(square,(x%alternation)-(y%alternation));
      }
      else {
        subdiv_4 = subdivided_square(square);
        if (tesselation < 2) {
          for (i=[0:3]) tesselated_square(subdiv_4[i],i%alternation);
        }
        else {
          for (i=[0:3]) {
            subdiv_16 = subdivided_square(subdiv_4[i]);
            if (tesselation < 3) {
              for (j=[0:3]) tesselated_square(subdiv_16[j],j%alternation);
            }
            else {
              for (j=[0:3]) {
                subdiv_64 = subdivided_square(subdiv_16[j]);
                if (tesselation < 4) {
                  for (k=[0:3]) tesselated_square(subdiv_64[k]);
                }
              }
            }
          }
        }
      }
    }

  }
}

//
// Subdivision helpers
//
function ctrz(a) = (a[0][2]+a[1][2]+a[3][2]+a[2][2])/4;
function avgx(a,i) = (a[i][0]+a[(i+1)%4][0])/2;
function avgy(a,i) = (a[i][1]+a[(i+1)%4][1])/2;
function avgz(a,i) = (a[i][2]+a[(i+1)%4][2])/2;

//
// Convert one square into 4, applying bilinear averaging
//
// Input  : 1 square (4 points)
// Output : An array of 4 squares
//
function subdivided_square(a) = [
  [ // SW square
    a[0],                          // SW
    [a[0][0],avgy(a,0),avgz(a,0)], // CW
    [avgx(a,1),avgy(a,0),ctrz(a)], // CC
    [avgx(a,1),a[0][1],avgz(a,3)]  // SC
  ],
  [ // NW square
    [a[0][0],avgy(a,0),avgz(a,0)], // CW
    a[1],                          // NW
    [avgx(a,1),a[1][1],avgz(a,1)], // NC
    [avgx(a,1),avgy(a,0),ctrz(a)]  // CC
  ],
  [ // NE square
    [avgx(a,1),avgy(a,0),ctrz(a)], // CC
    [avgx(a,1),a[1][1],avgz(a,1)], // NC
    a[2],                          // NE
    [a[2][0],avgy(a,0),avgz(a,2)]  // CE
  ],
  [ // SE square
    [avgx(a,1),a[0][1],avgz(a,3)], // SC
    [avgx(a,1),avgy(a,0),ctrz(a)], // CC
    [a[2][0],avgy(a,0),avgz(a,2)], // CE
    a[3]                           // SE
  ]
];


//================================================ Run the plan

translate([-mesh_size[X] / 2, -mesh_size[Y] / 2]) {
  $fn = 12;
  point_markers();
  bilinear_mesh();
}
