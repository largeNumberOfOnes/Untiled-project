import sys
import os

#? ############################################################################
#? ############################################################################

black  = 30
red    = 31
green  = 32
yellow = 33
cyan   = 34
purple = 35
cyan   = 36
white  = 37

color = lambda col, s: f'\033[{col}m' + s + f'\033[0m'
pcol  = lambda col, s: print(color(col, s))

#? ############################################################################
#? ############################################################################

key = sys.argv[1]


pcol(purple, f"Start collecting project with key '{key}':")

links = {
    simpleLib = 
}

print(sys.argv)

print(os.system('ls'))
