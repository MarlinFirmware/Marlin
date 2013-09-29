/*
  vector_3.cpp - Vector library for bed leveling
  Copyright (c) 2012 Lars Brubaker.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <math.h>
#include "Marlin.h"

#ifdef ENABLE_AUTO_BED_LEVELING
#include "vector_3.h"

vector_3::vector_3()
{
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

vector_3::vector_3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vector_3 vector_3::cross(vector_3 left, vector_3 right)
{
	return vector_3(left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x);
}

vector_3 vector_3::operator+(vector_3 v) 
{
	return vector_3((x + v.x), (y + v.y), (z + v.z));
}

vector_3 vector_3::operator-(vector_3 v) 
{
	return vector_3((x - v.x), (y - v.y), (z - v.z));
}

vector_3 vector_3::get_normal() 
{
	vector_3 normalized = vector_3(x, y, z);
	normalized.normalize();
	return normalized;
}

float vector_3::get_length() 
{
        float length = sqrt((x * x) + (y * y) + (z * z));
	return length;
}
 
void vector_3::normalize()
{
	float length = get_length();
	x /= length;
	y /= length;
	z /= length;
}

void vector_3::apply_rotation(matrix_3x3 matrix)
{
	float resultX = x * matrix.matrix[3*0+0] + y * matrix.matrix[3*1+0] + z * matrix.matrix[3*2+0];
	float resultY = x * matrix.matrix[3*0+1] + y * matrix.matrix[3*1+1] + z * matrix.matrix[3*2+1];
	float resultZ = x * matrix.matrix[3*0+2] + y * matrix.matrix[3*1+2] + z * matrix.matrix[3*2+2];

	x = resultX;
	y = resultY;
	z = resultZ;
}

void vector_3::debug(char* title)
{
	SERIAL_PROTOCOL(title);
	SERIAL_PROTOCOLPGM(" x: ");
	SERIAL_PROTOCOL(x);
	SERIAL_PROTOCOLPGM(" y: ");
	SERIAL_PROTOCOL(y);
	SERIAL_PROTOCOLPGM(" z: ");
	SERIAL_PROTOCOL(z);
	SERIAL_PROTOCOLPGM("\n");
}

void apply_rotation_xyz(matrix_3x3 matrix, float &x, float& y, float& z)
{
	vector_3 vector = vector_3(x, y, z);
	vector.apply_rotation(matrix);
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

matrix_3x3 matrix_3x3::create_from_rows(vector_3 row_0, vector_3 row_1, vector_3 row_2)
{
        //row_0.debug("row_0");
        //row_1.debug("row_1");
        //row_2.debug("row_2");
	matrix_3x3 new_matrix;
	new_matrix.matrix[0] = row_0.x; new_matrix.matrix[1] = row_0.y; new_matrix.matrix[2] = row_0.z; 
	new_matrix.matrix[3] = row_1.x; new_matrix.matrix[4] = row_1.y; new_matrix.matrix[5] = row_1.z; 
	new_matrix.matrix[6] = row_2.x; new_matrix.matrix[7] = row_2.y; new_matrix.matrix[8] = row_2.z; 
        //new_matrix.debug("new_matrix");
        
	return new_matrix;
}

void matrix_3x3::set_to_identity()
{
	matrix[0] = 1; matrix[1] = 0; matrix[2] = 0;
	matrix[3] = 0; matrix[4] = 1; matrix[5] = 0;
	matrix[6] = 0; matrix[7] = 0; matrix[8] = 1;
}

matrix_3x3 matrix_3x3::create_look_at(vector_3 target, vector_3 up)
{
    // There are lots of examples of look at code on the internet that don't do all these noramize and also find the position
    // through several dot products.  The problem with them is that they have a bit of error in that all the vectors arn't normal and need to be.
    vector_3 z_row = vector_3(-target.x, -target.y, -target.z).get_normal();
    vector_3 x_row = vector_3::cross(up, z_row).get_normal();
    vector_3 y_row = vector_3::cross(z_row, x_row).get_normal();

    //x_row.debug("x_row");
    //y_row.debug("y_row");
    //z_row.debug("z_row");
    
    matrix_3x3 rot = matrix_3x3::create_from_rows(vector_3(x_row.x, y_row.x, z_row.x),
                                vector_3(x_row.y, y_row.y, z_row.y),
                                vector_3(x_row.z, y_row.z, z_row.z));

    //rot.debug("rot");
    return rot;
}

matrix_3x3 matrix_3x3::create_inverse(matrix_3x3 original)
{
	//original.debug("original");
	float* A = original.matrix;
	float determinant = 
		+ A[0 * 3 + 0] * (A[1 * 3 + 1] * A[2 * 3 + 2] - A[2 * 3 + 1] * A[1 * 3 + 2])
		- A[0 * 3 + 1] * (A[1 * 3 + 0] * A[2 * 3 + 2] - A[1 * 3 + 2] * A[2 * 3 + 0])
		+ A[0 * 3 + 2] * (A[1 * 3 + 0] * A[2 * 3 + 1] - A[1 * 3 + 1] * A[2 * 3 + 0]);
	matrix_3x3 inverse;
	inverse.matrix[0 * 3 + 0] = +(A[1 * 3 + 1] * A[2 * 3 + 2] - A[2 * 3 + 1] * A[1 * 3 + 2]) / determinant;
	inverse.matrix[0 * 3 + 1] = -(A[0 * 3 + 1] * A[2 * 3 + 2] - A[0 * 3 + 2] * A[2 * 3 + 1]) / determinant;
	inverse.matrix[0 * 3 + 2] = +(A[0 * 3 + 1] * A[1 * 3 + 2] - A[0 * 3 + 2] * A[1 * 3 + 1]) / determinant;
	inverse.matrix[1 * 3 + 0] = -(A[1 * 3 + 0] * A[2 * 3 + 2] - A[1 * 3 + 2] * A[2 * 3 + 0]) / determinant;
	inverse.matrix[1 * 3 + 1] = +(A[0 * 3 + 0] * A[2 * 3 + 2] - A[0 * 3 + 2] * A[2 * 3 + 0]) / determinant;
	inverse.matrix[1 * 3 + 2] = -(A[0 * 3 + 0] * A[1 * 3 + 2] - A[1 * 3 + 0] * A[0 * 3 + 2]) / determinant;
	inverse.matrix[2 * 3 + 0] = +(A[1 * 3 + 0] * A[2 * 3 + 1] - A[2 * 3 + 0] * A[1 * 3 + 1]) / determinant;
	inverse.matrix[2 * 3 + 1] = -(A[0 * 3 + 0] * A[2 * 3 + 1] - A[2 * 3 + 0] * A[0 * 3 + 1]) / determinant;
	inverse.matrix[2 * 3 + 2] = +(A[0 * 3 + 0] * A[1 * 3 + 1] - A[1 * 3 + 0] * A[0 * 3 + 1]) / determinant;

	vector_3 row0 = vector_3(inverse.matrix[0 * 3 + 0], inverse.matrix[0 * 3 + 1], inverse.matrix[0 * 3 + 2]);
	vector_3 row1 = vector_3(inverse.matrix[1 * 3 + 0], inverse.matrix[1 * 3 + 1], inverse.matrix[1 * 3 + 2]);
	vector_3 row2 = vector_3(inverse.matrix[2 * 3 + 0], inverse.matrix[2 * 3 + 1], inverse.matrix[2 * 3 + 2]);

    row0.normalize();
    row1.normalize();
    row2.normalize();

	inverse = matrix_3x3::create_from_rows(row0, row1, row2);

	//inverse.debug("inverse");
	return inverse;
}

void matrix_3x3::debug(char* title)
{
	SERIAL_PROTOCOL(title);
	SERIAL_PROTOCOL("\n");
	int count = 0;
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			SERIAL_PROTOCOL(matrix[count]);
			SERIAL_PROTOCOLPGM(" ");
		        count++;
		}

		SERIAL_PROTOCOLPGM("\n");
	}
}

#endif // #ifdef ENABLE_AUTO_BED_LEVELING

