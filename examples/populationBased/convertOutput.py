with open("Output.txt") as f:
    content = f.readlines()

    runs = 0
    avgiter = 0
    miniter = 1000
    maxiter = 0
    result = 0
    isResult = False
    for line in content:
        if len(line) > 1:
            if isResult==True:
                runs+=1
                result+=float(line)
                isResult = False
            else:
                avgiter+=float(line)
                if miniter > float(line):
                    miniter = float(line)
                if maxiter < float(line):
                    maxiter = float(line)
                isResult = True
    avgiter = avgiter / runs
    result = result / runs
    print "number of runs: " + str(runs) + ", average iterations: " + str(avgiter) + ", min iterations: " + str(miniter) + ", max iterations: " + str(maxiter) + ", average result: " + str(result)
