import argparse
import sys

def computeDistance(args):
    word_1 = args.firstString
    word_2 = args.secondString
    lw1=len(word_1)
    lw2=len(word_2)
    
    # for all i and j, d[i,j] will hold the Levenshtein distance between
    # the first i characters of word_1 and the first j characters of word_2
    # note that d has (lw1+1)*(lw2+1) values
    d=[[0 for j in range(lw2+1)] for i in range(lw1+1)]

    # source prefixes can be transformed into empty string by
    # dropping all characters 
    for i in range (0, lw1+1):
        d[i][0] = i
    
    # target prefixes can be reached from empty source prefix
    # by inserting every character
    for j in range (0, lw2+1):
        d[0][j] = j
    
    # minimum of delete char from s, delete char from t, and delete char from both
    for i in range (1, lw1+1):
        for j in range (1, lw2+1):
            if word_1[i-1]==word_2[j-1]:
                cost=0
            else:
                cost=1
            d[i][j]=min(d[i-1][j]+1, d[i][j-1]+1, d[i-1][j-1]+cost)
            
    #sys.stderr.write(str(d)+"\n")
    print(str(d[lw1][lw2]))
    
    
# Creating a parser
parser = argparse.ArgumentParser(description="Calculate the distance of Levenshtein between two chains.")

# Adding arguments
parser.add_argument('firstString', help='First string')
parser.add_argument("secondString", help='Second string')

# Parsing arguments
args = parser.parse_args()
# Usage
computeDistance(args)