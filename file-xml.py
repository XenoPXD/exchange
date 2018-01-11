import argparse
import sys
import xml.etree.ElementTree as ElementTree

# author : Xavier Prunck (2018)

# https://docs.python.org/3/library/xml.etree.elementtree.html
ELEMENT_NODE=1
ATTRIBUTE_NODE=2
TEXT_NODE=3

class CustomFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass

class MyTreeBuilder(ElementTree.TreeBuilder):
   def comment(self, data):
       self.start(ElementTree.Comment, {})
       self.data(data)
       self.end(ElementTree.Comment)
      
def indent(elem, tab, level=0):
    i = "\n" + level*"\t"
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "\t"
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i
             
def detectType(args):
    posAttribut=args.xpath.find('/@')
    if posAttribut>0:
        args.typeNode=ATTRIBUTE_NODE
        args.name=args.xpath[posAttribut+2:]
        args.xpath=args.xpath[:posAttribut]
        
    posText=args.xpath.find('/text()')
    if posText>0:
        args.xpath=args.xpath[:posText]
        args.typeNode=TEXT_NODE
    
    if posAttribut<0 and posText<0:
        args.typeNode=ELEMENT_NODE
    
def read(args):
    tree = ElementTree.parse(args.file)
    root = tree.getroot()
    for node in root.findall(args.xpath):
        if args.typeNode==ATTRIBUTE_NODE:
            print(node.get(args.name))
        if args.typeNode==TEXT_NODE:
            print(node.text)
        if args.typeNode==ELEMENT_NODE:
            xmlstr = ElementTree.tostring(node, encoding='utf8', method='xml')
            sys.stdout.write(xmlstr.decode("utf-8") )       
            
def set(args):
    tree = ElementTree.parse(args.file, parser=ElementTree.XMLParser(target=MyTreeBuilder()))
    root = tree.getroot()
    print(args.xpath)
    for node in root.findall(args.xpath):
        if args.typeNode==ATTRIBUTE_NODE:
            node.set(args.name, args.set[0])
        if args.typeNode==TEXT_NODE:
            node.text=args.set[0]
            print(node.text)
        if args.typeNode==ELEMENT_NODE:
            newNode = ElementTree.fromstring(args.set[0])
            print (ElementTree.tostring(newNode))
            node.append(newNode)
    writeFile(args, tree)  
    
def comment(args):
    tree = ElementTree.parse(args.file)
    root = tree.getroot()
    print(args.xpath)
    for node in root.findall(args.xpath):
        if args.typeNode==ATTRIBUTE_NODE:
            node.set(args.name, args.set[0])
        if args.typeNode==TEXT_NODE:
            node.text=args.set[0]
            print(node.text)
        if args.typeNode==ELEMENT_NODE:
            #ElementTree.comment(node)
            node.start(ET.Comment, {})
            node.Comment
            
    # write(file, encoding="us-ascii", xml_declaration=None, default_namespace=None, method="xml", *, short_empty_elements=True)
    isXml_declaration=None
    strEncoding=""
    if args.xmlDeclaration:
        isXml_declaration=True
        strEncoding=args.xmlDeclaration[0]
    #print(isXml_declaration)
    #print(strEncoding)
    tree.write(args.file.name, encoding=strEncoding, xml_declaration=isXml_declaration, method='xml')
    
def remove(args):
    tree = ElementTree.parse(args.file)
    root = tree.getroot()
    print(args.xpath)
    for node in root.findall(args.xpath):
        if args.typeNode==ATTRIBUTE_NODE:
            node.attrib.pop(args.name, None)
        if args.typeNode==TEXT_NODE:
            node.clear()
        if args.typeNode==ELEMENT_NODE:
            node.clear()
    writeFile(args, tree)        
    
def getXmlstr(fileName):
    treeSet = ElementTree.parse(fileName)
    rootSet = treeSet.getroot()
    return ElementTree.tostring(rootSet)
        
def writeFile(args, tree):
    strEncoding=""
    if args.encoding:
        strEncoding=args.encoding[0]
    isXml_declaration=None
    if args.declaration:
        isXml_declaration=True
    if args.indent and args.indent[0]:
        indent(tree.getroot(), args.indent[1], args.indent[2])
    tree.write(args.file.name, encoding=strEncoding, xml_declaration=isXml_declaration, method='xml')
    
