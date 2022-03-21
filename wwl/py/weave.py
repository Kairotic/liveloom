#!/usr/bin/env python3

from itertools import *
import operator as op
# pip3 install more-itertools
from more_itertools import *
# pip3 install pyserial
import serial
import glob

# find first arduino device
device = glob.glob("/dev/serial/by-id/*Arduino*")[0]

arduino = serial.Serial(device, baudrate=9600, timeout=0.1)
print("opened serial port", arduino.name)

warpcount = 16
showrows = 12

up = True
down = False

class Weave:
    """A weave is an iterable, 1D sequence of ups and downs"""
    def __init__(self, bits):
        self.bits = bits

    def show(self):
        rows = take(showrows, self.to_rows())
        for row in rows:
            for bit in row:
                if (bit):
                    print("-", end="")
                else:
                    print("|", end="")
            print("")

    def inv(self):
        """turn ups to downs, and vice-versa"""
        return Weave(map(op.not_, self.bits))

    def to_rows(self):
        """Breaks the weave into rows, returning an
        iterable list of lists of ups and downs"""
        return(chunked(self.bits, warpcount))

    def with_all_rows(self, function):
        """Applies the given function to each bit in a weave"""
        return Weave(flatten(map(function, self.to_rows())))

    def with_rows(self, function, which_rows):
        """Applies the given function for each row that has a corresponding 'true' value 
        in the `which_rows` iterable list
        """
        rows = self.to_rows()
        def f(apply, row):
            if apply:
                return function(row)
            else:
                return(row)
        return Weave(flatten(map(f, which_rows, rows)))

    def backforth(self):
        """Reverses every other row"""
        return self.with_rows(reversed, cycle([True,False]))

    def step(self, step_by=1):
        
        def rotate(row_number, offset):
            return(consume(self.bits, ))

        row_numbers = count(0,1)
        steps = count(0, step_by)

        return Weave(flatten(map(rotate, row_numbers, steps)))    

    def send_row(self):
        row = take(warpcount, self.bits)
        s = ""
        for pick in row:
            if pick:
                s = s + "1"
            else:
                s = s + "0"
        s = s + "x\n"
        arduino.write(s.encode("utf-8"))


twill = Weave(cycle([up,down,down])) # .step()
print("hello")
twill.show()

# Tabby = repeat up and down, going back and forth from row to row.
print("hello")
tabby = Weave(cycle([up,down])).backforth()
tabby.show()
print("attempting to send two rows of tabby in quick succession..")
tabby.send_row()
tabby.send_row()

