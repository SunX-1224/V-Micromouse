# V-MicroMouse
This repository is for MicroMouse done as a project by out Team in Robotics Club of WRC. (if not updated, this repo only contains simulations for maze solving algorithm for micromouse with shortest path finding mechanism.)

This project was divided into 2 parts,

* Simulation : 
 > This part focused on simulating different maze solving algorithms best suited for our purposes.
 
 * Implementation
 > This part includes several stages like, Designing the bot, electronics, code and testing.


# LFRB pixelmaze
* In order to test other mazes using this program, generate a pixel maze of wall size 1 pixel by using any means, ( I used this generator : https://keesiemeijer.github.io/maze-generator/  )
* Here, set wall thickness to 1 and take the maze of any size you like.
* In the program, set the start and end positions according to your maze and also set the variable "facing" to correct value as required for your maze
* Initial mapping is indicated by blackish-gray colour and retracing is indicated by dark-green colour. And finally the shorted path is of cyan colour.

# LFRB linemaze
Algorithm is same as above but here I wanted to spice things up, so I created a micromouse bot solving the maze based on sensors which read the value of pixel below them.

* For testing this program, generate a line maze of line's thickness of 6 pixel from any means ( I used : https://www.mazegenerator.net/ )
* Save the generated maze as line in png or any other image format
* The maze should have black rectangle of width(perpendicular ko maze line) greater than 24 pixels and height at least 6 pixels as end point.
* Set the start position to anywhere in the maze and set facing to any valid direction.

(This one is not well-tested, so it may not work for every maze.. also any small flaw in the maze may affect the sensor reading. so, tweak anything you like while testing the program.)

# Team V-Robotics
* Sunil Sapkota
* Nisha Pandey
* Siddhartha Gupta
* Sumit Sigdel
* Sanjog Sapkota
* Sandesh Ghorasai
* Safal Lamsal
* Nirmal Adhikari
* Aakash Rai
