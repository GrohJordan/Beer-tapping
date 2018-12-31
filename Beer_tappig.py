##Copyright (c) 2018, Jordan Groh.
##
##This program is free software: you can redistribute it and/or modify
##it under the terms of the GNU General Public License as published by
##the Free Software Foundation, either version 3 of the License, or
##(at your option) any later version.
##
##This program is distributed in the hope that it will be useful,
##but WITHOUT ANY WARRANTY; without even the implied warranty of
##MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##GNU General Public License for more details.
##
##You should have received a copy of the GNU General Public License
##along with this program.  If not, see <http://www.gnu.org/licenses/>.


import serial
from matplotlib import pyplot;
from pylab import genfromtxt;

ser  = serial.Serial('/dev/ttyACM0')
ser.baudrate = 115200
ser.timeout = 0.001
print(ser)
i = 1
with open("Bier_tapping_data.txt", "w+") as f:


    while i != 512:
        
        data = ser.readline()
        datastring = str(data)
        wert = datastring[:-5]
        wert = wert.replace("b'", "")
        if wert != "":
            print(wert)
            f.write(wert)
            f.write('\n\r')
            i = i + 1
        
        



y  =  genfromtxt("Bier_tapping_data.txt")
x  =  [ e/4  for  e  in  range(511)]

pyplot.plot(x, y, label = "Druck")
pyplot.legend()
pyplot.ylabel("Welle")
pyplot.xlabel("Zeit in ms")
pyplot.show()
