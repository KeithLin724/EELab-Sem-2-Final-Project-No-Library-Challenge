'''
Title:tran 
Written By Keith Lin
This is a code for change the format about the hex code 
hex code is for display 
Date: 23/5/2022
'''

strTranList = ['B1111110',
               'B0110000',
               'B1101101',
               'B1111001',
               'B0110011',
               'B1011011',
               'B1011111',
               'B1110000',
               'B1111111',
               'B1111011']
outPrint = []
inP = ['B1100011']

for i in inP:
    strList = list(i)
    tmp = strList[1:]  # take out the B
    tmp = tmp[::-1]

    outPrint.append('B' + ''.join(tmp))

print(',\n'.join(outPrint))
