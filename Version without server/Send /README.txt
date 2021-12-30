This is analysation of city network system 

Here you can calculate the most dangerous network tower to crash. 
You can input the connections between towers and then input quantity of people connected to one isolated tower and then receive an answer. 

To run: you have to open terminal in a folder with file main.cpp and data.txt and then write make -- makefile will compile programs automatically. Then write ./ready to run.


SYNTAX in file data.txt:

{
//here you set the connections between two towers
<tower_name_1>,<tower_name_2>
<tower_name_2>,<tower_name_3>
//...
<tower_name_x>,<tower_name_y>
}
{
//here you set value of people connected to the tower
<tower_name_1>:<<tower_name_1_value_of_people>
<tower_name_2>:<<tower_name_2_value_of_people>
/...
<tower_name_y>:<<tower_name_y_value_of_people>
}
