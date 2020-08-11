Final Project

Compiling: 
1) The following project can be compiled using "g++ main.cpp -lglut -lGL".
2) Once the .out file has been made, you can run the program via command-line
3) When running the program, use a world.txt file as the next argument

The goal of this project was to create a CAD like editor in the 3D space to provide wireframe 
drawings. In the program, users can specify points through a file to create objects. 
 
The layout of the file is described as follows: first line states the number of shapes to be drawn, 
following line indicates the number of points in the shape, and the rest of the file indicates the x, 
y, and z coordinates. The number of points and coordinates are repeated until all shapes have 
been drawn. An example of the file format is shown below.


![image](https://github.com/Amit97-ops/wireframe-render/blob/master/world.png)
 
The viewing space also includes a 3D grid for users to understand the coordinates of the 
designed object. Users are allowed to change the viewing angle of a specified axis by pressing 
the x, y, or z keys to rotate around their respective axis. AitkenÕs interpolation was used to create 
the wireframe drawings. Some examples are shown below.  
     
 