# Creating a parser
tab = "\t\t\t"
e = "Supported XPath syntax:\n"
e += "Syntax"+tab+"Meaning\n"
e += "tag"+tab+"Selects all child elements with the given tag. For example, spam selects all child elements named spam, and spam/egg selects all grandchildren named egg in all children named spam.\n"
e += "*"+tab+"Selects all child elements. For example, */egg selects all grandchildren named egg.\n"
e += "."+tab+"Selects the current node. This is mostly useful at the beginning of the path, to indicate that it's a relative path.\n"
e += "//"+tab+"Selects all subelements, on all levels beneath the current element. For example, .//egg selects all egg elements in the entire tree.\n"
e += ".."+tab+"Selects the parent element. Returns None if the path attempts to reach the ancestors of the start element (the element find was called on).\n"
e += "[@attrib]"+"\t\t"+"Selects all elements that have the given attribute.\n"
e += "[@attrib='value']"+"\t"+"Selects all elements for which the given attribute has the given value. The value cannot contain quotes.\n"
e += "[tag]"+tab+"Selects all elements that have a child named tag. Only immediate children are supported.\n"
e += "[tag='text']"+"\t\t"+"Selects all elements that have a child named tag whose complete text content, including descendants, equals the given text.\n"
e += "[position]"+"\t\t"+"Selects all elements that are located at the given position. The position can be either an integer (1 is the first position), the expression last() (for the last position), or a position relative to the last position (e.g. last()-1).\n"

e += "text()"+tab+"Selects text of the current node.\n"
e += "@attrib"+tab+"Selects attrib of the current node.\n"
e += "\n"
e += "Example:\n"
e += 'file-xml country_data.xml --xpath ".//neighbor[@name='+chr(39)+'Costa Rica'+chr(39)+']" --read\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank/text()" --set 12\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank/@test" --set ok --declaration\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank" --set "<c test='+chr(39)+'test'+chr(39)+'/>" --declarationutf-8 --indent\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank" --setFile input.xml --declaration --encoding utf-8\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank/@updated" --remove --declaration --encoding utf-8\n'
e += 'file-xml country_data.xml --xpath ".//country[@name='+chr(39)+'Panama'+chr(39)+']/rank/text()" --remove --declaration --encoding utf-8\n'


parser = argparse.ArgumentParser(description="XML file modifier.", epilog=e, formatter_class=CustomFormatter)

# Adding arguments
parser.add_argument('file', type=argparse.FileType('r+'), help='file xml')
parser.add_argument('xpath', type=str, help='xpath')
parser.add_argument('-r', '--read', dest='read', action='store_true', help='read')
parser.add_argument('-s', '--set', dest='set', nargs=1, type=str, help='set text')
parser.add_argument('-S', '--setFile', dest='setFile', nargs=1, type=str, help='set File')
#parser.add_argument("--comment", dest='comment', action='store_true', help='comment')
parser.add_argument('-R', '--remove', dest='remove', action='store_true', help='remove')
parser.add_argument('-i', '--indent', dest='indent', metavar=('ACTIVE', 'TAB', 'LEVEL'), nargs=3, default=[False, '\t', 0], help='indent printing XML (default=%(default)s)')
parser.add_argument('-d', '--declaration', dest='declaration', action='store_true', help='declaration XML (default=%(default)s)')
parser.add_argument('-e', '--encoding', dest='encoding', nargs=1, type=str, default=['utf-8'], help='encoding (default=%(default)s)')

# Parsing arguments
args = parser.parse_args()
# Usage
print(args.file.name)
print(args.xpath)
print(args.indent[0])
print(args.indent[1])
#print(args.set[0])

if (args.remove):
    detectType(args)
    remove(args)

if (args.comment):
    detectType(args)
    comment(args)
    
if (args.read):
    detectType(args)
    read(args)
    
if (args.set):
    detectType(args)
    set(args)
    
if (args.setFile):
    detectType(args)
    xmlstr = getXmlstr(args.setFile[0])
    args.set = [0]
    args.set[0] = xmlstr.decode("utf-8")
    set(args)