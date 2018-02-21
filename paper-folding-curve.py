import sys
import math
import argparse
from time import sleep

class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass

# Print iterations progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = 'X'):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    sys.stdout.write('\r%s |%s| %s%% %s \r' % (prefix, bar, percent, suffix))
    sys.stdout.flush()
    #print('\r%s |%s| %s%% %s' % (prefix, bar, percent, suffix), end = '\r')
    # Print New Line on Complete
    if iteration == total: 
        print()

  
global compteurProgressBar
compteurProgressBar=0

# Pour compter le nombre de virage total
def bin(order):
    c=0
    out=0
    while c < order:
        pas=pow(2, c)
        out+=pas
        c+=1
    return out

def path(order, s, e, file):
    start=s+1
    current=s+1
    end=e
    modulo=2
    if (file!=""):
        printProgressBar(0, end-start, prefix = 'Progress:', suffix = 'Complete  ', length = 50)
    while s <= e:
        curve(order, start, current, end, modulo, file)
        current+=1
        s+=1
    #sys.stdout.write("\n")
    
def curve(order, start, current, end, modulo, file):
    global compteurProgressBar
    if current<=end+1:
        if (current % modulo):
            value = ("0", "1")[int(math.ceil(current / modulo)) % 2]
            if (file==""):
                sys.stdout.write(value)
                sys.stdout.flush()
            else:
                printProgressBar(compteurProgressBar-1, end-start, prefix = 'Progress:', suffix = 'Complete  ', length = 50)
                compteurProgressBar+=1
                file.write(value)     
        else:
            curve(order-1, start, current, end, modulo*2, file)


# Creating a parser
tab = "\t\t\t"
e = 'Get a stripe of paper.\nFold it into half.\n'
e += 'Fold it into half again in the same direction.\n'
e += 'Fold it the third time into half, in the same direction.\n'
e += 'Then open it, so that every fold is at a right angle.\n'
e += 'Viewing it from the edge you could see a pattern like this:\n'
e += '   _ \n'
e += '|_| |_\n'
e += '     _|\n'
e += "\n"
e += "Example:\n"
e += '--order 3 --start 3 --end 6\n'
e += '--order 5 --start 16 --end 18\n'
e += '--order 10 --start 999 --end 1011\n'
e += '--order 20 --start 100 --end 111\n'
e += '--order 50 --start 562949953421300 --end 562949953421400\n'
e += "\n"
e += "Output:\n"
e += '1100\n'
e += '110\n'
e += '1110010001101\n'
e += '100111001000\n'
e += '10001100100111011001110010011101100011001001110110011100100011011000110010011101100111001001110110001\n'

t = "Paper-folding curve.\n"
t += "Coding Games\n"
t += "https://www.codingame.com/training/community/paper-folding-curve\n"
t += "create by java_coffee_cup\n"
t += "resolved by Xeno_PXD\n"


parser = argparse.ArgumentParser(description=t, 
                                 epilog=e, formatter_class=CustomFormatter)

# Adding arguments
parser.add_argument('-o', '--order', dest='order', nargs=1, type=int, help='An integer N for the order of the curve')
parser.add_argument('-s', '--start', dest='s', metavar=('START'), nargs=1, type=int, help='A starting index')
parser.add_argument('-e', '--end', dest='e', metavar=('END'), nargs=1, type=int, help='A ending index')
parser.add_argument('-out', '--output', dest='output', metavar=('OUTPUT'), nargs=1, type=str, help='output file')
parser.add_argument('-n', '--number', dest='number', action='store_true', help='return number curve')

# Parsing arguments
args = parser.parse_args()
nb=bin(args.order[0])
start=0
end=0

if (args.number):
    print(nb)
    exit()
    
if (args.e is None):
    end = nb
else:
    if args.e[0]>nb:
        end=nb
    else:
        end=args.e[0]
        
if (args.s is None):
    start=0
else:
    if args.s[0]<0:
        start=0
    else:
        start=args.s[0]

file=""
if  (args.output is None):
    file=""
else:
    file = open(args.output[0], 'w+')

#print(str(args.order) +" "+ str(args.s) +" "+ str(args.e))
path(args.order[0], start, end, file)