import os
import inspect
import numpy as np
import gdal
from driver import *

def getParameters(fo):
    
    """
    Function: getParameters
    Description: The following function copies the parameter section into a new document
    """
    
    fw = open("parameterValues.param", 'w')
    always_print = False
    lines = fo.readlines()
    for line in lines:
        if always_print or "Parameters" in line:
            fw.write(line)
            always_print = True

def valueType(typeOfValues):

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
    Description: The following function stores parameter values in separate files
    """

    directoryName = "DataConverter"
    #nameOfMonths = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
    index = 0
               
    if not os.path.exists(directoryName):
        os.makedirs(directoryName)

    fr = open("parameterValues.param", 'r')
    for line in fr:
        if "####" in line:
            parameterName = fr.next().strip()
            numberOfDimensions = int(fr.next())
            for i in range(numberOfDimensions):
                fr.next()
            numberOfValues = int(fr.next())
            typeOfValues = int(fr.next())
            value = valueType(typeOfValues)
            name = np.arange(value).reshape(49,96) 
            if numberOfValues == 4704:
                for j in range(49): 
                    for k in range(96):
                        value = float(fr.next().strip())
                        name[j,k] = value
                
            elif numberOfValues == 56448:
                for i in range(12):
                    for j in range(49): 
                        for k in range(96):
                            value = float(fr.next().strip())
                            name[j,k] = value
                    index = index + 1
                index = 0 
            writeRaster("out.tif",name,96,49,100,100,108,101,4326,driver='gtiff')
                       
fo = open("LC.param", 'r')
getParameters(fo)
data = getValues()








