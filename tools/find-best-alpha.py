import sys
import re

belief = [ 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0 ]

lastAction = None
lastAlphaList = None

bestAction = None
bestAlphaList = None
bestSum = 0.0
for line in sys.stdin:
    if len(line.strip()) < 1:
        continue
    if re.search( r'^\s*\d+\s*$', line ):
        lastAction = int(line)
        continue
    lastAlphaList = line.strip().split( " " )

    print "Last: A=%d, V=%s" % ( lastAction, ", ".join( lastAlphaList ))

    lastSum = 0.0
    for i in xrange(0, len(belief)):
        lastSum += float(lastAlphaList[i]) * belief[i]
        continue

    print "Last Sum = %s" % lastSum
    
    if lastSum > bestSum:
        bestAction = lastAction
        bestAlphaList = lastAlphaList
    continue

print "\nBEST:\nA=%d, V=%s" % ( bestAction, ", ".join( bestAlphaList ))

