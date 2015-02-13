import os
import inspect
import numpy as np
import gdal
from driver import *

def getParameters(fo):
    
    """
    Function: getParameters
    Description: getParameters function copies the parameter section from the input file to a new file
    """
    
    fw = open("parameterValues.param", 'w')
    always_print = False
    lines = fo.readlines()
    for line in lines:
        if always_print or "Parameters" in line:
            fw.write(line)
            always_print = True

def getType(typeOfValues):

    value_1 = 4704
    value_2 = 4704.0

    if typeOfValues == 1:
        value = value_1
    elif typeOfValues == 2:
        value = value_2
    return value

def getValues():
    
    """
    Function: getValues
    Description: getValues function creates 2D arrays having parameter values 
    """
    
    index = 0

    def steps():
        for j in range(49): 
            for k in range(96):
                value = float(fr.next().strip())
                nameOfArray[j,k] = value
               
    fr = open("parameterValues.param", 'r')
    for line in fr:
        if "####" in line:
            parameterName = fr.next().strip()
            numberOfDimensions = int(fr.next())
            
            for i in range(numberOfDimensions):
                fr.next()

            numberOfValues = int(fr.next())
            typeOfValues = int(fr.next())
            value = getType(typeOfValues)
            nameOfArray = np.arange(value).reshape(49,96) 
            
            if numberOfValues == 4704:
                steps()
                
            elif numberOfValues == 56448:
                for i in range(12):
                    steps()
                    index = index + 1
                index = 0 
            writeRaster("output.tif",nameOfArray,96,49,100,100,108,101,4326,driver='gtiff')
                       
fo = open("LC.param", 'r')
getParameters(fo)
data = getValues()








