import sys
import math
import argparse

class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass

def createPattern(message):
    pattern = []
    c=1
    p = len(message)
    while sum(pattern) < p :
        string = message[:c]
        message = message[c:]
        pattern.append(len(string))
        c+=1
    return pattern

def decode(pattern, message):
    out = ''
    mod = len(pattern)%2
    for i in range(len(pattern)):
        value = int(pattern[i])
        if mod % 2 :
            string = message[len(message)-value:]
            message = message[:len(message)-value]
        else:
            string = message[:value]
            message = message[value:]
        out = string + out
        mod=mod+1
    return out
    
def encode(pattern, message):
    out = ''
    for i in range(len(pattern)):
        value = int(pattern[i])
        string = message[len(out):len(out)+value]
        out = string+out if i%2 else out+string
    return out

def convert(timesTransformed, message):
    pattern = createPattern(message)
    if timesTransformed > 0:
        r_pattern = pattern[::-1]
        for i in range(timesTransformed):
            message = decode(r_pattern, message)
        return message
    else:
        for i in range(abs(timesTransformed)):
            message = encode(pattern, message)
        return message

# Creating a parser
tab = "\t\t\t"
e = 'Finally, we have received the first messages from aliens!\n'
e += 'Unfortunately we cannot understand them because they have a very unique way of speaking.\n'
e += "\n"
e += "Example:\n"
e += '-n 1 -m "ghibcadef"\n'
e += '-n -1 -m "hello world"\n'
e += '-n 5 -m "hitoeplmu eneicldts aide  tsxt "\n'
e += '-n -6 -message "hello worlds"\n'
e += '-n 3 -m rius lorem. Duis risus nunc, condimentum at metun lacinia id. Pellentebortis. Suspendttis sed , maxis ornare nipulvinar. In v aliquam erat maximus bibenetus neque, tempus lovarius ipsnare vel. Donec , vitae sx enim. Sed vitaes sed nei ipFusces t. e at sum. Alt nibhgittidisse eu eteger id cursumque vel dui et libs.Maecenash. Suspendisse tristiqueeu condcondimentum atec orDui sitipsuorLem m dolteger quismus eget i ssim lacuss. Suspum feron arcu idvinar id eula elit in effiuspenlor. in blandem solm ne i psuc lorlicitudit ut acSIn luctus vcitur vae pulat arcu ferment maximus. Integerendisse hendrim. Inmentum nibh non dum.  amet, tur adlit. Fusceci pretium iacsi ut felibm neque, quis dignis orligsx nec sagi aliquam do maximuaodo nulla. isi quis, iquam esdu, npretium comMauris as. Ins elitque a mattittis. Morbi volutpat eroegestas irit vel ante ac dignisss nes scing elitconsecteoripi. Quisque msagiel puruuli mollis n enim est, ac bibendumissmentum. Ut dictum mi vel luctus rhoncus.tempor id.'
t = "Disordered First Contact.\n"
t += "Coding Games\n"
t += "https://www.codingame.com/ide/puzzle/disordered-first-contact\n"
t += "create by Klemek\n"
t += "resolved by Xeno_PXD\n"

parser = argparse.ArgumentParser(description=t, 
                                 epilog=e, formatter_class=CustomFormatter)

# Adding arguments
parser.add_argument('-n', dest='n', nargs=1, type=int, help='An integer N indicating the number of times the message was transformed. If N is positive you have to decode i.e. retrieve the original message. If N is negative you have to encode i.e. transform the message.')
parser.add_argument('-m', '--message', dest='m', metavar=('MESSAGE'), nargs=1, type=str, help='A string message to be decoded or encoded.')

# Parsing arguments
args = parser.parse_args()

print(convert(args.n[0], args.m[0]))